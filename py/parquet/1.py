# coding: utf-8

'''
CREATE TABLE user_pq (
id bigint NOT NULL DEFAULT 0,
name varchar,
age int
) with (appendonly=true, orientation=parquet);
insert into user_pq(id,name,age) values (generate_series(1,1000), 'interma', trunc(random() * 99 + 1));
hawq extract -d postgres -o user_pq.yml test_sa.user_pq
'''

import pyarrow.parquet as pq
t = pq.read_table('user_pq.parquet')
t = pq.read_table('./user_pq.parquet')
t = pq.read_table('./user_pq.parquetxx')
t = pq.read_table('./user_pq.parquet')
t = pq.read_table('./user_pq.parquet')
t
pq.ParquetFile('user_pq.parquet')
f = pq.ParquetFile('user_pq.parquet')
f
f.metadarta
f.metadata
f.schema
f..read_row_group(0)
f.read_row_group(0)
f.read_row_group(1)
f.read_row_group(0)
help
help()
get_ipython().show_usage()
get_ipython().run_cell_magic(u'writefile', u'test.py', u'')
writefile
get_ipython().magic(u'save 1.py')
get_ipython().magic(u'save')
get_ipython().magic(u'save')
get_ipython().magic(u'save test.py')
get_ipython().magic(u"save '1.py'")
get_ipython().magic(u'pinfo save')
get_ipython().magic(u'save 1.py 1-29')
