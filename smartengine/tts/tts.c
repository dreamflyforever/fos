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

/* Converts a hex character to its integer value */
char from_hex(char ch)
{
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code)
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str)
{
	char *pstr = str;
	char *buf = malloc(strlen(str) * 3 + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) ||
				*pstr == '-' ||
				*pstr == '_' ||
				*pstr == '.' ||
				*pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ') 
			*pbuf++ = '+';
		else 
			*pbuf++ = '%',
				*pbuf++ = to_hex(*pstr >> 4),
				*pbuf++ = to_hex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str)
{
	char *pstr = str;
	char *buf = malloc(strlen(str) + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4
					| from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}
/**
 * Create random UUID
 *
 * @param buf - buffer to be filled with the uuid string
 */
char *random_uuid( char buf[37] )
{
	const char *c = "89ab";
	char *p = buf;
	int n;

	for( n = 0; n < 16; ++n )
	{
		int b = rand()%255;

		switch( n )
		{
		case 6:
			sprintf(
			    p,
			    "4%x",
			    b%15 );
			break;
		case 8:
			sprintf(
			    p,
			    "%c%x",
			    c[rand()%strlen( c )],
			    b%15 );
			break;
		default:
			sprintf(
			    p,
			    "%02x",
			    b );
			break;
		}

		p += 2;

		switch( n )
		{
		case 3:
		case 5:
		case 7:
		case 9:
			*p++ = '-';
			break;
		}
	}

	*p = 0;

	return buf;
}

char *tts_url_output(char *cfg, char *text)
{
	char *appkey;
	char *secretkey;
	char timestamp[11] = {0};
	char *userid;
	int samplebytes;
	int samplerate;
	int channel;
	int speechvolume;
	char *coretype;
	float speechrate;
	int rightmargin;
	int realback;
	char *res; char *server;
	char *port;
	char *audiotype;
	char *buf = malloc(100);
	memset(buf, 0, 100);
	if ((cfg == NULL) & (text == NULL)) {
		printf("please make sure cfg and text is right!");
		return NULL;
	}

	cJSON *root = cJSON_Parse(cfg);
	cJSON *tmp = cJSON_GetObjectItem(root, "appKey");
	appkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "secretKey");
	secretkey = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "userId");
	userid = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "coretype");
	coretype = tmp->valuestring;

	tmp = cJSON_GetObjectItem(root, "cloud");
	cJSON *t = cJSON_GetObjectItem(tmp, "server");
	server = t->valuestring;
	t = cJSON_GetObjectItem(tmp, "port");
	port = t->valuestring;

	tmp = cJSON_GetObjectItem(root, "audio");
	t = cJSON_GetObjectItem(tmp, "sampleBytes");
	samplebytes = t->valueint;
	t = cJSON_GetObjectItem(tmp, "sampleRate");
	samplerate = t->valueint;
	t = cJSON_GetObjectItem(tmp, "channel");
	channel = t->valueint;
	t = cJSON_GetObjectItem(tmp, "audioType");
	audiotype = t->valuestring;

	tmp = cJSON_GetObjectItem(root, "request");
	t = cJSON_GetObjectItem(tmp, "speechVolume");
	speechvolume = t->valueint;
	t = cJSON_GetObjectItem(tmp, "speechRate");
	speechrate = t->valueint;
	t = cJSON_GetObjectItem(tmp, "rightMargin");
	rightmargin = t->valueint;
	t = cJSON_GetObjectItem(tmp, "realBack");
	realback = t->valueint;
	t = cJSON_GetObjectItem(tmp, "res");
	res = t->valuestring;

	unsigned int sec = time(NULL);
	itoa(sec, timestamp); 
	uuid_t uuid;
    	char authid[36] = {0};
    	uuid_generate(uuid);
	uuid_unparse(uuid, authid);
	sprintf(buf, "%s\n%s\n%s\n%s", appkey, timestamp, secretkey, authid);
	char *sig = hmac_sha1(secretkey, buf);
	char *head = malloc(150);
	memset(head, 0, 150);
	sprintf(head, "applicationId=%s&timestamp=%s&authId=%s&sig=%s&params=",
			appkey, timestamp, authid, sig);
	char *buff = malloc(strlen(text) + 300);
	memset(buff, 0, (int)strlen(text) + 300);
	sprintf(buff, "{"
				"\"audio\": "
					"{\"sampleBytes\": %d, "
					"\"sampleRate\": %d, "
					"\"channel\": %d, "
					"\"audioType\": \"%s\", "
					"\"compress\": \"raw\""
				"}, "
				"\"request\": "
					"{"
					"\"speechVolume\": 100, "
					"\"coreType\": \"%s\", "
					"\"realBack\": %d, "
					"\"res\": \"%s\",\"refText\": \"%s\""
				"}"
			"}",
			samplebytes, samplerate, channel, audiotype,
			coretype, realback, res, text);
	char *tt = url_encode(buff);
	char *b = malloc(strlen(tt) + strlen(head) + 1);
	memset(b, 0, strlen(tt) + strlen(head) + 1);
	sprintf(b, "%s%s", head, tt);
	char *url = malloc(1024 + strlen(b));
	sprintf(url, "http://%s:%s/%s/%s?%s",
		server, port, coretype, res, b);

	cJSON_Delete(root);
	free(tt);
	free(head);
	free(sig);
	free(buf);
	free(buff);
	free(b);
	return url;
}
