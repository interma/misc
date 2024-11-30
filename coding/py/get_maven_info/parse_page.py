#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import bs4 #BeautifulSoup4

DATA_DIR="./data"

def parse_by_bs4(data):
    soup = bs4.BeautifulSoup(data, "html.parser")

    #intro
    path = "div.im-description"
    items = soup.select(path)
    intro = items[0].text.strip()
    intro = intro.replace("\n", " ")

    #license
    path = "span.lic"
    items = soup.select(path)
    license = items[0].text.strip()

    #homepage
    path = "table.grid td > a"
    items = soup.select(path)
    #print items
    homepage = items[2]["href"].strip()

    #download page
    path = "a.vbtn"
    items = soup.select(path)
    download = items[0]["href"].strip()

    return intro,license,homepage,download

def get_version(jar):
    beg = jar.rfind('-')
    end = jar.rfind('.')
    if beg < 0 or end < 0 or beg > end:
        return ''
    return jar[beg+1:end]

def parse(input_file):
    global DATA_DIR
    result = []
    i = 0
    for line in open(input_file):
        items = line.split('\t')
        if len(items) != 2:
            print "line formation error: %s" % (line)
            continue
        jar = items[0].strip()
        mvn_url = items[1].strip()
        version = get_version(jar)
        if not mvn_url:
            result.append({"jar":jar,"intro":"","version":version,"license":"","home":"","down":""})
            continue
        #parse mvn_url and parse info
        in_file = DATA_DIR+"/"+jar+".html"
        with open(in_file) as fp:
            data = fp.read()
        intro,license,home,down = parse_by_bs4(data)
        result.append({"jar":jar,"intro":intro,"version":version,"license":license,"home":home,"down":down})
        i += 1
        if i > 10:
            pass
            #break
    return result

def print_result(result):
    for item in result:
        print "%s\t%s\t%s\t%s\t%s\t%s" % (item['jar'], item['version'], item['license'], item['home'], item['down'], item['intro'])

if __name__ == '__main__':
    input_file = sys.argv[1]
    result = parse(input_file)
    print_result(result)
