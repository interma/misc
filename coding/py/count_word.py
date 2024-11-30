import re
import sys

d = {}
for line in open(sys.argv[1]):
    l = re.findall("[a-zA-Z0-9_]+", line.strip())
    for word in l:
        word = word.strip()
        if word == "":
            continue
        if not d.has_key(word):
            d[word] = 0
        d[word] += 1

for word,count in d.items():
    print word+"\t"+str(count)
