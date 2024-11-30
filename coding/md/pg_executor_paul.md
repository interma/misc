```C
pp
pguo      81877      1  0 Mar30 pts/5    00:00:00 /data2/github/postgres/install/bin/postgres -D ../data
pguo      81879  81877  0 Mar30 ?        00:00:00 postgres: checkpointer process
pguo      81880  81877  0 Mar30 ?        00:00:01 postgres: writer process
pguo      81881  81877  0 Mar30 ?        00:00:01 postgres: wal writer process
pguo      81882  81877  0 Mar30 ?        00:00:01 postgres: autovacuum launcher process
pguo      81883  81877  0 Mar30 ?        00:00:01 postgres: stats collector process
pguo      81897   4052  0 Mar30 pts/5    00:00:00 bin/psql postgres
pguo      81898  81877  0 Mar30 ?        00:00:00 postgres: pguo postgres [local] idle

$ gdb -p 81898
(gdb) bt
#0  0x0000003397ee8f43 in __epoll_wait_nocancel () at ../sysdeps/unix/syscall-template.S:82
#1  0x0000000000698876 in WaitEventSetWaitBlock (set=0xdd1d40, timeout=-1, occurred_events=0x7ffff9f27bc0, nevents=1) at latch.c:990
#2  WaitEventSetWait (set=0xdd1d40, timeout=-1, occurred_events=0x7ffff9f27bc0, nevents=1) at latch.c:944
#3  0x00000000005e0040 in secure_read (port=0xdf5f70, ptr=0xbfac20, len=8192) at be-secure.c:149
#4  0x00000000005e7ce8 in pq_recvbuf () at pqcomm.c:921
#5  0x00000000005e87b5 in pq_getbyte () at pqcomm.c:964
#6  0x00000000006b6e52 in SocketBackend (argc=Unhandled dwarf expression opcode 0xf3
) at postgres.c:334
#7  ReadCommand (argc=Unhandled dwarf expression opcode 0xf3
) at postgres.c:507
#8  PostgresMain (argc=Unhandled dwarf expression opcode 0xf3
) at postgres.c:4021
#9  0x000000000046b0ea in BackendRun () at postmaster.c:4271
#10 BackendStartup () at postmaster.c:3945
#11 ServerLoop () at postmaster.c:1701
#12 0x000000000065ef39 in PostmasterMain (argc=3, argv=Unhandled dwarf expression opcode 0xf3
) at postmaster.c:1309
#13 0x000000000046bbc0 in main (argc=3, argv=0xdcfce0) at main.c:228

(gdb) b PortalRun
Breakpoint 1 at 0x6b9fe0: file pquery.c, line 712.
(gdb) c
Continuing.

/* 在psql运行select * from t1后 */

Breakpoint 1, PortalRun (portal=0xdef310, count=9223372036854775807, isTopLevel=1 '\001', dest=0xe85a20, altdest=0xe85a20, completionTag=0x7ffff9f27d80 "INSERT 0 1") at pquery.c:712
712	{

(gdb) bt
#0  PortalRun (portal=0xdef310, count=9223372036854775807, isTopLevel=1 '\001', dest=0xe85a20, altdest=0xe85a20, completionTag=0x7ffff9f27d80 "INSERT 0 1") at pquery.c:712
#1  0x00000000006b7f86 in exec_simple_query (argc=Unhandled dwarf expression opcode 0xf3
) at postgres.c:1094
#2  PostgresMain (argc=Unhandled dwarf expression opcode 0xf3
) at postgres.c:4076
#3  0x000000000046b0ea in BackendRun () at postmaster.c:4271
#4  BackendStartup () at postmaster.c:3945
#5  ServerLoop () at postmaster.c:1701
#6  0x000000000065ef39 in PostmasterMain (argc=3, argv=Unhandled dwarf expression opcode 0xf3
) at postmaster.c:1309
#7  0x000000000046bbc0 in main (argc=3, argv=0xdcfce0) at main.c:228
(gdb)
```

