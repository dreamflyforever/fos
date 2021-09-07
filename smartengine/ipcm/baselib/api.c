#include <pthread.h>
#include "ipcm_api.h"

static pthread_mutex_t node_mutex;
#if 0
#define  node_lock()  pthread_mutex_lock(&node_mutex); printf("[%s %d]lock\n", __func__, __LINE__)
#define  node_unlock()  pthread_mutex_unlock(&node_mutex); printf("[%s %d]unlock\n", __func__, __LINE__)
#else
#define  node_lock()  pthread_mutex_lock(&node_mutex); 
#define  node_unlock()  pthread_mutex_unlock(&node_mutex);
#endif
static const char *s_url = "mqtt://0.0.0.0:1883";
static const char *s_topic = "mg/test";

struct node_str node_obj;
pthread_t _node_thread;

void *_node_loop(void *arg);
int pub(char *topic, char *data, int len)
{
	int retval;
	int i = 0;
	while (node_obj.state == false) {
		printf("[%s %d]no connect\n", __func__, __LINE__);
		sleep(1);
		i++;
		if (i >= 10) goto _OUT;
	}
	struct mg_str topic_str = mg_str(topic), data_n = mg_str_n(data, len);

	/*XXX:return*/
	node_lock();
	//printf("[%s %d] pub : %s\n", __func__, __LINE__, topic);
	mg_mqtt_pub(node_obj.mgc, &topic_str, &data_n);
	node_unlock();
_OUT:
	return retval;
}

int sub(char *topic, sub_cb cb)
{
	int retval;
	int len = strlen(topic);
	int i = 0;
	while (node_obj.state == false) {
		printf("[%s %d]no connect\n", __func__, __LINE__);
		sleep(1);
		i++;
		if (i >= 10) goto _OUT;
	}

	node_lock();
	node_obj.sub_obj[node_obj.sub_index].topic = malloc(len+1);
	strncpy(node_obj.sub_obj[node_obj.sub_index].topic, topic, len);
	node_obj.sub_obj[node_obj.sub_index].cb = cb;
	node_obj.sub_index++;
	if (node_obj.sub_index >= SUB_COUNT) {
		printf("error: sub topic number > %d\n", SUB_COUNT);
		exit(0);
	}
	struct mg_str topic_str = mg_str(topic);
	mg_mqtt_sub(node_obj.mgc, &topic_str);
	node_unlock();
_OUT:
	return retval;
}

/*block the process*/
int mqtt_init(void)
{
	int retval = 0;
	retval = pthread_mutex_init(&node_mutex, NULL);
	if (retval != 0) {
		printf("mutex init failed\n");
		return -1;
	}
	if (pthread_create(&_node_thread,
		NULL, _node_loop, NULL)
		!= 0) {
        	printf("Create thread error!\n");
        	exit(1);
	}
	printf("node thread create success\n");
	while (node_obj.state == false) sleep(1);
	return retval;
}

int mqtt_deinit(void)
{
	int i;
	node_obj.state = false;
	mg_mgr_free(&(node_obj.mgr));
	for (i = 0; i <= node_obj.sub_index; i++) {
		free(node_obj.sub_obj[i].topic);
	}
	/*destroy node thread*/
	//pthread_cancel(_node_thread)
	pthread_mutex_destroy(&node_mutex);
	return 0;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
	//printf("enter fun, ev: %d\n", ev);
	if (ev == MG_EV_ERROR) {
		// On error, log error message
		LOG(LL_ERROR, ("%p %s", c->fd, (char *) ev_data));
	} else if (ev == MG_EV_CONNECT) {
		// If target URL is SSL/TLS, command client connection to use TLS
		if (mg_url_is_ssl(s_url)) {
			struct mg_tls_opts opts = {.ca = "ca.pem"};
			mg_tls_init(c, &opts);
		}
	} else if (ev == MG_EV_MQTT_OPEN) {
		node_obj.state = true;
		// MQTT connect is successful
		//struct mg_str topic = mg_str(s_topic), data = mg_str("hello");
		LOG(LL_INFO, ("CONNECTED to %s", s_url));
		//mg_mqtt_sub(c, &topic);
		//LOG(LL_INFO, ("SUBSCRIBED to %.*s", (int) topic.len, topic.ptr));
		//mg_mqtt_pub(c, &topic, &data);
		//LOG(LL_INFO, ("PUBSLISHED %.*s -> %.*s", (int) data.len, data.ptr,
		//			(int) topic.len, topic.ptr));
	} else if (ev == MG_EV_MQTT_MSG) {
		// When we get echo response, print it
		struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
		//LOG(LL_INFO, ("RECEIVED %.*s <- %.*s", (int) mm->data.len, mm->data.ptr,
		//			(int) mm->topic.len, mm->topic.ptr));
		int i;
		for (i = 0; i <= SUB_COUNT; i++) {
			if (node_obj.sub_obj[i].topic != NULL) {
				if (0 == strncmp(node_obj.sub_obj[i].topic, mm->topic.ptr,
					(strlen(node_obj.sub_obj[i].topic)-1))) {
					if (node_obj.sub_obj[i].cb != NULL) {
						node_obj.sub_obj[i].cb(0, (char *)mm->data.ptr, mm->data.len);
						goto _OUT;
					}
				}
			}
		}
	}

	if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE ) {
		printf("mqtt close\n");
		*(bool *) fn_data = true;  // Signal that we're done
	}
_OUT:
	return ;
}

void *_node_loop(void *arg)
{
	node_obj.state = false;
	struct mg_mqtt_opts opts;  // MQTT connection options
	bool done = false;         // Event handler flips it to true when done
	mg_mgr_init(&(node_obj.mgr));         // Initialise event manager
	memset(&opts, 0, sizeof(opts));                 // Set MQTT options
	opts.qos = 0;                                   // Set QoS to 1
	opts.will_topic = mg_str(s_topic);              // Set last will topic
	opts.will_message = mg_str("goodbye");          // And last will message
	node_obj.mgc = mg_mqtt_connect(&(node_obj.mgr), s_url, &opts, fn, &done);  // Create client connection
	while (done == false) {
		node_lock();
		mg_mgr_poll(&(node_obj.mgr), 100);  // Event loop
		node_unlock();
		usleep(1000);
	}
	node_obj.state = false;
	printf("exit node loop\n");
	pthread_exit((void*)0);
	mqtt_deinit();
	mqtt_init();
}
