#include "postgres.h"
#include "tuple_batch.h"

//#define TUPLE_BATCH_ROW_MAX_SIZE	100000

TupleBatch createMaxTupleBatch(int ncol, TupleDesc tupdesc, bool *projs)
{
	//return createTupleBatch(TUPLE_BATCH_ROW_MAX_SIZE, ncol, tupdesc, projs);
	return createTupleBatch(BATCH_SIZE, ncol, tupdesc, projs);
}

TupleBatch createTupleBatch(int nrow, int ncol, TupleDesc tupdesc, bool *projs)
{
	TupleBatch tb = (TupleBatch)palloc0(sizeof(TupleBatchData));
	tb->nrow = nrow;
	tb->ncol = ncol;
	tb->projs = projs;

	tb->columnDataArray = (TupleColumnData **)palloc0(tb->ncol * sizeof(TupleColumnData *));
	for (int i=0;i<tb->ncol;i++)
	{
		tb->columnDataArray[i] = (TupleColumnData *)palloc0(sizeof(TupleColumnData));
		tb->columnDataArray[i]->values = (Datum *)palloc0(tb->nrow * sizeof(Datum));
		tb->columnDataArray[i]->nulls = (bool *)palloc0(tb->nrow * sizeof(bool));
	}
	
	tb->vprojs = (int *)palloc0(ncol * sizeof(int));
	tb->nvalid = 0;

	tb->tupDesc = tupdesc;
	tb->rowSlot = MakeSingleTupleTableSlot(tb->tupDesc);
	tb->rowIdx = -1;

	tb->group_cnt = 0; //cnt is 0 indicates not a groupby agg
	tb->group_idx = -1;

	return tb;
}

void resetTupleBatch(TupleBatch tb)
{
	tb->nvalid = 0;
	tb->rowIdx = -1;	
}


void destroyTupleBatch(TupleBatch tb)
{
	for (int i=0;i<tb->ncol;i++)
	{
		if (tb->columnDataArray[i])
		{
			if (tb->columnDataArray[i]->values)
			{
				pfree(tb->columnDataArray[i]->values);
			}
			if (tb->columnDataArray[i]->nulls)
			{
				pfree(tb->columnDataArray[i]->nulls);
			}
			pfree(tb->columnDataArray[i]);
		}

		if (tb->vprojs)
		{
			pfree(tb->vprojs);
		}
	}

	pfree(tb);
}

TupleColumnData *getTupleBatchColumn(TupleBatch tb, int colIdx)
{
	return tb->columnDataArray[colIdx];
}

void setTupleBatchNValid(TupleBatch tb, int ncol)
{
	tb->nvalid = ncol;
}

void setTupleBatchProjColumn(TupleBatch tb, int colIdx, int value)
{
	tb->vprojs[colIdx] = value;
}

TupleTableSlot *getNextRowFromTupleBatch(TupleBatch tb, TupleDesc tupdesc)
{
	tb->rowIdx++;
	if (tb->rowIdx >= tb->nrow)
	{
		return NULL;
	}

	Datum *values = slot_get_values(tb->rowSlot);
	bool *nulls = slot_get_isnull(tb->rowSlot);	
	//ExecStoreAllNullTuple(tb->rowSlot);

	int i;
	if (tb->nvalid > 0)
	{
		// has done vectorized projection
		for (i=0;i<tb->nvalid;i++)
		{
			values[i] = tb->columnDataArray[tb->vprojs[i]-1]->values[tb->rowIdx];
			//nulls[i] = false;
		}
	
		//TupSetVirtualTupleNValid(tb->rowSlot, tb->nvalid);
	}
	else
	{
		for (i=0;i<tb->ncol;i++)
		{
			if (tb->projs[i])
			{
				values[i] = tb->columnDataArray[i]->values[tb->rowIdx];
				//nulls[i] = false;
			}
		}

		//TupSetVirtualTupleNValid(tb->rowSlot, tb->ncol);
	}

	return tb->rowSlot;	
}

void assignNextRowFromTupleBatch(TupleBatch tb, TupleTableSlot *slot)
{
	tb->rowIdx++;
	if (tb->rowIdx >= tb->nrow)
	{
		return;
	}

	Datum *values = slot_get_values(slot);
	bool *nulls = slot_get_isnull(slot);	

	for (int i=0;i<tb->ncol;i++)
	{
		if(tb->projs[i] == false) 
		{
			nulls[i] = true;
			continue;
		}
		else
		{
			nulls[i] = false;
			values[i] = tb->columnDataArray[i]->values[tb->rowIdx];
		}	
	}
	TupSetVirtualTupleNValid(slot, tb->ncol);

	return;	
}