总结:
- Postmaster fork a new backend process in BackendStartup() after psl connection.

  **优点**：实现逻辑相对简单，更加关注与query本身，因此大量全局变量。使用进程隔离好：租户安全、资源控制、错误隔离。

  **缺点**：大量长连接context switch需求较多。使用nginx/mysql的preforked worker process：可以多个query共享一个process（逻辑将会复杂因为要自己记录上下文而非交给内核）。OR一个worker一次只能一个query？
  
  **思考**: 你会用什么方式实现一个数据库？调度交给内核（process）还是语言（golang scheduler）还是自己（epoll like）？OLTP vs OLAP.
  
- Typical call path for a simple query:
  
  BackendStartup()-> BackendRun()->PostgresMain()->exec_simple_query()->PortalRun()
- 注意全局变量：IsUnderPostmaster is true in postmaster child processes
- libpq:
 
  在连接的时候，包的格式似乎是这个函数：pqBuildStartupPacket3()

  在发送query的时候，typically psql calls PQsendQuery()，这种就是所谓的"simple query". payload以'Q'开头。PostgresMain()中发现firstchar为'Q'的时候call into exec_simple_query()处理.

PostgresMain():
```C
        switch (firstchar)
        {
            case 'Q':           /* simple query */
                {
                    const char *query_string;

                    /* Set statement_timestamp() */
                    SetCurrentStatementStartTimestamp();

                    query_string = pq_getmsgstring(&input_message);
                    pq_getmsgend(&input_message);

                    if (am_walsender)
                        exec_replication_command(query_string);
                    else
                        exec_simple_query(query_string);

                    send_ready_for_query = true;
                }
                break;
```
因此似乎pg9.6上的wal replication是通过libpq。

**问题**：似乎是单线程／进程的，可以多线程/进程提速吗？有必要吗？

```
graph TD
  BackendStartup --> BackendRun
  BackendRun --> PostgresMain
  PostgresMain --> exec_simple_query
```

