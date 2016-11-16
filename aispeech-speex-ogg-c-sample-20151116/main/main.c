#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <agn_audioenc.h>
#include <nopoll.h>
#include <aiengine.h>
#include <pthread.h>

char *server_cfg = "{\
	\"appKey\": \"14709983278595d8\",\
	\"secretKey\": \"85d1e668eace0ce6539c299aa02b2334\",\
	\"provision\": \"auth/config.json\",\
	\"serialNumber\": \"bin/serialNumber\", \
	\"audiotype\": \"ogg\",\
	\"coretype\": \"cn.sds\",\
	\"res\": \"aihome\",\
	\"app\": {\
		\"userId\": \"wifiBox\"\
	},\
	\"cloud\": {\
		\"server\": \"s-test.api.aispeech.com\",\
		\"port\": \"10000\"\
	}\
}";

char *cloud_asr_param = "{\
	\"coreProvideType\": \"cloud\",\
	\"audio\": {\
	    \"audioType\": \"ogg\",\
	    \"sampleRate\": 16000,\
	    \"channel\": 1,\
	    \"compress\":\"raw\",\
	    \"sampleBytes\": 2\
	},\
	\"request\": {\
		\"coreType\": \"cn.sds\",\
		\"speechRate\":1.0,\
		\"res\": \"airobot\"\
	},\
	\"sdsExpand\":{\
		\"prevdomain\":\"\",\
		\"lastServiceType\": \"cloud\"\
		}\
}";

static FILE *wav;
struct aiengine *agn;

int agn_cb(const void *usrdata,
		const void *message,
		int size)
{
	if (message == NULL) {
		pf("cb message NULL\n");
	}

	printf("message:%s, size:%d\n", (char *)message, size);
	return 0;
}

extern int cloud_auth_do();

int main(int argc, char *argv[])
{
	char *wavpath = "wether.wav";
	int bytes;
	char buf[3200] = {0};
	int ret;
	agn = aiengine_new(server_cfg);
	cloud_auth_do();
	if (agn == NULL) {
		pf("error\n");
		return 0;
	}

	ret = check_provision(agn);;
	if (ret != 0) {
		printf("Authorization fail\n");
	} else {
		printf("Authorization success\n");
	}

	aiengine_start(agn, cloud_asr_param, agn_cb, NULL);
	wav = fopen(wavpath, "r");
	if (!wav) {
		printf("open wav : %s failed\n", wavpath);
		return 0;
	}

	fseek(wav, 44, SEEK_SET);
	while ((bytes = fread(buf, 1, sizeof(buf), wav))) {
		aiengine_feed(agn, buf, bytes);
	}

	fclose(wav);
	aiengine_stop(agn);

	aiengine_delete(agn);
	sleep(1);
	return 0;
}
