#include "parquet_reader.h"
#include "tuple_batch.h"
#include "debug.h"

extern bool getNextRowGroup(ParquetScanDesc scan);

static TupleTableSlot *ExecParquetScanRelation(ScanState *node);
static TupleTableSlot *ParquetVScanNext(ScanState *scanState);
static void parquet_vgetnext(ParquetScanDesc scan, ScanDirection direction, TupleTableSlot *slot); 
static int ParquetRowGroupReader_ScanNextTupleBatch(TupleDesc tupDesc,
							ParquetRowGroupReader *rowGroupReader,
							int *hawqAttrToParquetColNum,
							bool *projs,
							TupleTableSlot *slot);

static TupleTableSlot *ExecVProject(TupleTableSlot *orig_slot, ProjectionInfo *projInfo, ExprDoneCond *isDone);

static TupleBatch globalTB = NULL;

TupleTableSlot *
ExecParquetVScan(TableScanState *node)
{
    ScanState *scanState = (ScanState *)node;

    if (scanState->scan_state == SCAN_INIT ||
        scanState->scan_state == SCAN_DONE)
    {
        BeginScanParquetRelation(scanState);
		ParquetScanState *pss = (ParquetScanState *)scanState;
		globalTB = createMaxTupleBatch(pss->opaque->scandesc->pqs_tupDesc->natts,
				pss->opaque->scandesc->pqs_tupDesc, pss->opaque->proj);
    }

	resetTupleBatch(globalTB);
    TupleTableSlot *slot = ExecParquetScanRelation(scanState);
	//print_slot(slot);
    //TupleTableSlot *slot = ParquetVScanNext(scanState);


    if (!TupIsNull(slot))
    {
        //Gpmon_M_Incr_Rows_Out(GpmonPktFromTableScanState(node));
        //CheckSendPlanStateGpmonPkt(&scanState->ps);
    }

    else if (!scanState->ps.delayEagerFree)
    {
        EndScanParquetRelation(scanState);
    }

    return slot;
}

TupleTableSlot *
ExecParquetScanRelation(ScanState *node)
{
    ExprContext *econtext;
    List       *qual;
    ProjectionInfo *projInfo;

    /*
     * Fetch data from node
     */
    qual = node->ps.qual;
    projInfo = node->ps.ps_ProjInfo;

    /*
     * If we have neither a qual to check nor a projection to do, just skip
     * all the overhead and return the raw scan tuple.
     */
    if (!qual && !projInfo)
        return ParquetVScanNext(node);

    /*
     * Reset per-tuple memory context to free any expression evaluation
     * storage allocated in the previous tuple cycle.
     */
    econtext = node->ps.ps_ExprContext;
    ResetExprContext(econtext);

    /*
     * get a tuple from the access method loop until we obtain a tuple which
     * passes the qualification.
     */
    for (;;)
    {
        TupleTableSlot *slot;

        CHECK_FOR_INTERRUPTS();

        slot = ParquetVScanNext(node);

        /*
         * if the slot returned by the accessMtd contains NULL, then it means
         * there is nothing more to scan so we just return an empty slot,
         * being careful to use the projection result slot so it has correct
         * tupleDesc.
         */
        if (TupIsNull(slot))
        {
            if (projInfo)
                return ExecClearTuple(projInfo->pi_slot);
            else
                return slot;
        }

        /*
         * place the current tuple into the expr context
         */
        econtext->ecxt_scantuple = slot;

        /*
         * check that the current tuple satisfies the qual-clause
         *
         * check for non-nil qual here to avoid a function call to ExecQual()
         * when the qual is nil ... saves only a few cycles, but they add up
         * ...
         */
        if (!qual || ExecQual(qual, econtext, false))
        {
            /*
             * Found a satisfactory scan tuple.
             */
            if (projInfo)
            {
                /*
                 * Form a projection tuple, store it in the result tuple slot
                 * and return it.
                 */
                return ExecVProject(slot, projInfo, NULL);
            }
            else
            {
                /*
                 * Here, we aren't projecting, so just return scan tuple.
                 */
                return slot;
            }
        }

        /*
         * Tuple fails qual, so free per-tuple memory and try again.
         */
        ResetExprContext(econtext);
    }
}

