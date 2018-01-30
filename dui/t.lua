local wb = lasa.websocket.client:new()
--local ok, err = wb:connect('ws://172.16.10.19:9008/dm/v1/test?productId=100000404&serviceType=websocket&deviceId=xxx&userId=aaa')
local ok, err = wb:connect('ws://s.dui.ai/dm/v1/prod?productId=100000171&serviceType=websocket&deviceId=xxx&userId=aaa')
print('connect', ok, err)
local audiores = './t/tools/speech/t1.wav'
local cco, rco
local cjson = require("dkjson")

local function _send_pcm(ws, pcm)
	if not pcm then return end
	local step = 3200 - 1
	local step_idx = 3200
	local pos = 1
	local len = #pcm
	
	while pos <= len do
		if len - pos + 1 <= 3200 then
			local b, e = wb:send_binary( string.sub(pcm, pos, -1))
			print('feed', b, e)
			break
		else
			local b, e = wb:send_binary(string.sub(pcm, pos, pos + step))
			print('feed', b, e)
			pos = pos + step_idx
		end
	end
end

--语音请求
--cco = lasa.thread.spawn(function() 
--    local fp = io.open(audiores, 'rb')
--    local data = fp:read('*a')
--    fp:close()
--
--	local param = [[
--	{
--		 "topic": "recorder.stream.start",
--	     "recordId": "12341asdfa",
--	     "audio": {
--	         "audioType": "wav",
--	         "sampleRate": 16000,
--	         "channel": 1,
--	         "sampleBytes": 2
--	     }
--	}
--	]]
--	
--	--wb:set_timeout(3)
--    local b, e = wb:send_text(param)
--    print('send start', b, e)
--    if e then return end
--
--	if data then
--	    --wb:set_timeout(3)
--	    
--	    _send_pcm(wb, data)
--	 	
--	 	--wb:set_timeout(3)
--	    b, e = wb:send_binary('')
--	    print('send stop', b, e)
--    end
--end)

--文本请求语义
cco = lasa.thread.spawn(function()
		local param = {}
		param['topic'] = 'nlu.input.text'
		param['recordId'] = '12341asdfa'
		param['refText'] = arg[1]
		--print(cjson.encod(param))
		--wb:set_timeout(3)
		local b, e = wb:send_text(cjson.encode(param))
		print('send start', b, e)
		if e then return end
		end)

--事件请求对话
--cco = lasa.thread.spawn(function() 
--	local param = [[
--	{
--		 "topic": "dm.input.event",
--	     "eventId": "595b047f89e13800016011c2"
--	}
--	]]
--	
--	--wb:set_timeout(3)
--    local b, e = wb:send_text(param)
--    print('send start', b, e)
--    if e then return end
--end)

rco = lasa.thread.spawn(function()
	while true do
	    --wb:set_timeout(10)
	    local d, t, e = wb:recv_frame()
	    print('recv frame', d, t, e)
	    if 0 then break end
	    if e then break end
    end
end)

lasa.thread.wait(rco, cco)
lasa.thread.kill(rco)
lasa.thread.kill(cco)
