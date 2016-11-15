#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Converts a hex character to its integer value */
char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str) {
	char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
			*pbuf++ = *pstr;
		else if (*pstr == ' ') 
			*pbuf++ = '+';
		else 
			*pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str) {
	char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
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

char *tts_url_output(char *param)
{
	char url[2048] = {0};
	char buff[2048] = {0};
	memset(url, 0, 2048);
	int len;

	char *text = url_encode("抱歉没找到刘德华的歌，请问您想听什么歌呢？");
	sprintf(buff, "applicationId=146337845885959a&timestamp=1470721766&sig=c0bf68677e3324a4a9f5e35bbaa7b7e126b15dec&params={\"app\": {\"userId\": \"wifiBox\"}, \"audio\": {\"sampleBytes\": 2, \"sampleRate\": 16000, \"channel\": 1, \"audioType\": \"mp3\"}, \"request\": {\"speechVolume\": 50, \"coreType\": \"cn.sent.syn\", \"speechRate\": 0.85, \"rightMargin\": 5, \"realBack\": 1, \"res\": \"syn_chnsnt_zhilingf\",\"refText\": \"%s\"}}", text);
	len = strlen(buff);
	sprintf(url, "http:/112.80.39.95:8009/cn.sent.syn/syn_chnsnt_zhilingf?%s", buff);
	printf("\n%s\n", url);

}
#if 0
	sprintf(buff, "applicationId=146337845885959a&timestamp=1470721766&authId=57a96ee68827ad006a000001&sig=c0bf68677e3324a4a9f5e35bbaa7b7e126b15dec&params={\"app\": {\"userId\": \"wifiBox\"}, \"audio\": {\"sampleBytes\": 2, \"sampleRate\": 16000, \"compress\": \"raw\", \"channel\": 1, \"audioType\": \"mp3\"}, \"request\": {\"speechVolume\": 50, \"coreType\": \"cn.sent.syn\", \"speechRate\": 0.85, \"rightMargin\": 5, \"realBack\": 1, \"res\": \"syn_chnsnt_zhilingf\", \"leftMargin\": 5, \"refText\": \"%s\"}}", text);
#endif
int main()
{
	tts_url_output(NULL);
	return 0;
}
