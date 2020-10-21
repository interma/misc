
def cmp_func(a, b):
    i = a.split('shard')
    j = b.split('shard')
    if len(i) == 2 and len(j) == 2:
        return int(i[1]) - int(j[1])
    else:
        return 1

l = ['shard10', 'shard1', 'shard9']
l.sort(cmp=cmp_func)
print l