TupleTableSlot *
ParquetVScanNext(ScanState *scanState)
{
	Assert(IsA(scanState, TableScanState) || IsA(scanState, DynamicTableScanState));
	ParquetScanState *node = (ParquetScanState *)scanState;
	Assert(node->opaque != NULL && node->opaque->scandesc != NULL);

	parquet_vgetnext(node->opaque->scandesc, node->ss.ps.state->es_direction, node->ss.ss_ScanTupleSlot);
	return node->ss.ss_ScanTupleSlot;
}

void 
parquet_vgetnext(ParquetScanDesc scan, ScanDirection direction, TupleTableSlot *slot) 
{

	//AOTupleId aoTupleId;
	Assert(ScanDirectionIsForward(direction));

	for(;;)
	{
		if(scan->bufferDone)
		{
			/*
			 * Get the next row group. We call this function until we
			 * successfully get a block to process, or finished reading
			 * all the data (all 'segment' files) for this relation.
			 */
			while(!getNextRowGroup(scan))
			{
				/* have we read all this relation's data. done! */
				if(scan->pqs_done_all_splits)
				{
					ExecClearTuple(slot);
					return /*NULL*/;
				}
			}
/*
			StringInfo message = ParquetScanDescToString(scan);
			elog(LOG, "\n%s", message->data);
*/
			scan->bufferDone = false;
		}

		//dumpRowGroup(scan);
		int row_num  = ParquetRowGroupReader_ScanNextTupleBatch(
								scan->pqs_tupDesc,
								&scan->rowGroupReader,
								scan->hawqAttrToParquetColChunks,
								scan->proj,
								slot);
		if(row_num > 0)
		{
			//TupleBatch tb = (TupleBatch)slot->PRIVATE_tts_data;
			//tb->projs = scan->proj;

//			elog(LOG, "ParquetRowGroupReader_ScanNextTupleBatch: row_num:%d", row_num);
/*
			int segno = ((FileSplitNode *)list_nth(scan->splits, scan->pqs_splits_processed - 1))->segno;
			AOTupleIdInit_Init(&aoTupleId);
			AOTupleIdInit_segmentFileNum(&aoTupleId, segno);

			scan->cur_seg_row++;
			AOTupleIdInit_rowNum(&aoTupleId, scan->cur_seg_row);

			scan->cdb_fake_ctid = *((ItemPointer)&aoTupleId);

			slot_set_ctid(slot, &(scan->cdb_fake_ctid));
*/
			// set tuple batch to TupleTableSlot


			return;
		}

		/* no more items in the row group, get new buffer */
		scan->bufferDone = true;
	}
}

/*
 * Get next tuple batch from current row group into slot.
 *
 * Return false if current row group has no tuple left, true otherwise.
 */
