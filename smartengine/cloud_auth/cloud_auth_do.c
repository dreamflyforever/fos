#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <http.h>
#include <sha1.c>
#include <time.h>
#include <unistd.h>
#include <cJSON.h>
#include "base.h"

int cloud_auth_do(const char *cfg)
{
	cJSON *root = cJSON_Parse(cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "appKey");
	char *appKey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "secretKey");
	char *secretKey = tmp->valuestring;

	char *sig_in = malloc(80);
	memset(sig_in, 0, 80);
	int size;

	char *deviceId = "8c705ac0a268";
	/*XXX: maybe need the system time*/
	char timestamp[12] = {0};
	int sec= time(NULL);
	itoa(sec, timestamp);
	//printf("sec: %d, timestamp: %s\n", sec, timestamp);
	//sprintf(timestamp, "%d", (int)rand());

	memset(sig_in, 0, sizeof(1024));
	sprintf(sig_in, "%s%s%s%s", appKey, timestamp, secretKey, deviceId);
	char sig[41] = {0};
	memset(sig, 0, sizeof(sig));
	size = strlen(sig_in);

	sha1(sig_in, size, sig);
	char url[1024];
	memset(url, 0, sizeof(url));
	sprintf(url,
		"http://%s?appKey=%s&timestamp=%s&deviceId=%s&userId=MXCHIP&sig=%s",
		"auth.api.aispeech.com/device",
		appKey,
		timestamp,
		deviceId,
		sig);
	printf("%s\n", url);
	char *str = http_get(url);
	while (str == NULL) {
		str = http_get(url);
		sleep(1);
		printf("%s try do auth......\n", __func__);
	}
	if ((strstr(str, "error") != NULL) &&
		(strstr(str, "frequent") == NULL)) {
		/*XXX: notice maybe need to know the return result of http get*/
		printf("provision error\n");
	} else {
		printf("provision success\n");
	}

	printf("return:%s\n", str);
	free(str);
	free(sig_in);
	cJSON_Delete(root);
	return 0;
}
