#download CSVs form: http://www.sparsity-technologies.com/downloads/WikipediaDump/ 
#usage: combine.py articles_ids.csv article_category.csv articles_body.csv > output.csv
import sys

def clean_line(line):
	line = line.strip()
	if line == "" or line.startswith('"id",'):
		return "",""
	items = line.split('","')
	if len(items) != 2:
		#print "SKIP:"+line
		return "",""
	return items[0].strip('"'), items[1].strip('"')

# id -> title dict
title_dict = {}
for line in open(sys.argv[1]):
	a,b = clean_line(line)
	if a == "":
		continue
	title_dict[a] = b
	#print a,b

# id -> category_id dict
cate_dict = {}
for line in open(sys.argv[2]):
	a,b = clean_line(line)
	if a == "":
		continue
	cate_dict[a] = b
	#print a,b

# loop id -> body lines
print 'id,category_id,title,body'
for line in open(sys.argv[3]):
	id_,body = clean_line(line)
	if id_ == "":
		continue
	title = ""
	cate = 0
	if title_dict.has_key(id_):
		title = title_dict[id_]
	if cate_dict.has_key(id_):
		cate = cate_dict[id_]
	try:
		id_ = int(id_)
		cate = int(cate)
	except Exception as ex:
		continue

	print "%d,%d,\"%s\",\"%s\"" % (id_, cate, title, body)

