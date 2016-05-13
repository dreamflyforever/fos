#include <aiengine.h>
#include <cJSON.h>
extern int auth_do(char *path);

char *server_cfg = "{\
	\"luaPath\": \"bin/luabin.lub\",\
	\"appKey\": \"14500822818594e0\",\
	\"secretKey\": \"d2fa15a90d76f190fdfd81c4cd1ff55c\",\
	\"provision\": \"auth/config.json\",\
	\"serialNumber\": \"bin/serialNumber\", \
	\"audiotype\": \"ogg\",\
	\"coretype\": \"cn.dlg.ita\",\
	\"res\": \"xm_aihome\",\
	\"vad\":{\
		\"enable\": 1,\
		\"res\": \"bin/vad.aifar.0.0.2.bin\",\
		\"speechLowSeek\": 60,\
		\"sampleRate\": 16000,\
		\"strip\": 2\
	},\
	\"cloud\": {\
		\"server\": \"s.api.aispeech.com\",\
		\"port\": \"1028\"\
	},\
	\"native\": {\
		\"cn.dnn\": { \
			\"resBinPath\": \"bin/aihome_comm_xiaole_0910.bin\" \
		},\
		\"cn.asr.rec\":{\
			\"netBinPath\":\"bin/local.net.bin\",\
			\"resBinPath\":\"bin/ebnfr.aifar.0.0.1.bin\"\
		},\
		\"cn.gram\":{\
			\"resBinPath\":\"bin/ebnfc.aifar.0.0.1.bin\"\
		}\
	}\
}";

struct aiengine *aiengine_new(const char *cfg)
{
	struct aiengine *agn = malloc(sizeof(struct aiengine));
	char timestamp[1024];

	char path[2048];
	char buf[1024 * 1024];

	char *host = NULL;
	char *port = NULL;
	char *authId = "12345678909935";
	char *sig = "507626c1889ea5db9a475f1eeda9bd184d9a7913";

	char *coretype = NULL;
	char *res = NULL;

	cJSON* root=cJSON_Parse(server_cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "coretype");
	coretype = tmp->valuestring;
	tmp = cJSON_GetObjectItem(root, "res");
	res = tmp->valuestring;
	tmp = cJSON_GetObjectItem(root, "cloud");
	tmp = cJSON_GetObjectItem(tmp, "server");
	host = tmp->valuestring;
	pf("host:%s", host);
	
	tmp = cJSON_GetObjectItem(root, "cloud");
	tmp = cJSON_GetObjectItem(tmp, "port");
	port = tmp->valuestring;
	pf("port:%s", port);

	memset(timestamp, 0, 1024);
	memcpy(timestamp, "1460631415", 10);
	memset(buf, 0, 1024 * 1024);
	memset(path, 0, 2048);
	sprintf(path,
		"/%s/%s?applicationId=14476531938594b9&timestamp=%s&authId=%s&sig=%s&userId=test_yuyintianxia",
		coretype,
		res,
		timestamp,
		authId,
		sig);
	pf("path:%s\n", path);
	/*init context*/
	agn->ctx = nopoll_ctx_new();
	if (!agn->ctx) {
		printf("error nopoll new ctx failed!");
		exit(1);
	}

	/*create connection*/
	agn->conn = nopoll_conn_new(agn->ctx,
				host,
				port,
				NULL,
				path,
				NULL,
				NULL);

	if (!nopoll_conn_is_ok (agn->conn)) {
		printf("[%s %s %d]: connect error\n",
		__FILE__, __func__, __LINE__);
		return nopoll_false;
	}

	if (!nopoll_conn_wait_until_connection_ready (agn->conn, 5)) {
		printf("[%s %s %d]: connect not ready\n",
		__FILE__, __func__, __LINE__);
		return NULL;
	} else {
		printf("[%s %s %d]: connect success\n",
		__FILE__, __func__, __LINE__);
	}

	return agn;
}

int aiengine_start(struct aiengine *agn, const char *param, char id[64], aiengine_callback callback, const void *usrdata)
{
	char *coreprovidetype = NULL;
	char *audiotype = NULL;
	int channel;
	char *compress = NULL;
	int samplebytes;
	int samplerate;
	char *coretype = NULL;
	char *res = NULL;
	int cx;

	char text[1024];
	memset(text, 0, 1024);

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
	pf("coreType:%s\n", coretype);

	t = cJSON_GetObjectItem(tmp, "res");
	res = t->valuestring;
	pf("res:%s\n", res);
	sprintf(text, "{\"coreProvideType\": \"%s\",\
		\"audio\": {\"audioType\": \"%s\", \"sampleBytes\": %d,\
		\"sampleRate\": %d, \"channel\": %d, \"compress\":\"%s\"},\
		\"request\": {\"coreType\": \"%s\", \"res\": \"%s\"}}",
		coreprovidetype, audiotype, samplebytes, samplerate,
		channel, compress, coretype, res);
	printf("%s\n", text);
	cx = nopoll_conn_send_text(agn->conn, text, strlen(text));
	printf("[%s %s %d]: text len: %d\n",
		__FILE__, __func__, __LINE__,
		(int)strlen(text));
	if (cx < 0)
		printf("\n[%s %s %d]: send text error\n",
			__FILE__, __func__, __LINE__);

	agn->audioenc = audioenc_new(NULL, audioenc_notify);
	if (agn->audioenc == NULL)
		pf("audioenc NULL\n");
	agn->cfg = (agn_audioenc_cfg_t *)calloc(1, sizeof(*agn->cfg));
	if (!agn->cfg) {
		printf("calloc cfg failed!\n");
		while(1);
	} else {
		agn->cfg->spx_quality = 8;
		agn->cfg->spx_complexity = 2;
		agn->cfg->spx_vbr = 0;
	}
	audioenc_start(agn->audioenc, 16000, 1, 16, agn->cfg);

	return 0;
}

int aiengine_feed(struct aiengine *agn, const void *data, int size)
{
	audioenc_encode(agn->audioenc, data, size);
	return 0;
}

int aiengine_stop(struct aiengine *agn)
{
	if (agn->audioenc) {
		audioenc_delete(agn->audioenc);
	}
	audioenc_stop(agn->audioenc);
	return 0;
}

int aiengine_delete(struct aiengine *agn)
{
	free(agn->cfg);
	return 0;
}

int aiengine_cancel(struct aiengine *engine)
{
	return 0;
}

int check_provision(struct aiengine *agn)
{
	int ret;
	cJSON* root=cJSON_Parse(server_cfg);
	cJSON* tmp = cJSON_GetObjectItem(root, "provision");
	printf("provision path: %s\n", tmp->valuestring);
	ret = auth_do(tmp->valuestring);
	return ret;

}
