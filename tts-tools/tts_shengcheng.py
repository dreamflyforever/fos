#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import json
import hmac
import hashlib
import time
import uuid
import urllib2
import urllib

appkey='14806716618595ef'
secretkey='685e23f477c36adb21320152ba9be2ef'

#appkey='1402276278000001'
#secretkey='247297701abb9eafd2508b91cd073183'
coretype='cn.sent.syn'
res = 'syn_chnsnt_qianranf'
#res = 'syn_chnsnt_anonyf'
#syn_chnsnt_anonyf
#syn_chnsnt_qianranf
#syn_chnsnt_anonyg
#syn_chnsnt_lzliaf

# 开发环境 112.80.39.95:8010
#host='112.80.39.95'
host='s.api.aispeech.com'
#port=8888
port=1028

voice='voice'
#text='我还没有听到你说话，请重新唤醒使用'
text=sys.argv[1];
audiotype='mp3'
voicename=text+'.mp3'

if __name__ == "__main__":
    timestamp = str(int(time.time()))
    authId = str(uuid.uuid1())
    sig = hmac.new(secretkey, appkey+"\n"+timestamp+"\n"+secretkey+"\n"+authId, hashlib.sha1).hexdigest()
    d2={
                "applicationId":appkey,
                "timestamp":timestamp,
                "authId":authId,
                "sig":sig,
                "params":'{"audio": {"audioType": "%s","sampleBytes": 2,"sampleRate":16000,"channel":1,"compress":"raw"},"request": {"coreType": "%s","res": "%s","refText":"%s","realBack":0,"speechRate":0.10}}' % (audiotype,coretype,res,text)
        }
    p=urllib.urlencode(d2)
    url='http://%s:%d/%s/%s?%s' % (host,port,coretype,res,p)
    print url
    req=urllib2.urlopen(url)
    data=req.read()
    print len(data)
    if len(data)>0:
        fd = open(voicename, "wb")
        fd.write(data)
        fd.close()