```C
/* 只分析主要函数 */
static void
exec_simple_query(const char *query_string)
{
    /* 为什么需要，比如insert失败了怎么办，要回滚；read的话可能影响别的transaction.
     * Mctx变成CurTransactionContext。如果xact_started了的话do nothing.
     * 也就是说：如果这个query_string是一个list，那么全是在一个xaction中？
     */
    start_xact_command();

    /*
     * MessageContext会在PostgresMain()的loop中每次处理query前面会reset.
     * 似乎主要用于parse和plan两个阶段.
     */
    oldcontext = MemoryContextSwitchTo(MessageContext);
    parsetree_list = pg_parse_query(query_string);

    /* 因为分成多个query，比如：insert into t1 values(1); select * from t1; */
    foreach(parsetree_item, parsetree_list)
    {
        Node       *parsetree = (Node *) lfirst(parsetree_item);

        /*
         * pg采取埋点检测interrupt，因此要求代码逻辑不能hang太长时间，可以retry方式。
         * HAWQ的libhdfs似乎在hdfs没有启动的时候wait for最多40s，造成cancel不了query
         * 那么长时间，理论上这就是一个典型的负面例子或者bug。思考：preemption, spinlock.
         */
        CHECK_FOR_INTERRUPTS();

        querytree_list = pg_analyze_and_rewrite(parsetree, query_string,
                                                NULL, 0);

        /* 问题： 为什么是一个list，是否一定是只有一个entry的list? */
        plantree_list = pg_plan_queries(querytree_list,
                                        CURSOR_OPT_PARALLEL_OK, NULL);

        /* In case it is cancelled during planning. */
        CHECK_FOR_INTERRUPTS();
        /* portal是很重要数据结构，见后 */
        /* 创建和初始化PortalMemory Context */
        portal = CreatePortal("", true, true);
        /* commandTag: e.g. "SELECT" see process title. */
        PortalDefineQuery(portal,
                          NULL,
                          query_string,
                          commandTag,
                          plantree_list,
                          NULL);
        PortalStart(portal, NULL, 0, InvalidSnapshot);
        /* 
         * 1. 有个全局变量：CommandDest whereToSendOutput = DestDebug;
         * 2. psql的时候select/insert, gdb显示的是DestRemote, 代码解释是 results sent to frontend process
         * 3. callback functions in receiver.
         * 4. e.g. After execution, it will call receiver->receiveSlot() to send back the result to psql.
         */
        receiver = CreateDestReceiver(dest);
        /* Switch回xaction的memory context. */
        MemoryContextSwitchTo(oldcontext);
        /* 
         * FETCH_ALL=LONG_MAX，如果哪天那个query真的cnt有那么大呢？（尤其是32bit的机器），这其实应该算是一
         * 个bug。completionTag在PortalRun完成后设置，比如："SELECT 5"：针对有5 rows的"select * from t1"
         */
        (void) PortalRun(portal,
                         FETCH_ALL,
                         isTopLevel,
                         receiver,
                         receiver,
                         completionTag);

        /* 问题：为什么不能放在PortalDrop()中？*/
        (*receiver->rDestroy) (receiver);

        PortalDrop(portal, false);
        finish_xact_command);
    }
    /*
     * 问题：是不是这个时候psql一定能显示结果，不一定，因为printtup()->pq_endmessage()不一定会flush，
     * 但是PostgresMain() loop中的ReadyForQuery()->pq_flush()一定会flush output到psql的client.
     * 和hawq中老版本不一样，libpq也通过static PQcommMethods可以实现各种通信方式.
     */
}

typedef struct PortalData
{
    /* Bookkeeping data */
    const char *name;           /* portal's name */
    const char *prepStmtName;   /* source prepared statement (NULL if none) */
    MemoryContext heap;         /* subsidiary memory for portal */
    ResourceOwner resowner;     /* resources owned by portal */
    /* 典型的是 PortalCleanup */
    void        (*cleanup) (Portal portal);     /* cleanup hook */

    /*
     * State data for remembering which subtransaction(s) the portal was
     * created or used in.  If the portal is held over from a previous
     * transaction, both subxids are InvalidSubTransactionId.  Otherwise,
     * createSubid is the creating subxact and activeSubid is the last subxact
     * in which we ran the portal.
     */
    SubTransactionId createSubid;       /* the creating subxact */
    SubTransactionId activeSubid;       /* the last subxact with activity */

    /* The query or queries the portal will execute */
    const char *sourceText;     /* text of query (as of 8.4, never NULL) */
    const char *commandTag;     /* command tag for original query */
    List       *stmts;          /* PlannedStmts and/or utility statements */
    /* 在PortalDefineQuery()设置后还有什么用处？*/
    CachedPlan *cplan;          /* CachedPlan, if stmts are from one */

    /* e.g. for exec_simple_query()：params是NULL, 因为有的query有参数 */
    ParamListInfo portalParams; /* params to pass to query */

    /* Features/options */
    /* 针对不同策略，处理策略方式不同。问题：分类的原则是？简单的SELECT就是一类 PORTAL_ONE_SELECT。 */
    PortalStrategy strategy;    /* see above */
    int         cursorOptions;  /* DECLARE CURSOR option bits */

    /* Status data */
    PortalStatus status;        /* see above */
    /* 例如：PinPortal(portal) */
    bool        portalPinned;   /* a pinned portal can't be dropped */

    /* If not NULL, Executor is active; call ExecutorEnd eventually: */
    /* 关键数据结构，executor的参数入口，ProcessQuery()中CreateQueryDesc() + ExecutorStart() + ExecutorRun() + ExecutorFinish() + ExecutorEnd(). */
    /* 问题：1) 为什么不用portal作为参数；2）为什么Finish()和End()区别？
    QueryDesc  *queryDesc;      /* info needed for executor invocation */

    /* If portal returns tuples, this is their tupdesc: */
    TupleDesc   tupDesc;        /* descriptor for result tuples */
    /* and these are the format codes to use for the columns: */
    int16      *formats;        /* a format code for each column */

    /*
     * Where we store tuples for a held cursor or a PORTAL_ONE_RETURNING or
     * PORTAL_UTIL_SELECT query.  (A cursor held past the end of its
     * transaction no longer has any active executor state.)
     */
    Tuplestorestate *holdStore; /* store for holdable cursors */
    MemoryContext holdContext;  /* memory containing holdStore */

    /*
     * Snapshot under which tuples in the holdStore were read.  We must keep a
     * reference to this snapshot if there is any possibility that the tuples
     * contain TOAST references, because releasing the snapshot could allow
     * recently-dead rows to be vacuumed away, along with any toast data
     * belonging to them.  In the case of a held cursor, we avoid needing to
     * keep such a snapshot by forcibly detoasting the data.
     */
    Snapshot    holdSnapshot;   /* registered snapshot, or NULL if none */

    /*
     * atStart, atEnd and portalPos indicate the current cursor position.
     * portalPos is zero before the first row, N after fetching N'th row of
     * query.  After we run off the end, portalPos = # of rows in query, and
     * atEnd is true.  Note that atStart implies portalPos == 0, but not the
     * reverse: we might have backed up only as far as the first row, not to
     * the start.  Also note that various code inspects atStart and atEnd, but
     * only the portal movement routines should touch portalPos.
     */
    bool        atStart;
    bool        atEnd;
    uint64      portalPos;

    /* Presentation data, primarily used by the pg_cursors system view */
    TimestampTz creation_time;  /* time at which this portal was defined */
    bool        visible;        /* include this portal in pg_cursors? */
}   PortalData;

typedef struct PortalData *Portal;

```C
/* ----------------
 *      query descriptor:
 *
 *  a QueryDesc encapsulates everything that the executor
 *  needs to execute the query.
 *
 *  For the convenience of SQL-language functions, we also support QueryDescs
 *  containing utility statements; these must not be passed to the executor
 *  however.
 * ---------------------
 */
