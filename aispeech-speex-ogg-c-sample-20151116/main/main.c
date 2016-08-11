#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <agn_audioenc.h>
#include <nopoll.h>
#include <aiengine.h>

//\"cloud\": {\
//		\"server\": \"s-test.api.aispeech.com\",\
//		\"port\": \"10000\"\
//	},

#if 1
char *server_cfg = "{\
	\"appKey\": \"14327742440003c5\",\
	\"secretKey\": \"59db7351b3790ec75c776f6881b35d7e\",\
	\"provision\": \"auth/config.json\",\
	\"serialNumber\": \"bin/serialNumber\", \
	\"audiotype\": \"ogg\",\
	\"coretype\": \"cn.dlg.ita\",\
	\"res\": \"aihome\",\
	\"app\": {\
        	\"userId\": \"wifiBox\"\
    	},\
	\"cloud\": {\
		\"server\": \"112.80.39.95\",\
		\"port\": \"8009\"\
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
		\"coreType\": \"cn.dlg.ita\",\
		\"speechRate\":1.0,\
		\"res\": \"aihome\"\
        }\
}";
#else

char *server_cfg = "{\
	\"appKey\": \"14327742440003c5\",\
	\"secretKey\": \"59db7351b3790ec75c776f6881b35d7e\",\
	\"provision\": \"auth/config.json\",\
	\"serialNumber\": \"bin/serialNumber\", \
	\"audiotype\": \"ogg\",\
	\"coretype\": \"cn.sds\",\
	\"res\": \"aihome\",\
	\"app\": {\
        	\"userId\": \"wifiBox\"\
    	},\
	\"cloud\": {\
		\"server\": \"112.80.39.95\",\
		\"port\": \"8009\"\
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
		\"res\": \"aihome\"\
        },\
	\"sdsExpand\":{\
		\"prevdomain\":\"\",\
		\"lastServiceType\": \"cloud\"\
		}\
}";
#endif
static FILE *wav;
struct aiengine *agn;

int agn_cb(const void *usrdata,
		const void *message,
		int size)
{
	if (message == NULL) {
		pf("cb message NULL\n");
	}

	pf("message:%s, size:%d\n", (char *)message, size);
	return 0;
}

int main(int argc, char *argv[])
{
	char *wavpath = "wether.wav";
	int bytes;
	char buf[3200]  = {0};
	int ret;
start:
	while (1) {
		agn = aiengine_new(server_cfg);
		if (agn == NULL) {
			printf("%s %s %d: error\n", __FILE__, __func__, __LINE__);
			return 0;
			goto start;
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
	}
	return 0;
}
