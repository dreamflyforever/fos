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
import datetime
import os

appkey='14327742440003c5'
secretkey='59db7351b3790ec75c776f6881b35d7e'

coretype='cn.sds'
res = sys.argv[1]

plat_addr = sys.argv[2]

#本地主机
if plat_addr == 'devn':
    host='10.12.8.10'
    port=8088
#本地主机
elif plat_addr == 'local':
    host='172.16.10.105'
    port=6380
#开发环境
elif plat_addr == 'dev':
    host='112.80.39.95'
    port=8888
#灰度环境
elif plat_addr == 'gray':
    host='s-test.api.aispeech.com'
    port=10000
#产品环境
elif plat_addr == 'prod':
    host='s.api.aispeech.com'
    port=1028

text = sys.argv[3]

if __name__ == "__main__":
    timestamp = str(int(time.time()))
    authId = str(uuid.uuid1())
    sig = hmac.new(secretkey, appkey+"\n"+timestamp+"\n"+secretkey+"\n"+authId, hashlib.sha1).hexdigest()
    d2={
            "applicationId":appkey,
            "timestamp":timestamp,
            "authId":authId,
            "sig":sig,
            "params":'{"app":{"userId":"leChange"},"request":{"sdsExpand":{"prevdomain":"","lastServiceType": "cloud"},"coreType": "%s","res": "%s","refText":"%s"}}'%(coretype,res,text)
        }
    p=urllib.urlencode(d2)
    url='http://%s:%d/%s/%s?%s&version=0.2.0' % (host,port,coretype,res,p)
    print url
    starttime = datetime.datetime.now()
    req = urllib2.urlopen(url)
    endtime = datetime.datetime.now()
    data = req.read()
    print data
