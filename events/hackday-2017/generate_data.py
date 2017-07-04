import sys
import random
import time

#table
#create table orders (id bigint, name varchar(32), price int, ts int) with (appendonly=true, orientation=parquet, rowgroupsize=8388608, pagesize=1048576);


line_num = int(sys.argv[1])
begin_id = int(sys.argv[2])

for i in range(0,line_num):
    _id = begin_id+i
    _name = "order_name XXX"
    _price = random.randint(1, 100)
    _ts = time.time()

    print "%d|%s|%d|%d" % (_id,_name,_price,_ts)

sys.exit(0)