typedef struct QueryDesc
{
    /* These fields are provided by CreateQueryDesc */
    CmdType     operation;      /* CMD_SELECT, CMD_UPDATE, etc. */
    PlannedStmt *plannedstmt;   /* planner's output, or null if utility */
    /* 比如：qd->utilitystmt = plannedstmt->utilityStmt; in case DECLARE CURSOR.
     * 如果是utility，plan放在plan的utilityStmt?
     */
    Node       *utilitystmt;    /* utility statement, or null */
    const char *sourceText;     /* source text of the query */
    Snapshot    snapshot;       /* snapshot to use for query */
    Snapshot    crosscheck_snapshot;    /* crosscheck for RI update/delete */
    DestReceiver *dest;         /* the destination for tuple output */
    ParamListInfo params;       /* param values being passed in */
    int         instrument_options;     /* OR of InstrumentOption flags */

    /* These fields are set by ExecutorStart */
    TupleDesc   tupDesc;        /* descriptor for result tuples */
    EState     *estate;         /* executor's query-wide state */
    PlanState  *planstate;      /* tree of per-plan-node state */

    /* This is always set NULL by the core system, but plugins can change it */
    struct Instrumentation *totaltime;  /* total time spent in ExecutorRun */
} QueryDesc;

/* 
 * 注意最后一行是因为 DR_printtup的第一个member是 "DestReceiver pub"，
 * 为什么要这么写不好维护的代码？可以给社区提一个patch.
 */
DestReceiver *
printtup_create_DR(CommandDest dest)
{
    DR_printtup *self = (DR_printtup *) palloc0(sizeof(DR_printtup));

    self->pub.receiveSlot = printtup;   /* might get changed later */
    self->pub.rStartup = printtup_startup;
    self->pub.rShutdown = printtup_shutdown;
    self->pub.rDestroy = printtup_destroy;
    self->pub.mydest = dest;

    /*
     * Send T message automatically if DestRemote, but not if
     * DestRemoteExecute
     */
    self->sendDescrip = (dest == DestRemote);

    self->attrinfo = NULL;
    self->nattrs = 0;
    self->myinfo = NULL;
    self->tmpcontext = NULL;

    return (DestReceiver *) self;
}

/*
 * PORTAL_ONE_SELECT: the portal contains one single SELECT query.  We run
 * the Executor incrementally as results are demanded.  This strategy also
 * supports holdable cursors (the Executor results can be dumped into a
 * tuplestore for access after transaction completion).
 * 非CTE
 * PORTAL_ONE_RETURNING: the portal contains a single INSERT/UPDATE/DELETE
 * query with a RETURNING clause (plus possibly auxiliary queries added by
 * rule rewriting).  On first execution, we run the portal to completion
 * and dump the primary query's results into the portal tuplestore; the
 * results are then returned to the client as demanded.  (We can't support
 * suspension of the query partway through, because the AFTER TRIGGER code
 * can't cope, and also because we don't want to risk failing to execute
 * all the auxiliary queries.)
 * /
/* 这个函数决定strategy的函数*/
/* 问题：how set T_Query, T_PlannedStmt, T_InsertStmt? */
PortalStrategy
ChoosePortalStrategy(List *stmts);

