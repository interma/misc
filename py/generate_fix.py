import random
import sys
import string

cnt = int(sys.argv[1])

def rankey():
    return random.randint(1,1000)
def ranvalue():
    return ''.join([random.choice("abcdefg ") for n in xrange(8)])

for i in range(0,cnt):
    k1=rankey()
    k2=rankey()
    v1=ranvalue()
    v2=ranvalue()
    fix_msg = "%s=%s;%s=%s" % (k1,v1,k2,v2)
    print fix_msg

