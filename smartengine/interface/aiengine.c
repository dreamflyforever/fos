#include <aiengine.h>
#include <cJSON.h>
#include <agn_hmac_sha1.h>
#include <time.h>
#include "base.h"
#include <uuid/uuid.h>
char session[40] = {0};
extern int auth_do(char *path);

int _audioenc_notify(void *user_data,
		unsigned char *body,
		int body_len,
		unsigned char *head,
		int head_len)
{
	struct aiengine *agn = (struct aiengine *)user_data;
	if (agn->conn == NULL) {
		pf("conn == NULL\n");
		return 0;
	}

	if (!nopoll_conn_is_ok(agn->conn)) {
		pf("connect error\n");
		return nopoll_false;
	}

	if (!nopoll_conn_wait_until_connection_ready(agn->conn, 5)) {
		pf("connect not ready\n");
		return 0;
	} else {
		//pf("connect success\n");
	}
#if 1
	nopoll_conn_send_binary(agn->conn, (const char *)head, head_len);
	nopoll_sleep(10000);

	nopoll_conn_send_binary(agn->conn, (const char *)body, body_len);
	nopoll_sleep(10000);
#endif
	return 0;
}

struct aiengine *aiengine_new(const char *cfg)
{
	struct aiengine *agn = malloc(sizeof(struct aiengine));
	char timestamp[12] = {0};

	char *path = malloc(600);
	char *buf = malloc(128); 
	char *host = NULL;
	char *port = NULL;
	char *appkey = NULL;
	char *secretkey = NULL;
	char *userid = NULL;

	char *coretype = NULL;
	char *res = NULL;
	int len;
 
	uuid_t uuid;
	char authId[36] = {0};
    	uuid_generate(uuid);
	uuid_unparse(uuid, authId);

	cJSON *root = cJSON_Parse(cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "coretype");
	coretype = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "appKey");
	appkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "secretKey");
	secretkey = tmp->valuestring;
	pf("secretKey: %s\n", secretkey);

	memset(agn, 0, sizeof(struct aiengine));
	tmp = cJSON_GetObjectItem(root, "provision");
	len = strlen(tmp->valuestring);
	agn->provision_path = (char *)malloc(len);
	strncpy(agn->provision_path, tmp->valuestring, len);

	tmp = cJSON_GetObjectItem(root, "res");
	res = tmp->valuestring;
	tmp = cJSON_GetObjectItem(root, "cloud");
	tmp = cJSON_GetObjectItem(tmp, "server");
	host = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "app");
	tmp = cJSON_GetObjectItem(tmp, "userId");
	userid = tmp->valuestring;
	len = strlen(userid);
	agn->userid = (char *)malloc(len + 1);
	memset(agn->userid, 0, len + 1);
	strncpy(agn->userid, tmp->valuestring, len);

	tmp = cJSON_GetObjectItem(root, "cloud");
	tmp = cJSON_GetObjectItem(tmp, "port");
	port = tmp->valuestring;

	memset(path, 0, 600);
	
	int sec = time(NULL);
	itoa(sec, timestamp);
	char *ts = "10334";
	char *authid = "54352b05-273a-466e-bd50-0f6bcc3f409a";
	//sprintf(timestamp, "%d", (int)rand);
	sprintf(buf, "%s\n%s\n%s%s", appkey, ts, secretkey, authid);
	char *sig = hmac_sha1(secretkey, buf);
	printf("%s\n", sig);
#if DUI
	sprintf(path, "/dds/v1/prod?serviceType=websocket&productId=278575318&"
		"deviceName=%s&userId=\"numberone\"&accessToken=geComesHere"
		"&deviceId=tryDeviceId&securityCode=geComesHere", "123545678");
#else
#if use_pcm
	sprintf(path,
		"/%s/%s?version=1.0.0&applicationId=%s&timestamp=%s"
		"&authId=%s&sig=%s",
		coretype,
		res,
		appkey,
		timestamp,
		authId,
		sig);
