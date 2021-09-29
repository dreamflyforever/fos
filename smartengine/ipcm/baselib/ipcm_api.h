#include "mongoose.h"
#define SUB_COUNT 10

typedef int (*sub_cb)(int fd, char *data, int len);
struct sub_str {
	char *topic;
	sub_cb cb;
};

struct node_str {
	int sub_index;
	struct sub_str sub_obj[SUB_COUNT];
	struct mg_connection *mgc;
	struct mg_mgr mgr;         // Event manager
	bool state;

};
extern struct node_str node_obj;

int sub(char *topic, sub_cb cb);
int pub(char *topic, char *data, int len);
int mqtt_init(void);
int mqtt_deinit(void);
