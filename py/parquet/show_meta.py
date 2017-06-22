# show metadata and schema of a parquet file
import pyarrow.parquet as pq
import sys

input_file = sys.argv[1]

f = pq.ParquetFile(input_file)
print "==input file=="
print f.metadata
print f.schema

sys.exit(0)
