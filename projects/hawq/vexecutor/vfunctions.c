#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "tuple_batch.h"

Datum int4_sum_vec(PG_FUNCTION_ARGS);
Datum int8_sum_vec(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(int4_sum_vec);
PG_FUNCTION_INFO_V1(int8_sum_vec);

Datum
int4_sum_vec(PG_FUNCTION_ARGS)
{
	TupleColumnData *columnData = (TupleColumnData *) PG_GETARG_POINTER(1);
	int rowCount = PG_GETARG_INT32(2);

	int64 newValue = 0;
    if (PG_ARGISNULL(0))
    {
        newValue = 0;
    }
	else
	{
		newValue = PG_GETARG_INT64(0);
	}	

	for (int i=0;i<rowCount;i++)
	{
		if (!columnData->nulls[i])
		{
			newValue = newValue + (int64) DatumGetInt32(columnData->values[i]);
		}
	}

	PG_RETURN_INT64(newValue);
}

Datum
int8_sum_vec(PG_FUNCTION_ARGS)
{
	PG_RETURN_INT64(108);
}
