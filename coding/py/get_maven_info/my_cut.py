import sys


input_file = sys.argv[1]
cnum = int(sys.argv[2])
for line in open(input_file):
    items = line.split("\t")
    print items[cnum].strip()

    
