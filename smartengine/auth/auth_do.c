#include "agn_provision.h"
#include <string.h>
#include <stdio.h>
#include <http.h>
#include <aiengine.h>

static int provision_http_request(void *userdata,
				const char *url,
				char response[1024])
{
	int ret;
	char *body = http_get(url);
	while (body == NULL) {
		body = http_get(url);
		sleep(1);
		printf("try do auth......\n");
	}
	ret = strlen(body);
	printf("body:len %s\n", body);
	memcpy(response, body, strlen(body));
	printf("url: %s\n", url);
	return ret;
}

int auth_do(char *path)
{
	char buf[1024];
	int read_count;
	FILE *file_fd = fopen(path, "r");
	if (file_fd == NULL) {
		printf("open file error.\n");
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	read_count = fread(buf, 1, 1024, file_fd);
	if (read_count <= 0) {
		printf("file NULL char\n");
	}
	int ret = agn_provision_check_auth(buf, NULL);
	if (ret != 0) {
		printf("no serialnumber, maybe create one\n");
	}

	pf("check provision ret:%d\n", ret);
	ret = agn_provision_do_auth(buf, NULL, provision_http_request, NULL);
	fclose(file_fd);
	pf("check provision ret:%d\n", ret);
	return ret;
}
