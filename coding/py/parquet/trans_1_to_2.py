# transfrom parquet1 to parquet2 (wish stats field should be generated automatically)

import pyarrow.parquet as pq
import sys

input_file = sys.argv[1]
output_file = sys.argv[2]

f = pq.ParquetFile(input_file)
print "==input file=="
print f.metadata
print f.schema

num = int(f.metadata.num_row_groups)

writer = None
schema = None
for i in range(0, num):
    row_group = f.read_row_group(i)
    if not writer:
        schema = row_group.schema
        writer = pq.ParquetWriter(output_file, schema, version="2.0")
    writer.write_table(row_group)
writer.close()

f = pq.ParquetFile(output_file)
print "==output file=="
print f.metadata
print f.schema

sys.exit(0)