#else

	if (strcmp(userid, "wechat") == 0) {
		sig = hmac_sha1(appkey, "0123456789");
		sprintf(path, "/AIHomeMP/websocket?sig=%s&deviceid=%s", sig, "abcdef");
	} else {
		sprintf(path,
			"/%s/%s?applicationId=%s&timestamp=%s"
			"&authId=%s&sig=%s&userId=%s",
			coretype,
			res,
			appkey,
			timestamp,
			authId,
			sig,
			userid);
	}
#endif
#endif
#if use_free_server
	sprintf(buf, "%s\n%s\n%s%s", appkey, timestamp);
	sig = hmac_sha1(appkey, buf);
	sprintf(path, "/appkey=%s&timestamp=%s&sig=%s", appkey, timestamp, sig);
#endif
	/*init context*/
	agn->ctx = nopoll_ctx_new();
	if (!agn->ctx) {
		printf("error nopoll new ctx failed!");
		goto handle;
	}
#if DUI
	/*create connection*/
	agn->conn = nopoll_conn_new(
			agn->ctx,
			"dds.dui.ai",
			"80",
			NULL,
			path,
			NULL,
			NULL
			);
#elif WECHAT
	char *_host = "test.iot.aispeech.com";
	char *_port = "80";
	char *_path = "/AIHomeMP/websocket?deviceId=123&wechatId=456";
	/*create connection*/
	agn->conn = nopoll_conn_new(
			agn->ctx,
			_host,
			_port,
			NULL,
			_path,
			NULL,
			NULL
			);
#else
	char *pa="/cn.asr.rec/english?applicationId=1531109559458467&timestamp=10334&authId=54352b05-273a-466e-bd50-0f6bcc3f409a&sig=ebe2ee6b3b463ea8f684368b154b46dc0f47ae16&userId=wifiBox";
	 /*create connection*/
	agn->conn = nopoll_conn_new(
			agn->ctx,
			host,
			port,
			NULL,
			pa,
			NULL,
			NULL
			);
#endif
	if (!nopoll_conn_is_ok(agn->conn)) {
		pf("connect error\n");
		goto handle;
	}
#if 0
	[aiengine_server_init server] scheme: ws, host: s.api.aispeech.com, port: 1028, path: /cn.asr.rec/english?applicationId=1531109559458467&timestamp=50&authId=8bca20b7-65ec-4b78-9443-4f5d15421836&sig=032379cc5ec722039c2188f43809c71bcefd8fc5&userId=wifiBox, userid: wifiBox

[T: 51008 M: common C: info F: display_event_handler L: 171]: change display: 080A

ws://s.api.aispeech.com:1028/cn.asr.rec/english?applicationId=1531109559458467&timestamp=15454&authId=54352b05-273a-466e-bd50-0f6bcc3f409a&sig=6cc5e59c7cece7baf21df17791d0fc2a10b0bb51&userId=wifiBox
#endif
	if (!nopoll_conn_wait_until_connection_ready(agn->conn, 5)) {
		printf("connect not ready\n");
	} else {
		printf("connect success\n");
	}
handle:
	free(sig);
	free(path);
	agn->provision_ok = 1;
	free(buf);
	cJSON_Delete(root);
	return agn;
}

int aiengine_start(struct aiengine *agn,
		const char *param,
		aiengine_callback cb,
		const void *usrdata)
{
	int retvalue = 0;
	char *coreprovidetype = NULL;
	char *audiotype = NULL;
	int channel;
	char *compress = NULL;
	int samplebytes;
	int samplerate;
	char *coretype = NULL;
	char *res = NULL;
	int cx;

	char *text = malloc(1024);
	if (text == NULL) {
		retvalue = -1;
		goto handle;
	}

	if (agn->provision_ok != 1) {
		pf("aiengine provision check faile\n");
		return -1;
	}

	cJSON *root=cJSON_Parse(param);
	cJSON *tmp = cJSON_GetObjectItem(root, "coreProvideType");
	coreprovidetype = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "audio");
	cJSON *t = cJSON_GetObjectItem(tmp, "audioType");
	audiotype = t->valuestring;

	t = cJSON_GetObjectItem(tmp, "sampleRate");
	samplerate = t->valueint;

	t = cJSON_GetObjectItem(tmp, "channel");
	channel = t->valueint;

	t = cJSON_GetObjectItem(tmp, "compress");
	compress = t->valuestring;

	t = cJSON_GetObjectItem(tmp, "sampleBytes");
	samplebytes = t->valueint;

	tmp = cJSON_GetObjectItem(root, "request");
	t = cJSON_GetObjectItem(tmp, "coreType");
	coretype = t->valuestring;

	t = cJSON_GetObjectItem(tmp, "res");
	res = t->valuestring;

	memset(text, 0, 1024);
