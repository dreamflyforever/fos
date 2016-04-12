1. search song
aaron@localhost webapi]$ python check_aihome.py -v song.ogg
{"recordId":"5649b42eac9fe20046000007","applicationId":"14476531938594b9","eof":1,"result":{"input":"我想听刘德华的冰雨","semantics":{"request":{"domain":"音乐","action":"音乐","slotcount":2,"param":{"歌曲名":"冰雨","歌手名":"刘德华"}}}}}

2. search wether
[aaron@localhost webapi]$ python check_aihome.py -v wether.ogg
{"recordId":"5649b436ac9fe20046000008","extra":{"天气":{"最低温度":"3°C","最高温度":"11°C","生活指数":{"运动指数":"较不宜(推荐您进行各种室内运动)","感冒指数":"少发(无明显降温，感冒机率较低)","紫外线指数":"最弱(辐射弱，涂擦SPF8-12防晒护肤品)"," 舒适度指数":"较舒适(白天晴，早晚偏凉，午后舒适)","洗车指数":"不宜(有雨，雨水和泥水会弄脏爱车)"},"风向":"无持续风向","天气":"多云转阴转雨夹雪","风力":"微风"}},"applicationId":"14476531938594b9","eof":1,"output":"北京二零一五年十一月十七日天气是 多云转阴转雨夹雪,最低温度是3°C,最高温度是11°C,风向是无持续风向,风力是微风.","result":{"input":"查一下北京明天的天气","semantics":{"request":{"domain":"天气","action":"天气","slotcount":2,"param":{"日期":"20151117","城市":"北京"}}}}}


API接口说明
1. 构造websocket url 【默认按照python脚本的值即可】
	  url='ws://%s:%s/%s/%s?applicationId=14476531938594b9&timestamp=%s&authId=%s&sig=%s&userId=test_yuyintianxia"' % (args.host,args.port,args.coretype,args.res,timestamp,authId,sig)
2. 创建websocket连接
		ws = websocket.create_connection(url)
3. 发送文本请求【TEXT】
		ws.send(text, websocket.ABNF.OPCODE_TEXT)
4. 发送编码后的录音数据【BINARY】
	  ws.send(unit, websocket.ABNF.OPCODE_BINARY)
	 - 可以一次性发送全部数据【这样可能会造成最后等待的时间偏长】
	 - 也可以每一秒或者几百毫秒发送一次，反复调用这个接口【类似于实时上传录音】（推荐）
5. 发送终止信息【一个长度为0的二进制消息】
		ws.send('', websocket.ABNF.OPCODE_BINARY)	 	 	
6. 接收服务器端的响应（json格式）
	  result = ws.recv()				