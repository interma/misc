#ifndef __DEUBG_DUMP_H_
#define __DEUBG_DUMP_H_

#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "cdb/cdbparquetfooterprocessor.h"
#include "cdb/cdbparquetfooterserializer.h"
#include "access/parquetmetadata_c++/MetadataInterface.h"
#include "cdb/cdbparquetrowgroup.h"
#include "utils/memutils.h"
#include "utils/palloc.h"
#include "snappy-c.h"
#include "zlib.h"
#include "cdb/cdbparquetam.h"
#include "nodes/print.h"

StringInfo ParquetScanDescToString(ParquetScanDesc scan);
StringInfo ParquetRowGroupReaderToString(ParquetRowGroupReader *rgreader);
StringInfo ParquetColumnReaderToString(ParquetColumnReader *colreader);
StringInfo ParquetStorageReadToString(ParquetStorageRead *sreader);
void dumpRowGroup(ParquetScanDesc scan);
void dumpTupleBatch(TupleTableSlot *slot);

#endif