#if DUI
	if (usrdata == NULL) {
		char *sessionid = aiengine_get_dui_session(agn);
		if (sessionid != NULL) {
			sprintf(text, "{"
				"\"topic\": \"recorder.stream.start\","
				"\"recordId\": \"12341asdfa\","
				"\"sessionId\":\"%s\","
				"\"audio\": {"
					"\"audioType\": \"ogg\","
					"\"sampleRate\": 16000,"
					"\"channel\": 1,"
					"\"sampleBytes\": 2}"
				"}", sessionid);
			free(sessionid);
		} else {
			sprintf(text, "{"
				"\"topic\": \"recorder.stream.start\","
				"\"recordId\": \"12341asdfa\","
				"\"audio\": {"
					"\"audioType\": \"ogg\","
					"\"sampleRate\": 16000,"
					"\"channel\": 1,"
					"\"sampleBytes\": 2}"
				"}");
		}
	} else {
		sprintf(text, "{"
			"\"topic\": \"nlu.input.text\","
			"\"recordId\": \"12341asdfa\","
			"\"refText\": \"%s\"}", usrdata);
	}
#else
#if use_pcm
	sprintf(text, "{\"request\":{\"sdsExpand\":{\"lastServiceType\":\"cloud\","
	"\"prevdomain\":\"\"},\"version\":\"1.0.0\",\"contextId\":\"\",\"recordId\""
	":\"5836b9b835847e46d1000010\",\"res\":\"airobot\",\"env\":\"lbs_city=\\\"苏"
	"州市\\\";use_frame_split=0;use_vad_restart=1;\",\"coreType\":\"cn.sds\"},"
	"\"app\":{\"deviceId\":\"dev-test\",\"userId\":\"leChange\",\"applicationId\""
	":\"1454033453859541\"},\"audio\":{\"audioType\":\"wav\",\"sampleRate\":16000,"
	"\"sampleBytes\":2,\"channel\":1}}");

	printf("%s, %d\n", text, strlen(text));
#else
	char *r = strstr(param, "sdsExpand");
	if (r != NULL) {
		printf("enter sds\n");
		sprintf(text, "{\"coreProvideType\": \"%s\",\
			\"app\":{\"userId\":\"%s\"},\
			\"audio\": {\"audioType\": \"%s\", \"sampleBytes\": %d,\
			\"sampleRate\": %d, \"channel\": %d, \"compress\":\"%s\"},\
			\"request\": {\"coreType\": \"%s\", \"res\": \"%s\",\
			\"sdsExpand\":{\"prevdomain\":\"\", \"lastServiceType\": \"cloud\"}}}",
			coreprovidetype, agn->userid, audiotype, samplebytes, samplerate,
			channel, compress, coretype, res);
	} else {
		printf("enter sync\n");
		sprintf(text, "{\"coreProvideType\": \"%s\",\
			\"audio\": {\"audioType\": \"%s\", \"sampleBytes\": %d,\
			\"sampleRate\": %d, \"channel\": %d, \"compress\":\"%s\"},\
			\"request\": {\"coreType\": \"%s\", \"res\": \"%s\"}}",
			coreprovidetype, audiotype, samplebytes, samplerate,
			channel, compress, coretype, res);
	}
#endif
#endif
	if (agn->conn == NULL) {
		pf("conn == NULL\n");
		retvalue = -1;
		goto error_handle;
	}
	printf("\n\n%s\n\n", text);
	cx = nopoll_conn_send_text(agn->conn, text, strlen(text));
	if (cx < 0) {
		printf("\n[%s %s %d]: send text error\n",
				__FILE__, __func__, __LINE__);
		retvalue = -1;
		goto error_handle;
	}
