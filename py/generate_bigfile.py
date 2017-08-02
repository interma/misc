import sys

file_name = sys.argv[1]
count = int(sys.argv[2])
content = "12345678"*128*1024 #1Mb
f = open(file_name, 'w')
f.write(content*count)
f.close()

