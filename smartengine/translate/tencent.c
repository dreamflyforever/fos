#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>
#include <agn_hmac_sha1.h>
#include <ctype.h>
#include <time.h>
#include "base.h"
#include <uuid/uuid.h>
#include <agn_audioenc.h>

char *tran_output(int from, int to, char *s)
{
	char *message = malloc(2048);
	memset(message, 0, 2048);
	char *appkey = "14709983278595d8";
	char *secretkey = "85d1e668eace0ce6539c299aa02b2334";
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
	char *ue = url_encode(s);
	sprintf(message, "http://112.80.39.95:9003/data/translate?sig=%s&appKey=%s&timestamp=%s&authId=%s&q=%s&from=%s&to=%s&source=tencent", sig, appkey, timestamp, authid, ue, from, to);
	printf("\n\n%s\n\n", message);
	char *str = http_get(message);
	//printf("%s:%s\n", __func__, str);
	free(message);
	free(buf);
	free(ue);
	free(sig);
	return str;
}

char *fetch_key(unsigned char const *start, char *key, unsigned long length)
{
	char *p = strstr(start, key);
	if (p == NULL) return NULL;
	char *url = malloc(2048);
	memset(url, 0, 2048);
	int i = 0;
	int len = strlen(key);
	p = p + len + 3;
	/*TODO: add size judge*/
	while (!(p[i] == '"')) {
		url[i] = p[i];
		if (p[i] == "\0") break;
		i++;
	}

	return url;
}

char *tran_output_parse(char *s)
{
	char *c = fetch_key(s, "dst", strlen(s));
	return c;
}