#if !use_pcm
	agn->audioenc = audioenc_new(agn, _audioenc_notify);
	if (agn->audioenc == NULL) {
		pf("audioenc NULL\n");
		retvalue = -1;
		goto error_handle;
	}
	agn->cfg = (agn_audioenc_cfg_t *)calloc(1, sizeof(*agn->cfg));
	if (!agn->cfg) {
		printf("calloc cfg failed!\n");
		retvalue = -1;
		goto error_handle;
	} else {
		agn->cfg->spx_quality = 8;
		agn->cfg->spx_complexity = 2;
		agn->cfg->spx_vbr = 0;
	}
	audioenc_start(agn->audioenc, 16000, 1, 16, agn->cfg);
	if (cb == NULL) {
		pf("cb NULL\n");
	}
#endif
	agn->cb = cb;
	agn->usrdata = usrdata;
error_handle:
	free(text);
	cJSON_Delete(root);
handle:
	return retvalue;
}

int aiengine_feed(struct aiengine *agn, const void *data, int size)
{
	if (agn->provision_ok != 1) {
		pf("aiengine provision check faile\n");
		return -1;
	}
	pf("size: %d\n", size);
	if (agn->conn == NULL) {
		pf("conn == NULL\n");
		return 0;
	}

#if use_pcm
	nopoll_conn_send_binary(agn->conn, data, size);
	nopoll_sleep(100000);
#else
	audioenc_encode(agn->audioenc, data, size);
#endif
	return 0;
}

int aiengine_stop(struct aiengine *agn)
{
	int retvalue = 0;

	char *buff = malloc(1024*1024);
	if (buff == NULL) {
		retvalue = -1;
		goto error;
	}

	if (agn == NULL) {
		pf("error: agn == NULL");
		retvalue = -1;
		goto error;
	}

	memset(buff, 0, 1024*1024);
	int times = 0;

	if (!nopoll_conn_is_ok(agn->conn)) {
		pf("connect error\n");
		retvalue = -1;
		goto error;
	}

	if (!nopoll_conn_wait_until_connection_ready(agn->conn, 5)) {
		pf("connect not ready\n");
		retvalue = -1;
		goto error;
	} else {
	}
#if !use_pcm	
		audioenc_stop(agn->audioenc);
#endif
	/*raw send data API*/
	nopoll_conn_send_frame(agn->conn, 1, 1, 2, 0, "", 0);
#if DUI
	int i;
	for (i = 0; i < 2; i++) {
		times = 0;
		printf("recv times: %d\n", i);
		memset(buff, 0, 1024*1024);
#endif
		while ((agn->msg = nopoll_conn_get_msg(agn->conn)) == NULL) {
			if (!nopoll_conn_is_ok(agn->conn)) {
				printf ("ERROR: received websocket connection close"
        	                " during wait reply..\n");
				return nopoll_false;
			}
			nopoll_sleep(10000);
			times++;
			if (times > 5000) {
				printf("message get nothing\n");
				goto msg_error;
			}
		}
		memcpy(buff,
        	        (char *)nopoll_msg_get_payload(agn->msg),
        	        nopoll_msg_get_payload_size(agn->msg));

		agn->size = nopoll_msg_get_payload_size(agn->msg);

		if (agn->cb) {
			pf("msg received: %s\n", buff);
#if DUI
			//aiengine_set_dui_session(agn, buff, agn->size);
#endif
			agn->cb(agn->usrdata, buff, agn->size);
		} else {
		}
#if DUI
	}
#endif
msg_error:
	/*unref message*/
	nopoll_msg_unref(agn->msg);
error:
#if !use_pcm
	if (agn->audioenc) {
		audioenc_delete(agn->audioenc);
	}
#endif
	if (agn->cfg)
		free(agn->cfg);
	if (buff != NULL)
		free(buff);
	printf("aiengine stop\n");
	return retvalue;
}

int aiengine_delete(struct aiengine *agn)
{
	int retvalue = 0;
	if (agn == NULL) {
		retvalue = -1;
		goto error;
	}
	/*finish connection*/
	if (agn->conn != NULL) {
		nopoll_conn_close(agn->conn);
	}
	/*finish*/
	if (agn->ctx)
		nopoll_ctx_unref(agn->ctx);
	if (agn->provision_path)
		free(agn->provision_path);
	if (agn->userid)
		free(agn->userid);
	if (agn)
		free(agn);
error:
	return retvalue;
}

