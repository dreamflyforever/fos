#include <aiengine.h>
#include <cJSON.h>
#include <sig_output.h>

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
	nopoll_conn_send_binary(agn->conn, (const char *)head, head_len);
	nopoll_sleep(10000);

	nopoll_conn_send_binary(agn->conn, (const char *)body, body_len);
	nopoll_sleep(10000);
	return 0;
}

struct aiengine *aiengine_new(const char *cfg)
{
	struct aiengine *agn = malloc(sizeof(struct aiengine));
	char timestamp[1024];

	char path[2048];
	char buf[1024 * 1024];

	char *host = NULL;
	char *port = NULL;
	char *authId = "12345678909935";
	char *appkey = NULL;
	char *secretkey = NULL;

	char *coretype = NULL;
	char *res = NULL;
	int len;

	cJSON* root = cJSON_Parse(cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "coretype");
	coretype = tmp->valuestring;
	
	tmp = cJSON_GetObjectItem(root, "appkey");
	appkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "secretkey");
	secretkey = tmp->valuestring;

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

	tmp = cJSON_GetObjectItem(root, "cloud");
	tmp = cJSON_GetObjectItem(tmp, "port");
	port = tmp->valuestring;

	memset(timestamp, 0, 1024);
	memcpy(timestamp, "1460631415", 10);
	memset(buf, 0, 1024 * 1024);
	memset(path, 0, 2048);

	char buffer[1024];
	sprintf(buffer, "%s\n%s\n%s\n%s", appkey, timestamp, secretkey, authId);
	char *sig = sig_output(buf);

	sprintf(path,
		"/%s/%s?applicationId=14476531938594b9&timestamp=%s"
		"&authId=%s&sig=%s&userId=test_yuyintianxia",
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

	if (!nopoll_conn_is_ok(agn->conn)) {
		printf("[%s %s %d]: connect error\n",
		__FILE__, __func__, __LINE__);
		return nopoll_false;
	}

	if (!nopoll_conn_wait_until_connection_ready(agn->conn, 5)) {
		printf("[%s %s %d]: connect not ready\n",
		__FILE__, __func__, __LINE__);
		return NULL;
	} else {
		printf("[%s %s %d]: connect success\n",
		__FILE__, __func__, __LINE__);
	}

	return agn;
}

int aiengine_start(struct aiengine *agn,
			const char *param,
			aiengine_callback cb,
			const void *usrdata)
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

	agn->audioenc = audioenc_new(agn, _audioenc_notify);
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
	if (cb == NULL) {
		pf("cb NULL\n");	
	} else {
	}
	agn->cb = cb;
	agn->usrdata = usrdata;

	return 0;
}

int aiengine_feed(struct aiengine *agn, const void *data, int size)
{
	audioenc_encode(agn->audioenc, data, size);
	return 0;
}

int aiengine_stop(struct aiengine *agn)
{
	char buff[1024 * 1024];
	/*raw send data API*/
	nopoll_conn_send_frame(agn->conn, 1, 1, 2, 0, "", 0);

	while ((agn->msg = nopoll_conn_get_msg(agn->conn)) == NULL) {
		if (! nopoll_conn_is_ok (agn->conn)) {
			printf ("ERROR: received websocket connection close during wait reply..\n");
			return nopoll_false;
		}
		nopoll_sleep(10000);
	}
	memcpy(buff, (char *)nopoll_msg_get_payload (agn->msg), nopoll_msg_get_payload_size(agn->msg));
	agn->message = buff;
	agn->size = nopoll_msg_get_payload_size(agn->msg);
	audioenc_stop(agn->audioenc);
	if (agn->cb) {
		pf("callback\n");
		agn->cb(agn->usrdata, agn->message, agn->size);
	} else {
		pf("callback NULL\n");	
	}

	if (agn->audioenc) {
		audioenc_delete(agn->audioenc);
	}

	/*unref message*/
	nopoll_msg_unref(agn->msg);

	/*finish connection*/
	nopoll_conn_close(agn->conn);
	
	/*finish*/
	nopoll_ctx_unref(agn->ctx);
	return 0;
}

int aiengine_delete(struct aiengine *agn)
{
	free(agn->cfg);
	free(agn->provision_path);
	free(agn);
	return 0;
}

int aiengine_cancel(struct aiengine *engine)
{
	return 0;
}

int check_provision(struct aiengine *agn)
{
	int ret;
	if (agn == NULL)
		pf("agn NULL\n");

	pf("provision path: %s\n", agn->provision_path);
	ret = auth_do(agn->provision_path);
	return ret;
}
