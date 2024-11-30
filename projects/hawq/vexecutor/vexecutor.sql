CREATE OR REPLACE FUNCTION int4_sum_vec(bigint, int)
RETURNS bigint
AS '$libdir/vexecutor', 'int4_sum_vec'
LANGUAGE C STRICT;
