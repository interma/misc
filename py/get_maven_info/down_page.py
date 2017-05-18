#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import requests

DATA_DIR="./data"

def download(input_file):
    global DATA_DIR
    for line in open(input_file):
        items = line.split('\t')
        if len(items) != 2:
            print "line formation error: %s" % (line)
            continue
        jar = items[0].strip()
        mvn_url = items[1].strip()
        if not mvn_url:
            continue
        print mvn_url
        r = requests.get(mvn_url)
        if r.status_code != 200:
            print "visit [%s] failed" % (mvn_url)
            continue
        out_file = DATA_DIR+"/"+jar+".html"
        with open(out_file, "w") as fp:
            fp.write(r.text.encode('utf-8'))
        time.sleep(2)


if __name__ == '__main__':
    input_file = sys.argv[1]
    download(input_file)
