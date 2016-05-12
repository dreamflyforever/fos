/*******************************************************************************
 * Copyright (C), 2008-2013, AISpeech Tech. Co., Ltd.
 * 
 * FileName    : main.c
 * Author      : hongbin.liu
 * Date        : Tue 27 Aug 2013 08:07:42 PM CST
 * Description : speex test module
 *------------------------------------------------------------------------------*
 * Record      : Add nopoll library to send data to server, and return ASR.
	       : 20.4.2016
	       : modify by Shanjin.Yang
 		
 *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "agn_audioenc.h"
#include <nopoll.h>
#include "aiengine.h"

static FILE *ogg;
static FILE *wav;
struct aiengine *agn;

#define print(format, ...) \
	{printf("[%s : %s : %d] ", __FILE__, __func__, __LINE__);\
	printf(format, ##__VA_ARGS__);}

static int audioenc_notify(void *user_data,
			unsigned char *body,
			int body_len,
			unsigned char *head,
			int head_len)
{
#ifdef SAVE_OGG
	fwrite(head, 1, head_len, ogg);
	fwrite(body, 1, body_len, ogg);
#endif
	if (agn->conn == NULL) {
		print("conn == NULL\n");
		return 0;
	}

	nopoll_conn_send_binary(agn->conn, (const char *)head, head_len);
	nopoll_sleep(10000);

	nopoll_conn_send_binary(agn->conn, (const char *)body, body_len);
	nopoll_sleep(10000);
	return 0;
}

static void audioenc_wav(char *wavpath, agn_audioenc_s *audioenc)
{
	int bytes;
	char oggpath[1024] = {0};
	char buf[3200]  = {0};

	sprintf(oggpath, "%.*s.ogg", (int)strlen(wavpath) - 4, wavpath);

	wav = fopen(wavpath, "r");
	if (!wav) {
		printf("open wav : %s failed\n", wavpath);
		goto end;
	}

#ifdef SAVE_OGG
	ogg = fopen(oggpath, "w");
	if (!ogg) {
		printf("open ogg : %s failed\n", oggpath);
		goto end;
	}
#endif
	fseek(wav, 44, SEEK_SET);
	agn_audioenc_cfg_t *cfg = NULL;
	cfg = (agn_audioenc_cfg_t *)calloc(1, sizeof(*cfg));
	if (!cfg) {
		printf("calloc cfg failed!\n");
		goto end;	
	} else {
		cfg->spx_quality = 8;
		cfg->spx_complexity = 2;
		cfg->spx_vbr = 0;
	}
	audioenc_start(audioenc, 16000, 1, 16, cfg);
	while ((bytes = fread(buf, 1, sizeof(buf), wav))) {
		audioenc_encode(audioenc, buf, bytes);
	}
	audioenc_stop(audioenc);

	printf("converted %s to %s\n", wavpath, oggpath);
end:
	if (wav) {
		fclose(wav);
		wav = NULL;
	}

	if (ogg) {
		fclose(ogg);
		ogg = NULL;
	} 
	if (cfg) {
		free(cfg);
		cfg = NULL;		
	}
}

int audioenc_output(char *file)
{
	agn_audioenc_s *audioenc = NULL;
	audioenc = audioenc_new(NULL, audioenc_notify);
	audioenc_wav(file, audioenc);

	if (audioenc) {
		audioenc_delete(audioenc);
	}
	return 0;
}

static char *cloud_syn_param = "{\
	\"coreProvideType\": \"cloud\",\
        \"audio\": {\
            \"audioType\": \"ogg\",\
            \"sampleRate\": 16000,\
            \"channel\": 1,\
	    \"compress\":\"raw\",\
            \"sampleBytes\": 2\
        },\
        \"request\": {\
		\"coreType\": \"cn.dlg.ita\",\
		\"speechRate\":1.0,\
		\"res\": \"xm_aihome\"\
        }\
}";

int main(int argc, char *argv[])
{
	agn = aiengine_new(NULL);
	if (agn == NULL) {
		printf("%s %s %d: error\n", __FILE__, __func__, __LINE__);
	}

	int ret = check_provision(agn);;
	if (ret != 0) {
		printf("Authorization fail\n");
		return 0;
	} else {
		printf("Authorization success\n");
	}
	aiengine_start(agn, cloud_syn_param, NULL, NULL, NULL);
	audioenc_output("1.wav");
	/*raw send data API*/
	nopoll_conn_send_frame(agn->conn, 1, 1, 2, 0, "", 0);

	while ((agn->msg = nopoll_conn_get_msg(agn->conn)) == NULL) {
		if (! nopoll_conn_is_ok (agn->conn)) {
			printf ("ERROR: received websocket connection close during wait reply..\n");
			return nopoll_false;
		}
		nopoll_sleep(10000);
	}
	printf("msg received \n");
	char buf[1024 * 1024];
	memcpy(buf, (char *)nopoll_msg_get_payload (agn->msg), nopoll_msg_get_payload_size(agn->msg));
	printf("%s", buf);
	/*unref message*/
	nopoll_msg_unref(agn->msg);

	/*finish connection*/
	nopoll_conn_close(agn->conn);
	
	/*finish*/
	nopoll_ctx_unref(agn->ctx);

	return 0;
}
