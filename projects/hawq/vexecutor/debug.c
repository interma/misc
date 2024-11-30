#include "debug.h"
#include "cdb/cdbparquetcolumn.h"
#include "parquet_reader.h"
#include "tuple_batch.h"

StringInfo ParquetScanDescToString(ParquetScanDesc scan)
{
	int ncol = scan->pqs_tupDesc->natts;
	StringInfo message = makeStringInfo();
	StringInfo tmp = makeStringInfo();	

	appendStringInfo(message, "\n============================================================\n");
	// scan state
	appendStringInfo(tmp, "pqs_filenamepath:%s pqs_splits_processed:%d "
					"pqs_need_new_split:%d pqs_done_all_splits:%d bufferDone:%d",
					scan->pqs_filenamepath,
					scan->pqs_splits_processed,
					scan->pqs_need_new_split,
					scan->pqs_done_all_splits,
					scan->bufferDone);
	appendStringInfo(message, "ScanState: %s\n", tmp->data);
	resetStringInfo(tmp);	
	
	// proj
	appendStringInfo(tmp, "[");	
	for (int i=0;i<ncol;i++)
	{
		appendStringInfo(tmp, "%d ", scan->proj[i]);
	}
	appendStringInfo(tmp, "]");

	appendStringInfo(message, "PROJECT: %s\n", tmp->data);
	resetStringInfo(tmp);	

	// row group reader 
	tmp = ParquetRowGroupReaderToString(&scan->rowGroupReader);
	appendStringInfo(message, "ROWGROUP: %s\n", tmp->data);
	resetStringInfo(tmp);

	// storage read
	tmp = ParquetStorageReadToString(&scan->storageRead);
	appendStringInfo(message, "STORAGEREAD: %s\n", tmp->data);

	// ao entry

	// splits info 

	appendStringInfo(message, "============================================================\n");
	return message;	
}

StringInfo ParquetRowGroupReaderToString(ParquetRowGroupReader *rgreader)
{
	StringInfo message = makeStringInfo();

	appendStringInfo(message, "RowGroup: rowRead/rowCount:%d/%d columnReaderCount:%d\n",
					rgreader->rowRead, rgreader->rowCount, rgreader->columnReaderCount);	
	for (int i=0;i<rgreader->columnReaderCount;i++)
	{
		ParquetColumnReader colreader = rgreader->columnReaders[i];
		appendStringInfo(message, "Column%d: %s\n", i, (ParquetColumnReaderToString(&colreader))->data);	
		appendStringInfo(message, "-------------------------------------------------------\n");	
	}	

	return message;
}

StringInfo ParquetColumnReaderToString(ParquetColumnReader *colreader)
{
	StringInfo message = makeStringInfo();
	
	// column reader
	appendStringInfo(message, "ColumnReader: dataPageNum:%d dataPageCapacity:%d dataPageProcessed:%d "
						"repetitionLevel:%d definitionLevel:%d dataLen:%d pageBufferLen:%d\n",
						colreader->dataPageNum,
						colreader->dataPageCapacity,
						colreader->dataPageProcessed,
						colreader->repetitionLevel,
						colreader->definitionLevel,
						colreader->dataLen,
						colreader->pageBufferLen);

	// column metadata
	struct ColumnChunkMetadata_4C *ccmeta = colreader->columnMetadata;
	StringInfo colmsg = makeStringInfo();
	appendStringInfo(colmsg, "ColumnMetadata: path:%s colName:%s pathInSchema:%s hawqTypeId:%d r:%d d:%d "
						"depth:%d type:%d file_offset:%lld firstDataPage:%lld valueCount:%ld "
						"totalSize:%lld totalUncompressedSize:%lld\n",
						ccmeta->path,
						ccmeta->colName,
						ccmeta->pathInSchema,
						ccmeta->hawqTypeId,
						ccmeta->r,
						ccmeta->d,
						ccmeta->depth,
						ccmeta->type,
						ccmeta->file_offset,
						ccmeta->firstDataPage,
						ccmeta->valueCount,
						ccmeta->totalSize,
						ccmeta->totalUncompressedSize);	
	appendStringInfo(message, "%s\n", colmsg->data);

	// column data pages
	for (int i=0;i<colreader->dataPageNum;i++)
	{
		struct ParquetDataPage_S *page = &colreader->dataPages[i];
		appendStringInfo(message, "\tPAGE: num_values:%d page_type:%d uncompressed_page_size:%d "
						"compressed_page_size:%d values_buffer_capacity:%d header_len:%d finalized:%d\n",
						page->header->num_values,
						page->header->page_type,
						page->header->uncompressed_page_size,
						page->header->compressed_page_size,
						page->values_buffer_capacity,
						page->header_len,
						page->finalized);		
	}
	
	return message;
}

StringInfo ParquetStorageReadToString(ParquetStorageRead *sreader)
{
	StringInfo message = makeStringInfo();
	
	appendStringInfo(message, "StorageRead: isActive:%d relationName:%s segmentFileName:%s "
						"rowGroupProcessedCount:%d rowGroupCount:%d preRead:%d\n",
						sreader->isActive,
						sreader->relationName,
						sreader->segmentFileName,
						sreader->rowGroupProcessedCount,
						sreader->rowGroupCount,
						sreader->preRead);
	
	ParquetMetadata fmeta = sreader->parquetMetadata;
	appendStringInfo(message, "ParquetMetadata: fieldCount:%d num_rows:%ld blockCount:%d "
						"hawqschemastr:%s colCount:%d schemaTreeNodeCount:%d version:%d",
						fmeta->fieldCount,
						fmeta->num_rows,
						fmeta->blockCount,
						fmeta->hawqschemastr,
						fmeta->colCount,
						fmeta->schemaTreeNodeCount,
						fmeta->version);	

	return message;
}

void dumpRowGroup(ParquetScanDesc scan)
{
	ParquetRowGroupReader *rgreader = &scan->rowGroupReader;
	Datum 	value;
	bool	null; 
	for (int i=0;i<rgreader->columnReaderCount;i++)
	{
		ParquetColumnReader *colreader = &rgreader->columnReaders[i];
		int hawqTypeID = scan->pqs_tupDesc->attrs[i]->atttypid;
		for (int j=0;j<rgreader->rowCount;j++)
		{
			ParquetColumnReader_readValue(colreader, &value, &null, hawqTypeID);
			elog(LOG, "Column:%s value:%d\n", colreader->columnMetadata->colName, DatumGetInt32(value));
		}
	}	
}

void dumpTupleBatch(TupleTableSlot *slot)
{
	TupleBatch tb = (TupleBatch)slot->PRIVATE_tts_data;

	if (tb == NULL)
	{
		elog(ERROR, "no tuple batch in slot");
	}

	elog(LOG, "dumpTupleBatch ncol:%d nrow:%d", tb->ncol, tb->nrow);
	for (int i=0;i<tb->ncol;i++)
	{
		TupleColumnData *tc = tb->columnDataArray[i];
		StringInfo colmsg = makeStringInfo();	
		for (int j=0;j<tb->nrow;j++)
		{
			appendStringInfo(colmsg, "%d ", DatumGetInt32(tc->values[j]));
		}	
		elog(LOG, "Column%d values:%s", i, colmsg->data); 
	}
}
