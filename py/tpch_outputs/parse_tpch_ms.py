# get the mean second of Jon's tpch outputs
import sys

def process_line(line):
    # 101|tpch.01|4|00:02:39.159483
    hms = line.strip().split('|')[3]
    # 00:00:30.30346
    hms = hms.split('.')[0]
    h = int(hms.split(':')[0])
    m = int(hms.split(':')[1])
    s = int(hms.split(':')[2])
    return h*3600+m*60+s

sum22 = []
# init for 22 query
for i in range(0,22):
    sum22.append(0)

# process each file
fcnt = 0
for f in sys.stdin:
    fd = open(f.strip())
    fcnt +=1
    print "processing "+f.strip()
    qcnt = 0
    for line in fd.readlines():
        sec = process_line(line)
        sum22[qcnt] += sec
        qcnt += 1

# output
for i in range(0,22):
    print sum22[i]/fcnt


sys.exit(0)