```

总结：
- transaction (Could set)
```
postgres=# show transaction_isolation;
 transaction_isolation
-----------------------
 read committed
(1 row)
```
- 典型的memory context
```C
/*
 * CurrentMemoryContext
 *      Default memory context for allocations.
 */
MemoryContext CurrentMemoryContext = NULL;

/*
 * Standard top-level contexts. For a description of the purpose of each
 * of these contexts, refer to src/backend/utils/mmgr/README
 */
MemoryContext TopMemoryContext = NULL;
MemoryContext ErrorContext = NULL;
MemoryContext PostmasterContext = NULL;
MemoryContext CacheMemoryContext = NULL;
MemoryContext MessageContext = NULL;
MemoryContext TopTransactionContext = NULL;
MemoryContext CurTransactionContext = NULL;

/* This is a transient link to the active portal's memory context: */
MemoryContext PortalContext = NULL;
```
```C
void
PortalStart(Portal portal, ParamListInfo params,
            int eflags, Snapshot snapshot)
{
        portal->strategy = ChoosePortalStrategy(portal->stmts);

        /* For PORTAL_ONE_SELECT */
                queryDesc = CreateQueryDesc((PlannedStmt *) linitial(portal->stmts),
                                            portal->sourceText,
                                            GetActiveSnapshot(),
                                            InvalidSnapshot,
                                            None_Receiver,
                                            params,
                                            0);
        /* */
                ExecutorStart(queryDesc, myeflags);
}
```
```C
typedef struct QueryDesc {
    ......
    /* These fields are set by ExecutorStart */
    TupleDesc   tupDesc;        /* descriptor for result tuples */
    /* 整个query级别的状态 */
    EState     *estate;         /* executor's query-wide state */
    /* plan tree的每个node的状态 */
    PlanState  *planstate;      /* tree of per-plan-node state */
    ......
}

typedef struct PlanState
{
    ......
    struct PlanState *lefttree; /* input plan tree(s) */
    struct PlanState *righttree;
    ......
}

typedef struct JoinState
{
    PlanState   ps;
    JoinType    jointype;
    List       *joinqual;       /* JOIN quals (in addition to ps.qual) */
} JoinState;

typedef struct HashJoinState
{
    JoinState   js;             /* its first field is NodeTag */
    List       *hashclauses;    /* list of ExprState nodes */
    List       *hj_OuterHashKeys;       /* list of ExprState nodes */
    ......
}
/* PlanState是一个基类 */

```
**pg允许通过hook实现不同的执行器**
```C
void
ExecutorStart(QueryDesc *queryDesc, int eflags)
{
    if (ExecutorStart_hook)
        (*ExecutorStart_hook) (queryDesc, eflags);
    else
        standard_ExecutorStart(queryDesc, eflags);
}
```
**典型的executor函数**

ExecutorStart(QueryDesc *queryDesc, int eflags)

    ->standard_ExecutorStart()

        ->Populate Estate

        ->InitPlan

            ->ExecInitNode
        
ExecutorRun(QueryDesc *queryDesc, ScanDirection direction, uint64 count)

    ->standard_ExecutorRun()

        ->ExecutePlan()

            ->Loop { ExecProcNode(): 得到一个tuple，returns TupleTableSlot* }

ExecutorEnd(QueryDesc *queryDesc)

    ->standard_ExecutorEnd()

        ->ExecEndPlan()

            ->Recursively calls ExecEndNode()


***



==问题==
- Later总结：Transaction and explicit/implicit Locking (and how xact loop with start_xact_command())?
- 为什么会有PushActiveSnapshot(GetTransactionSnapshot());
- 一次只有一个tuple (good vs bad), simple code logic, low latency, bad performance.
- TODO: bug fixes, a simple SIMD?
- external table.
- 
Difference:
T_Query = 700,
T_PlannedStmt,   -> "Declare cursor"?
T_InsertStmt,
T_DeleteStmt,
T_UpdateStmt,
T_SelectStmt
PlannedStmt VS T_PlannedStmt?
- resource owner VS plan->refcount
- Check cursor how?
- How executre without portal?
- column vs row vs external table vs SIMD

