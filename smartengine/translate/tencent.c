#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>
#include <agn_hmac_sha1.h>
#include <ctype.h>
#include <time.h>
#include "base.h"
#include <uuid/uuid.h>

char *tran_output(int from, int to, char *s)
{
	char *message = malloc(2048);
	memset(message, 0, 2048);
#if 0
	char *appkey = "1500886067859698";
	char *secretkey = "3f8133582992427f7b041441a05e43dd";
	char timestamp[11] = {0};
	unsigned int sec = time(NULL);
	char *buf = malloc(100);
	memset(buf, 0, 100);

	itoa(sec, timestamp); 
	uuid_t uuid;
    	char authid[36] = {0};
    	uuid_generate(uuid);
	uuid_unparse(uuid, authid);
	sprintf(buf, "%s\n%s\n%s\n%s", appkey, timestamp, secretkey, authid);
	char *sig = hmac_sha1(secretkey, buf);
#endif
	char *ue = url_encode(s);
#if 0
	sprintf(message,
	"http://112.80.39.95:9003/data/translate?sig=%s&appKey=%s&timestamp=%s&authId=%s&q=%s&from=%s&to=%s&source=tencent",
	sig, appkey, timestamp, authid, ue, from, to);
#endif
	sprintf(message,
	"http://112.80.39.95:9003/data/translate?__internal__=1&q=%s&from=en&to=zh&source=tencent",
	ue);

	printf("\n\n%s\n\n", message);
	char *str = http_get(message);
	free(message);
#if 0
	free(buf);
	free(sig);
#endif
	free(ue);
	return str;
}
