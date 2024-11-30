#ifndef __TUPLE_BATCH_H__
#define __TUPLE_BATCH_H__

#include "executor/tuptable.h"
#include "executor/execHHashagg.h"

#define BATCH_SIZE 1024

typedef struct TupleColumnData {
	Datum	*values;
	bool	*nulls;
} TupleColumnData;

//hashagg group linklist header
typedef struct GroupData {
	HashAggEntry *entry;	// pointer to agg_hash_entry
	int idx; 				// pointer to idx_list
} GroupData;

typedef struct TupleBatchData {
	// for original scan tuples
	int				nrow;
	int				ncol;
	bool			*projs;
	
	// for projection
	int				nvalid;
	int				*vprojs;

	// original scan column data
	TupleColumnData	**columnDataArray;

	// row tuple 
	TupleDesc		tupDesc;
	TupleTableSlot 	*rowSlot;
	int				rowIdx;

	// for hash agg
	GroupData 	group_header[BATCH_SIZE];	//group linklist header
	int 		group_idx;					//current group header index
	int 		group_cnt;					//group header count

	//linklist elements
	//each item's index is the index to columnData
	//each item's value is the next pointer, -1 is the end of a linklist
	int 		idx_list[BATCH_SIZE];
} TupleBatchData, *TupleBatch;

TupleBatch createTupleBatch(int nrow, int ncol, TupleDesc tupdesc, bool *projs);
void destroyTupleBatch(TupleBatch tb);
TupleColumnData *getTupleBatchColumn(TupleBatch tb, int colIdx);
void setTupleBatchNValid(TupleBatch tb, int ncol);
void setTupleBatchProjColumn(TupleBatch tb, int colIdx, int value);
TupleBatch createMaxTupleBatch(int ncol, TupleDesc tupdesc, bool *projs);

TupleTableSlot *getNextRowFromTupleBatch(TupleBatch tb, TupleDesc tupdesc);
void assignNextRowFromTupleBatch(TupleBatch tb, TupleTableSlot *slot);
void resetTupleBatch(TupleBatch tb);


#endif
