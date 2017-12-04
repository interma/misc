#ifndef __PARQUET_READER__
#define __PARQUET_READER__

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
#include "executor/spi.h"
#include "cdb/cdbparquetam.h"
#include "nodes/print.h"

TupleTableSlot *ExecParquetVScan(TableScanState *node);

#endif
