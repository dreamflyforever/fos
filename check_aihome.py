#!/usr/bin/python
# -*- coding: utf-8 -*-

# a simple script for monitor vst,aihud and other chezai service
# ToDo:
#     a). a common framework to contain most service
#     b). check all of backends

import websocket
import sys
import argparse
import json
import hmac
import hashlib
import time
import uuid



appkey='1458977484859577'
secretkey='d19c742b65bfe89857130819297ace0f'

audiotype='ogg'
coretype='cn.dlg.ita'
res='xm_aihome'
host='s-test.api.aispeech.com'
port='10000'
voice='song.ogg'



if __name__ == "__main__":


    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--host',    default=host, help='hostip, default: {0}'.format(host))
    parser.add_argument('-p', '--port',    default=port, help='port, default: {0}'.format(port))
    parser.add_argument('-v', '--voicename',    default=voice, help='auido voice, default: {0}'.format(voice))
    parser.add_argument('-a', '--audiotype',default=audiotype, help='audio type, default: {0}'.format(audiotype))
    parser.add_argument('-c', '--coretype',     default=coretype, help='core type, default: {0}'.format(coretype))
    parser.add_argument('-r', '--res',      default=res, help='res name, default: {0}'.format(res))
    args = parser.parse_args()

    fd = open(args.voicename, "rb")
    data = fd.read()
    fd.close()
    
    timestamp = str(int(time.time()))
    authId = str(uuid.uuid1())
    #sig = hmac-sha1(secretkey, appkey+"\n"+timestamp+"\n"+secretkey+"\n"+authId)
    sig = hmac.new(secretkey, appkey+"\n"+timestamp+"\n"+secretkey+"\n"+authId, hashlib.sha1).hexdigest()
    #print sig
         
    url='ws://%s:%s/%s/%s?applicationId=14476531938594b9&timestamp=%s&authId=%s&sig=%s&userId=test_yuyintianxia"' % (args.host,args.port,args.coretype,args.res,timestamp,authId,sig)
    ws = websocket.create_connection(url)
    text='{"coreProvideType": "cloud","audio": {"audioType": "%s","sampleBytes": 2,"sampleRate": 16000,"channel": 1,"compress":"raw"},"request": {"coreType": "%s","res": "%s"}}' % (args.audiotype,args.coretype,args.res)
    ws.send(text, websocket.ABNF.OPCODE_TEXT)
    
    #send all one time
    #ws.send(data, websocket.ABNF.OPCODE_BINARY)
    
    #send 100ms per time   
    size = len(data)    
    left = 0
    right = 1024*3
    counter = 0
    while left < size:
    	unit = data[left:right]
    	ws.send(unit, websocket.ABNF.OPCODE_BINARY)
    	time.sleep(0.1)
    	counter = counter + 1
    	left=right
    	right = right+1024*3
    	#print counter   	    
    
    ws.send('', websocket.ABNF.OPCODE_BINARY)
    result = ws.recv()
    print result
    ws.close()

