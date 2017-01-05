import tornado.ioloop
import tornado.web
import json


counter = 0

class MainHandler(tornado.web.RequestHandler):
    def get(self):
	self.write("Hello, world")

"""
request:
    { "requestId": "1", "user": "interma", "clientIp": "127.0.0.1", "context": "connect to db", "access": [ { "resource": { "database": "postgres" }, "privileges": [ "CONNECT" ] } ] }

response:
    {""requestId"":1,""access"":[{""resource"":{""database"":""template1"",""schema"":""public""},""privileges"":[""usage""],""allowed"":true}]}
"""
class RPSHandler(tornado.web.RequestHandler):
    def get(self):
	self.write("Hello, world")

    def post(self):
        global counter
        counter += 1
	self.set_header("Content-Type", "application/json")
        jreq = json.loads(self.request.body)
        req = json.dumps(jreq, sort_keys=False, indent=4, separators=(',', ': '))
        print "\033[1;42m#%d\033[1;m"%(counter),
        print '\033[0m'
        print req

        yn = raw_input("[y/n]: ")
        res = {"access":[{"allowed":True}]}
        if yn and yn.strip() == 'n':
            res = {"access":[{"allowed":False}]}
        self.write(json.dumps(res))

def make_app():
    return tornado.web.Application([
	(r"/", MainHandler),
	(r"/rps", RPSHandler),
    ])

if __name__ == "__main__":
    app = make_app()
    app.listen(8080)
    tornado.ioloop.IOLoop.current().start()
