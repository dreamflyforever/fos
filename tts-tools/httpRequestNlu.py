#!/usr/bin/python
# -*- coding: utf-8 -*-
#输入一轮请求文本内容，对应argv[1]

import sys
import json
import hmac
import hashlib
import time
import uuid
import urllib2
import urllib
import re

appkey='14327742440003c5'
secretkey='59db7351b3790ec75c776f6881b35d7e'

coretype='cn.dlg.ita'
res = sys.argv[1]

host = '112.80.39.95'
port = 8888
text = sys.argv[2]

if __name__ == "__main__":
    timestamp = str(int(time.time()))
    authId = str(uuid.uuid1())
    sig = hmac.new(secretkey, appkey+"\n"+timestamp+"\n"+secretkey+"\n"+authId, hashlib.sha1).hexdigest()
    d2={
            "applicationId":appkey,
            "timestamp":timestamp,
            "authId":authId,
            "sig":sig,
            "params":'{"app":{"userId":"leChange"},"request":{"sdsExpand":{"prevdomain":"","lastServiceType":"cloud"},"env":"lbs_city=苏州市;","refText":"%s"}}'%(text)
        }
    p=urllib.urlencode(d2)
    url='http://%s:%d/%s/%s?%s' % (host,port,coretype,res,p)
    print url
    req=urllib2.Request(url,p)
    response = urllib2.urlopen(req)
    data=response.read()
    print data
