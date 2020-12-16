import fileinput
import sys

ping = """
  <requestHandler name="/admin/ping" class="solr.PingRequestHandler">
    <lst name="defaults">
      <str name="action">status</str>
    </lst>
    <str name="healthcheckFile">server-enabled.txt</str>
  </requestHandler>
"""

file_path = sys.argv[1]
for line in fileinput.FileInput(file_path,inplace=1):
    if "</config>" in line:
        line=ping+line
    print line,
