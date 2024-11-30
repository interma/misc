#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import json
from googleapiclient.discovery import build

def search(jar_list, api_key):
    result = []
    service = build("customsearch", "v1", developerKey=api_key)

    for jar in jar_list:
        res = service.cse().list(
            q=jar,
            cx='001132580745589424302:jbscnf14_dw',
            lr='lang_en'
        ).execute()
        if not res or not res['items']:
            print "search google[%s] failed: %s" % (jar, res)
            continue
        res = res['items']
        # only read top-5 records
        found = False
        for i in range(0,5):
            title = res[i]['title']
            url = res[i]['link']
            # print title, url
            if title.find('Maven Repository') >= 0:
                result.append({"jar":jar,"mvn_url":url})
                found = True
                break
        if not found:
            result.append({"jar":jar,"mvn_url":""})

    return result


def print_search_result(result):
    for item in result:
        print "%s\t%s" % (item['jar'], item['mvn_url'])

def get_jar_name(input_file):
    jar_list = []
    for line in open(input_file):
        items = line.split('/')
        nf = len(items)
        if nf <= 0:
            continue
        jar = items[nf-1].strip()
        if jar.find('jar') < 0:
            continue
        jar_list.append(jar)
    return jar_list


if __name__ == '__main__':
    input_file = sys.argv[1]
    api_key= sys.argv[2]

    jar_list = get_jar_name(input_file)
    #print jar_list
    result = search(jar_list, api_key)
    print_search_result(result)
