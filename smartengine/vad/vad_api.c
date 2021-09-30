#include <string.h>
#include "vad_api.h"

typedef int (*VAD_CB)(void *arg, int flag);
int vad_init(vad_str **entity, VAD_CB cb, void *arg)
{
	int retval = PASS;
	if (entity == NULL) {
		retval = FAIL;
		vad_log("entity NULL\n");
		goto out;
	}
	if (PASS != wb_vad_init(entity)) {
		vad_log("fail\n");
		retval = FAIL;
		goto out;
	}
	if (cb == NULL) {
		retval = FAIL;
		goto out;
	}
	(*entity)->cb = cb;
	(*entity)->arg = arg;
out:
	return retval;
}
int vad_settime(vad_str *entity, float ms)
{
	int retval = PASS;
	if (entity == NULL) {
		retval = FAIL;
		vad_log("entity NULL\n");
		goto out;
	}

	entity->millisecond = ms;
out:
	return PASS;
}

int vad_feed(vad_str *entity, char *buf, int len)
{
	/*frames = 16k * 2B  * t/ 1000, t = 7.8ms*/
	int frames = FRAME_LEN;
	static int size;
	int i;
	int temp;
	/*for vad*/
	float indata[FRAME_LEN];   
	short outdata[FRAME_LEN];
	int retval = PASS;
	int vad_flag;
	if (entity == NULL) {
		retval = FAIL;
		vad_log("entity NULL\n");
		goto out;
	}
	//for(i = 0; i< frames; i++) {
	for(i = 0; i< len; i++) {
		indata[i]=0;
		temp = 0;
		memcpy(&temp, buf + 2 * i, 2);
		indata[i]=(short)temp;
		outdata[i]=temp;
		if (indata[i] > 65535/2)
			indata[i] = indata[i]-65536;
	}
	vad_flag = wb_vad(entity, indata);
	if (vad_flag == 1) {
		size = 0;
	} else {
		if (size < entity->millisecond) {
			vad_log("size %d\n", size);
			size++;
		} else {
			if (entity->cb == NULL) {
				vad_log("vad user callback is NULL\n");
				retval = FAIL;
				goto out;
			}
			/*user callback*/
			entity->cb(entity->arg, PASS);
			retval = SILENT;
			size = 0;
		}
	}
out:
	return retval;
}

int vad_delete(vad_str **entity)
{
	int retval = PASS;
	wb_vad_exit(entity);
	return retval;
}
