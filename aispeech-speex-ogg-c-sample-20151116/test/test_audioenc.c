/*******************************************************************************
 * Copyright (C), 2008-2013, AISpeech Tech. Co., Ltd.
 * 
 * FileName    : test_audioenc.c
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

static FILE *ogg;
static FILE *wav;
noPollConn *conn;

char speex_buf[1024 * 1024 * 1024];
int speex_len;
/* #define SAVE_OGG */
#define print(format, ...) \
	{printf("[%s : %s : %d] ", __FILE__, __func__, __LINE__);\
	printf(format, ##__VA_ARGS__);}

static int _test_audioenc_notify(void *user_data, unsigned char *body, int body_len, unsigned char *head, int head_len)
{
#ifdef SAVE_OGG
	fwrite(head, 1, head_len, ogg);
	fwrite(body, 1, body_len, ogg);
#endif
#if 1
	if (conn == NULL) {
		print("conn == NULL\n");
		return 0;
	}

	nopoll_conn_send_binary(conn, head, head_len);
	nopoll_sleep(10000);

	nopoll_conn_send_binary(conn, body, body_len);
	nopoll_sleep(10000);
	printf("send data\n");
#endif
#if 0
	memcpy(buf, head, head_len);
	strncat(buf, body, body_len);
#endif
#if 0
	memcat(speex_buf, head, body, head_len, body_len);
	speex_len = head_len + body_len;
#endif
	return 0;
}

static void _test_audioenc_wav(char *wavpath, agn_audioenc_s *audioenc)
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
	if(!cfg) {
		printf("calloc cfg failed!\n");
		goto end;	
	} else {
		cfg->spx_quality = 8;
		cfg->spx_complexity = 2;
		cfg->spx_vbr = 0;
	}
	audioenc_start(audioenc, 16000, 1, 16, cfg);
	while((bytes = fread(buf, 1, sizeof(buf), wav))) {
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
	if(cfg) {
		free(cfg);
		cfg = NULL;		
	}
}

int audioenc_output(char *file)
{
	char line[1024];
	agn_audioenc_s *audioenc = NULL;
	audioenc = audioenc_new(NULL, _test_audioenc_notify);
	_test_audioenc_wav(file, audioenc);

end:
	if (audioenc) {
		audioenc_delete(audioenc);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int cx;
	noPollCtx  *ctx;
	noPollMsg  *msg;
	char timestamp[1024];

	char path[2048];
	char buf[1024 * 1024];

	char *appkey = "1458977484859577";
	char *secretkey = "d19c742b65bfe89857130819297ace0f";

	char *audiotype = "ogg";
	char *coretype = "cn.dlg.ita";
	char *res = "xm_aihome";
	char *host = "s.api.aispeech.com";
	char *port = "1028";
	char *authId = "1234567890993";
	char *sig = "c6c5bec781ef95acf49443074359bb61f7a1463e";
	char text[1024];

	if (argc < 2) {
		printf("usage: hello song.song\n");
		return 0;
	}
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

	/*init context*/
	ctx = nopoll_ctx_new ();
	if (!ctx) {
		printf("error nopoll new ctx failed!");
		exit(1);
	}

	/*create connection*/
	conn = nopoll_conn_new (ctx,
				host,
				port,
				NULL,
				path,
				NULL,
				NULL);

	if (!nopoll_conn_is_ok (conn)) {
		printf("[%s %s %d]: connect error\n",
		__FILE__, __func__, __LINE__);
		return nopoll_false;
	}

	if (!nopoll_conn_wait_until_connection_ready (conn, 5)) {
		printf("[%s %s %d]: connect not ready\n",
		__FILE__, __func__, __LINE__);
		return 0;
	} else {
		printf("[%s %s %d]: connect success\n",
		__FILE__, __func__, __LINE__);
	}

	memset(text, 0, 1024);
	sprintf(text, "{\"coreProvideType\": \"cloud\",\
		\"audio\": {\"audioType\": \"%s\", \"sampleBytes\": 2,\
		\"sampleRate\": 16000, \"channel\": 1, \"compress\":\"raw\"},\
		\"request\": {\"coreType\": \"%s\", \"res\": \"%s\"}}",
		audiotype, coretype, res);
	printf("%s\n", text);
	cx = nopoll_conn_send_text(conn, text, strlen(text));
	printf("[%s %s %d]: text len: %d\n",
		__FILE__, __func__, __LINE__, (int)strlen(text));
	if (cx < 0)
		printf("\n[%s %s %d]: send text error\n",
			__FILE__, __func__, __LINE__);
#if 0
	/*-------------------------------------------------------------------------*/
	/*read ogg file and send to server*/
	int rc, num;
	int len;
	char fd_buf[1034 * 3] = {0};

	/*open file in the format of binary*/
	FILE *fpin = fopen(argv[1], "rb");
	if (fpin == NULL) {
		printf("open error\n");
		return 0;
	}
	while ((rc = fread(fd_buf, 1, 1024 * 3, fpin)) != 0) {
		cx = nopoll_conn_send_binary(conn, fd_buf, rc);

		/*sleep for a while or will send failed*/
		nopoll_sleep(10000);

		printf("cx returned value %d \n", cx);
		if (cx < 0){
			printf("send binary error, break\n");
		}
	}
	fclose(fpin);
	/*-------------------------------------------------------------------------*/
#endif
	audioenc_output(argv[1]);
	/*modify source code if you want to use this API*/
	/*nopoll_conn_send_binary(conn, "", 0);*/
	/*raw API*/
	nopoll_conn_send_frame(conn, 1, 1, 2, 0, "", 0);

	while ((msg = nopoll_conn_get_msg (conn)) == NULL) {
		if (! nopoll_conn_is_ok (conn)) {
			printf ("ERROR: received websocket connection close during wait reply..\n");
			return nopoll_false;
		}
		nopoll_sleep(10000);
	}
	printf("msg received \n");
	memcpy(buf, (char *)nopoll_msg_get_payload (msg), nopoll_msg_get_payload_size (msg));
	printf("%s", buf);
	/*unref message*/
	nopoll_msg_unref (msg);

	/*finish connection*/
	nopoll_conn_close (conn);
	
	/*finish*/
	nopoll_ctx_unref (ctx);

	return 0;
}