int aiengine_cancel(struct aiengine *engine)
{
	return 0;
}

int check_provision(struct aiengine *agn)
{
	int ret;
#if 0
	if (agn == NULL)
		pf("agn NULL\n");

	pf("provision path: %s\n", agn->provision_path);
	ret = auth_do(agn->provision_path);
	if (ret != 0) {
		pf("check provision error\n");
		agn->provision_ok = -1;
	} else {
		pf("check provision success\n");
		agn->provision_ok = 1;
	}

	/*XXX:*/
#endif
	agn->provision_ok = 1;
	return ret;
}


char *text_asr_output(char *cfg, char *text)
{
	char *appkey;
	char *secretkey;
	char timestamp[11] = {0};
	char *userid;
	int samplebytes;
	int samplerate;
	int channel;
	int speechvolume;
	char *coretype;
	float speechrate;
	int rightmargin;
	int realback;
	char *res; char *server;
	char *port;
	char *audiotype;
	char *buf = malloc(100);
	memset(buf, 0, 100);
	if ((cfg == NULL) & (text == NULL)) {
		printf("please make sure cfg and text is right!");
		return NULL;
	}

	cJSON *root = cJSON_Parse(cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "appKey");
	appkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "secretKey");
	secretkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "userId");
	userid = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "coretype");
	coretype = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "cloud");
	cJSON *t = cJSON_GetObjectItem(tmp, "server");
	server = t->valuestring;
	t = cJSON_GetObjectItem(tmp, "port");
	port = t->valuestring;

	tmp = cJSON_GetObjectItem(root, "audio");
	t = cJSON_GetObjectItem(tmp, "sampleBytes");
	samplebytes = t->valueint;
	t = cJSON_GetObjectItem(tmp, "sampleRate");
	samplerate = t->valueint;
	t = cJSON_GetObjectItem(tmp, "channel");
	channel = t->valueint;
	t = cJSON_GetObjectItem(tmp, "audioType");
	audiotype = t->valuestring;

	tmp = cJSON_GetObjectItem(root, "request");
	t = cJSON_GetObjectItem(tmp, "speechVolume");
	speechvolume = t->valueint;
	t = cJSON_GetObjectItem(tmp, "speechRate");
	speechrate = t->valueint;
	t = cJSON_GetObjectItem(tmp, "rightMargin");
	rightmargin = t->valueint;
	t = cJSON_GetObjectItem(tmp, "realBack");
	realback = t->valueint;
	t = cJSON_GetObjectItem(tmp, "res");
	res = t->valuestring;

	unsigned int sec = time(NULL);
	itoa(sec, timestamp); 
	uuid_t uuid;
    	char authid[36] = {0};
    	uuid_generate(uuid);
	uuid_unparse(uuid, authid);
	sprintf(buf, "%s\n%s\n%s\n%s", appkey, timestamp, secretkey, authid);
	char *sig = hmac_sha1(secretkey, buf);
	char *head = malloc(150);
	memset(head, 0, 150);
	sprintf(head, "applicationId=%s&timestamp=%s&authId=%s&sig=%s&params=",
			appkey, timestamp, authid, sig);
	char *buff = malloc(strlen(text) + 300);
	memset(buff, 0, (int)strlen(text) + 300);

	sprintf(buff, "{"
			"\"app\":"
				"{\"userId\":\"dreamflyfoerver\","
				"\"userName\":\"jim\"},"
			"\"request\":{\"sdsExpand\":{\"prevdomain\":\"\","
				"\"lastServiceType\":\"cloud\"},"
				"\"recordId\":\"58888888888888888888888888888114\","
				"\"refText\":\"%s\"}}", text);

	char *tt = url_encode(buff);
	char *b = malloc(strlen(tt) + strlen(head) + 1);
	memset(b, 0, strlen(tt) + strlen(head) + 1);
	sprintf(b, "%s%s", head, tt);
	char *url = malloc(1024 + strlen(b));
	sprintf(url, "http://%s:%s/cn.sds/airobot?%s",
		server, port, b);
	cJSON_Delete(root);
	free(tt);
	free(head);
	free(sig);
	free(buf);
	free(buff);
	free(b);
	return url;
}