int
ParquetRowGroupReader_ScanNextTupleBatch(
	TupleDesc 				tupDesc,
	ParquetRowGroupReader	*rowGroupReader,
	int						*hawqAttrToParquetColNum,
	bool 					*projs,
	TupleTableSlot 			*slot)
{
	Assert(slot);

	if (rowGroupReader->rowRead >= rowGroupReader->rowCount)
	{
		ParquetRowGroupReader_FinishedScanRowGroup(rowGroupReader);
		return false;
	}

	/*
	 * get the next item (tuple) from the row group
	 */
	int nrow = 0;
	int ncol = slot->tts_tupleDescriptor->natts;
	if (rowGroupReader->rowRead + BATCH_SIZE > rowGroupReader->rowCount) {
		nrow = rowGroupReader->rowCount-rowGroupReader->rowRead;
		rowGroupReader->rowRead = rowGroupReader->rowCount;
	}
	else {
		nrow = BATCH_SIZE;
		rowGroupReader->rowRead += BATCH_SIZE;
	}
	
	//TupleBatch tb = createTupleBatch(nrow, ncol);
	TupleBatch tb = globalTB;
	tb->nrow = nrow;
	tb->ncol = ncol;
	slot->PRIVATE_tts_data = (void *)tb;

	int colReaderIndex = 0;
	for(int i = 0; i < tb->ncol; i++)
	{
		TupleColumnData *tc = getTupleBatchColumn(tb, i);
		Datum 	*values = tc->values;
		bool	*nulls = tc->nulls; 
		
		if(projs[i] == false)
		{
			nulls[i] = true;
			continue;
		}

		ParquetColumnReader *nextReader =
			&rowGroupReader->columnReaders[colReaderIndex];
		int hawqTypeID = tupDesc->attrs[i]->atttypid;
		
		for(int j=0;j<tb->nrow;j++)
		{
		 
			if(hawqAttrToParquetColNum[i] == 1)
			{
				ParquetColumnReader_readValue(nextReader, &values[j], &nulls[j], hawqTypeID);
				//elog(WARNING, "value:%d null:%d", DatumGetInt32(values[j]), nulls[j]);
			}
			else
			{
				/*
				 * Because there are some memory reused inside the whole column reader, so need
				 * to switch the context from PerTupleContext to rowgroup->context
				 */
				MemoryContext oldContext = MemoryContextSwitchTo(rowGroupReader->memoryContext);

				switch(hawqTypeID)
				{
					case HAWQ_TYPE_POINT:
						ParquetColumnReader_readPoint(nextReader, &values[j], &nulls[j]);
						break;
					case HAWQ_TYPE_PATH:
						ParquetColumnReader_readPATH(nextReader, &values[j], &nulls[j]);
						break;
					case HAWQ_TYPE_LSEG:
						ParquetColumnReader_readLSEG(nextReader, &values[j], &nulls[j]);
						break;
					case HAWQ_TYPE_BOX:
						ParquetColumnReader_readBOX(nextReader, &values[j], &nulls[j]);
						break;
					case HAWQ_TYPE_CIRCLE:
						ParquetColumnReader_readCIRCLE(nextReader, &values[j], &nulls[j]);
						break;
					case HAWQ_TYPE_POLYGON:
						ParquetColumnReader_readPOLYGON(nextReader, &values[j], &nulls[j]);
						break;
					default:
						/* TODO array type */
						/* TODO UDT */
						Insist(false);
						break;
				}

				MemoryContextSwitchTo(oldContext);
			}
		}

		colReaderIndex += hawqAttrToParquetColNum[i];
	}

	/*construct tuple, and return back*/
	TupSetVirtualTupleNValid(slot, ncol);
	return tb->nrow;
}

TupleTableSlot *
ExecVProject(TupleTableSlot *orig_slot, ProjectionInfo *projInfo, ExprDoneCond *isDone)
{
	TupleTableSlot *slot;
	
	Assert(projInfo != NULL);

	slot = projInfo->pi_slot;

	ExecClearTuple(slot);

	TupleBatch tb = (TupleBatch)orig_slot->PRIVATE_tts_data; 
	slot->PRIVATE_tts_data = (void *)tb; 

	if (projInfo->pi_isVarList)
	{
		/* simple Var list: this always succeeds with one result row */
		if (isDone)
			*isDone = ExprSingleResult;

		//ExecVariableList(projInfo, slot_get_values(slot), slot_get_isnull(slot));
		setTupleBatchNValid(tb, list_length(projInfo->pi_targetlist));

		int *varNumbers = projInfo->pi_varNumbers;
		for (int i = list_length(projInfo->pi_targetlist) - 1; i >= 0; i--)
		{
			setTupleBatchProjColumn(tb, i, varNumbers[i]);
		}

		ExecStoreVirtualTuple(slot);
	}
	else
	{
		elog(ERROR, "never in vagg");
		/*
		if (ExecTargetList(projInfo->pi_targetlist,
						   projInfo->pi_exprContext,
						   slot_get_values(slot),
						   slot_get_isnull(slot),
						   (ExprDoneCond *) projInfo->pi_itemIsDone,
						   isDone))
		ExecStoreVirtualTuple(slot);
		*/
	}

	return slot;
}

