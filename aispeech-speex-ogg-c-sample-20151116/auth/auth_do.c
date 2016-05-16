#include "agn_provision.h"
#include <string.h>
#include <stdio.h>
#include <http.h>

static int provision_http_request(void *userdata, const char *url, char response[1024])
{
	char *body = http_get(url);
	printf("body: %s\n", body);
	memcpy(response, body, strlen(body));
	printf("url: %s\n", url);
	return 0;
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

	ret = agn_provision_do_auth(buf, NULL, provision_http_request, NULL);
	fclose(file_fd);

	return ret;
}
