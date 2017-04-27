# encoding:UTF-8
import urllib.request
import json
import datetime

#url = "http://www.nffund.com/new/fundproduct/?action=searchjjgk&fundcode=159948#secondMenu"
url="http://www.nffund.com/FundNewServlet.java?action=searchsgsh&fundcode=159948"
data = urllib.request.urlopen(url).read()
data = data.decode('UTF-8')
result_obj=json.loads(data)
print(result_obj)



