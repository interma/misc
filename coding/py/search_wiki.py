#!/usr/bin/env python
# paired with Ivan 2019-06-13
import getopt
import re
import sys
import time
import tornado.ioloop
import tornado.web
from datetime import datetime
from pgdb import DatabaseError
import pg

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        db = connect(user='gpadmin', password='', host='localhost', port=5432, database="demo")
        sql = "select count(*) from test_wiki;"
        curs = db.query(sql)
        row = curs.getresult()
        count = row[0]
        output = """
<html>
<body>
<h2>
There are %d wiki pages in our db
</h2>
<form action="/">
<input type="text" name="word" size="20px" value="">
<input type="radio" name="ner" value="person"> Person
<input type="radio" name="ner" value="organization"> Organization
<input type="radio" name="ner" value="all" checked> All
<input type="submit" value="search">
</form>
<hr>
        """ % (count)
        self.write(output)

        start_time = datetime.now()

        word = self.get_argument('word', None)
        ner = self.get_argument('ner', None)

        if word:
            sql2 = ""
            if ner == "person":
                sql2= "SELECT a.id, a.original_id, gptext.highlight(a.title, 'title', hs) as title, q.score FROM test_wiki a, gptext.search(TABLE(SELECT 1 SCATTER BY 1), 'demo.public.wiki2', 'title:_ner_person_%s', Null,'hl=true&hl.fl=title&rows=1&sort=score desc') q WHERE a.id = q.id::bigint ORDER BY q.score desc;" % (word)
            elif ner == "organization":
                sql2= "SELECT a.id, a.original_id, gptext.highlight(a.title, 'title', hs) as title, q.score FROM test_wiki a, gptext.search(TABLE(SELECT 1 SCATTER BY 1), 'demo.public.wiki2', 'title:_ner_organization_%s', Null,'hl=true&hl.fl=title&rows=1&sort=score desc') q WHERE a.id = q.id::bigint ORDER BY q.score desc;" % (word)
            else:
                sql2= "SELECT a.id, a.original_id, a.title, q.score FROM test_wiki a, gptext.search(TABLE(SELECT 1 SCATTER BY 1),'demo.public.test_wiki','title:%s', null, 'rows=5') q WHERE q.id::int8 = a.id ORDER BY score DESC;" % (word)
            curs = db.query(sql2)
            rows = curs.getresult()
            count = len(rows)
            end_time = datetime.now()
            interval = str(end_time-start_time)

            output = "There are %d matches (%s s):<br>" % (count, interval)
            self.write(output)
            output = "<table>"
            self.write(output)
            i = 1
            for row in rows:
                self.write("<tr>")
                self.write("<td>%d</td><td>%s</td>" % (i, row[2]))
                self.write("</tr>")
                i = i + 1
            output = "</table>"
            self.write(output)

        output = """
</body>
</html>
        """
        self.write(output)

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
    ])


def connect(user=None, password=None, host=None, port=None,
            database=None, utilityMode=False):
    '''Connect to DB using parameters in GV'''
    options = utilityMode and '-c gp_session_role=utility' or None
    try:
        #logger.debug('connecting to %s:%s %s' % (host, port, database))
        db = pg.connect(host=host, port=port, user=user,
                        passwd=password, dbname=database, opt=options)
    except:
        #logger.fatal('could not connect to %s: "%s"' %
        #             (database, str(ex).strip()))
        exit(1)

    #logger.debug('connected with %s:%s %s' % (host, port, database))
    return db

if __name__ == '__main__':
    app = make_app()
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()

