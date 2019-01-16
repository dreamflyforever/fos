#ifndef _AIENGINE_H_
#define _AIENGINE_H_

#include <nopoll.h>
#include <agn_audioenc.h>

#define DUI 1
#define WECHAT 0
#define use_pcm 0
#define use_free_server 0

typedef int (*aiengine_callback)(const void *usrdata,
		const void *message,
		int size);

struct aiengine {
	noPollCtx *ctx;
	noPollMsg *msg;
	char *provision_path;
	noPollConn *conn;
	agn_audioenc_s *audioenc;
	agn_audioenc_cfg_t *cfg;
	aiengine_callback cb;
	const void *usrdata;
	int size;
	int provision_ok;
	char *userid;
#if DUI
	char session[40];
#endif
};

struct aiengine *aiengine_new(const char *cfg);
int aiengine_delete(struct aiengine *engine);
int aiengine_start(struct aiengine *engine,
			const char *param,
			aiengine_callback callback,
			const void *usrdata);

int aiengine_feed(struct aiengine *engine, const void *data, int size);
int aiengine_stop(struct aiengine *engine);
int aiengine_cancel(struct aiengine *engine);
int check_provision(struct aiengine *agn);
#if DUI
char *aiengine_get_dui_session(struct aiengine *agn);
int aiengine_set_dui_session(struct aiengine *agn, char *buf, int size);
int dui_result_process(char *buf, int size,
	char *output_speakurl, char *output_linkurl);
char *player_url(char *buf);
#endif
#endif
