import tornado.ioloop
import tornado.web
import json
import sys

# global
counter = 0

# configure
ALL_GRANT = False
ONE_LINE = False

def read_cmdline(cmdline):
    global ALL_GRANT
    global ONE_LINE

    items = cmdline.split(',')
    for item in items:
        k,v = item.split('=')
        if k == "ALL_GRANT" and int(v) > 0:
            ALL_GRANT = True
        if k == "ONE_LINE" and int(v) > 0:
            ONE_LINE = True

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
    def make_response(self, req, ok):
        res = {}
        res["requestId"] = req["requestId"]
        res["access"] = []
        for a in req["access"]:
            resource = a["resource"]
            privileges = a["privileges"]
            allowed = False
            if ok:
                allowed = True
            res["access"].append({"resource":resource,"privileges":privileges,"allowed":allowed})
        return res

    '''
    {"allowExceptions": [], "denyExceptions": [], "denyPolicyItems": [], "description": "no description", "isAuditEnabled": true, "isEnabled": true, "name": "pxfpolicy1-3", "policyItems": [{"accesses": [{"isAllowed": true, "type": "usage-schema"}], "conditions": [], "delegateAdmin": true, "groups": null, "users": ["userpxf1"]}], "resources": {"database": {"isExcludes": false, "isRecursive": false, "values": ["hawq_feature_test_db"]}, "schema": {"isExcludes": false, "isRecursive": false, "values": ["testhawqranger_pxfhcatalogtest"]}, "table": {"isExcludes": false, "isRecursive": false, "values": ["*"]}}, "service": "hawq", "version": 3}

    '''
    def dump_request(self, jreq):
        pass

    def get(self):
	self.write("Hello, world")

    def post(self):
        global counter
        counter += 1
	self.set_header("Content-Type", "application/json")
        jreq = json.loads(self.request.body)
        if ONE_LINE:
            req = json.dumps(jreq, sort_keys=False)
        else:
            req = json.dumps(jreq, sort_keys=False, indent=4, separators=(',', ': '))
        print "\033[1;42m#%d\033[1;m"%(counter),
        print '\033[0m'
        print req
        
        allowed = True
        if not ALL_GRANT:
            yn = raw_input("[y/yd/n]: ")
            if yn and yn.strip() == 'n':
                allowed = False
            if yn and yn.strip() == 'yd':
                self.dump_request(jreq)
        jres = self.make_response(jreq, allowed)
        self.write(json.dumps(jres, sort_keys=True))

def make_app():
    return tornado.web.Application([
	(r"/", MainHandler),
	(r"/rps", RPSHandler),
    ])

if __name__ == "__main__":
    if len(sys.argv) > 1:
        read_cmdline(sys.argv[1])

    app = make_app()
    #app.listen(8080)
    app.listen(8432)
    tornado.ioloop.IOLoop.current().start()
