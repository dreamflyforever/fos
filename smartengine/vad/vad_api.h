#include <stdio.h>
#include "wb_vad.h"

#define PASS 0
#define FAIL -1
#define SILENT -1

#define DEBUG 0

#if DEBUG
#define vad_log(format, ...) \
	{printf("[%s : %s : %d] ", \
	__FILE__, __func__, __LINE__); \
	printf(format, ##__VA_ARGS__);}
#else
#define vad_log(format, ...) 
#endif

typedef VadVars vad_str;

/*When silent it will call the cb function to notify user*/
int vad_init(vad_str **entity, VAD_CB cb, void *arg);

/*set vad detect time*/
int vad_settime(vad_str *entity, float ms);

/*audio input*/
int vad_feed(vad_str *entity, char *data, int len);

int vad_delete(vad_str **entity);
