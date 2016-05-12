#include <nopoll.h>

struct aiengine {
	noPollCtx *ctx;
	noPollMsg *msg;
	char *provision_path;
	noPollConn *conn;
};

typedef int (*aiengine_callback)(const void *usrdata, const char *id, int type, const void *message, int size);
struct aiengine *aiengine_new(const char *cfg);
int aiengine_delete(struct aiengine *engine);
int aiengine_start(struct aiengine *engine, const char *param, char id[64], aiengine_callback callback, const void *usrdata);
int aiengine_feed(struct aiengine *engine, const void *data, int size);
int aiengine_stop(struct aiengine *engine);
int aiengine_cancel(struct aiengine *engine);
int check_provision(struct aiengine *agn);