#if DUI

char *fetch_session(char const *start, char *key, unsigned long length)
{
	char *p = strstr(start, key);
	if (p == NULL) return NULL;
	char *url = malloc(2048);
	memset(url, 0, 2048);
	int i = 0;
	int len = strlen(key);
	p = p + len + 2;
	/*TODO: add size judge*/
	while (!((p[i] == '}') || (p[i] == ','))) {
		url[i] = p[i];
		if (p[i] == "\0") break;
		i++;
		if (i == 200) break;
	}

	//printf("[%s %s %d]%s\n", __FILE__, __func__, __LINE__, url);
	return url;
}

char *aiengine_get_dui_session(struct aiengine *agn)
{
	printf("%s %d\n", __func__, __LINE__);
	char *tmp = NULL;
	if (strlen(session) != 0) {
		printf("[%s %d]sessionid: %s\n", __func__, __LINE__, session);
		tmp = malloc(40);
		memset(tmp, 0, 40);
		memcpy(tmp, session, 38);
	} else {
		printf("[%s %d]no sessionid\n", __func__, __LINE__);
	}
	return tmp;
}

int aiengine_set_dui_session(struct aiengine *agn, char *buf, int size)
{
	int retvalue = 0;
	char *sessionid = NULL;
	char *is_session_end = fetch_session(buf, "shouldEndSession", 0);
	if (is_session_end == NULL) {
		retvalue = -1;
		goto handle_is_session_end;
	}
	printf("[%d]shouldEndSession: %s\n", __LINE__, is_session_end);
	if (strcmp(is_session_end, "false") == 0) {
		char *sessionid = fetch_key(buf, "sessionId", 0);
		if (sessionid == NULL) {
			retvalue = -1;
			goto handle_sessionid;
		}
		memset(session, 0, 40);
		memcpy(session, sessionid, 39);
		printf("[%d]sessionid : %s\n", __LINE__, sessionid);
	}
	free(sessionid);
handle_sessionid:
	free(is_session_end);
handle_is_session_end:
	return retvalue;
}

int dui_result_process(char *buf, int size,
	char *output_speakurl, char *output_linkurl)
{
	int retval = 0;
	char *linkurl = NULL;
	char *speakurl = NULL;
	char *output = NULL;
	if (buf == NULL) {
		retval = -1;
		goto error;
	}
	speakurl = fetch_key(buf, "speakUrl", 0);
	if (speakurl != NULL) {
		printf("speakUrl: %s\n", speakurl);
		memcpy(output_speakurl, speakurl, strlen(speakurl));
	}
	linkurl = fetch_key(buf, "linkUrl", 0);
	if (linkurl != NULL) {
		printf("linkUrl: %s\n", linkurl);
		memcpy(output_linkurl, linkurl, strlen(linkurl));
	}
	output = fetch_key(buf, ",\"nlg", 0);
	if (output == NULL) {
		output = fetch_key(buf, "{\"nlg", 0);
	}
	if (output != NULL) printf("\n\n%s\n\n", output);
	else printf("\n\nNULL\n\n");

	free(speakurl);
	free(linkurl);
	free(output);
error:
	return retval;
}

char *player_url(char *buf)
{
	char *returl = NULL;
	if (buf == NULL) {
		goto end;
	}
	char *url = strstr(buf, "refText=");
	if (url == NULL) {
		goto end;
	}
	char url_encode_part[100] = {0};
	int i;
	for (i = 0; i < 100; i++) {
		url_encode_part[i] = url[i+8];
		if (url[i+8] == '&') break;
	}
	char *path = url_encode(url_encode_part);
	returl = malloc(1024);
	memset(returl, 0, 1024);
	sprintf(returl,
		"https://s.dui.ai/tts/v1?refText=%s&speed=1.000000&volume=30&speaker=lucyf",
		path);
	free(path);
end:
	return returl;
}
#endif
