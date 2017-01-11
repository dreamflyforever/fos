#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <agn_audioenc.h>
#include <nopoll.h>
#include <aiengine.h>
#include <pthread.h>
#include <tts.h>
#if 0
	s-test.api.aispeech.com:10000

mico:
	appKey: 14709983278595d8
	secretKey: 85d1e668eace0ce6539c299aa02b2334

#endif

#if 0
/*ingenic*/
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
rtt:
	appKey\": \"14796952588595df\",\
	ecretKey\": \"1cd1349a6ad1fe31de37ad4a9005f626\",\

#else

char *server_cfg = "{\
	\"appKey\": \"14709983278595d8\",\
	\"secretKey\": \"85d1e668eace0ce6539c299aa02b2334\",\
	\"provision\": \"auth/config.json\",\
	\"serialNumber\": \"bin/serialNumber\", \
	\"audiotype\": \"pcm\",\
	\"coretype\": \"cn.sds\",\
	\"res\": \"airobot\",\
	\"app\": {\
		\"userId\": \"wifiBox\"\
	},\
	\"cloud\": {\
		\"server\": \"58.210.96.236\",\
		\"port\": \"8888\"\
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
#endif

#define MILLION 1000000
long clock_get()
{
	long ret;
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	ret = t.tv_sec * MILLION + t.tv_nsec/1000;
	return ret;
}

long calcu(long start, long end)
{
	long r = (end - start)/1000;
	return r;
}

long start;
long end;

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
	end = clock_get();
	long t = calcu(start, end);
	printf("time  interval: %ld milliseconds\n", t);
	return 0;
}

extern int cloud_auth_do(const char *cfg);

char *tts_param =  "{\
	\"appKey\": \"14796952588595df\",\
	\"secretKey\": \"1cd1349a6ad1fe31de37ad4a9005f626\",\
	\"userId\": \"wifiBox\",\
	\"coretype\": \"cn.sent.syn\",\
	\"cloud\": {\
		\"server\": \"58.210.96.236\",\
		\"port\": \"8888\"\
	},\
	\"audio\": {\
		\"sampleBytes\": 2,\
		\"sampleRate\": 16000,\
		\"channel\": 1, \
		\"audioType\": \"mp3\"\
	},\
	\"request\": {\
		\"speechVolume\": 50, \
		\"speechRate\": 0.85, \
		\"rightMargin\": 5, \
		\"realBack\": 0, \
		\"res\": \"syn_chnsnt_zhilingf\"\
	}\
}";

int main(int argc, char *argv[])
{
#if 1
	char *url;
	url = tts_url_output(tts_param,
			"抱歉没找到刘德华的歌，请问您想听什么歌呢？");
	printf("\n%s\n", url);
	free(url);
#endif
	char *wavpath = "wether.wav";
	int bytes;
	char buf[3200] = {0};
	agn = aiengine_new(server_cfg);
	//cloud_auth_do(server_cfg);
	if (agn == NULL) {
		pf("error\n");
		return 0;
	}
#if 0
	int ret;
	ret = check_provision(agn);;
	if (ret != 0) {
		printf("Authorization fail\n");
	} else {
		printf("Authorization success\n");
	}
#endif
	aiengine_start(agn, cloud_asr_param, agn_cb, NULL);
	wav = fopen(wavpath, "rb");
	if (!wav) {
		printf("open wav : %s failed\n", wavpath);
		return 0;
	}

	fseek(wav, 44, SEEK_SET);
	while ((bytes = fread(buf, 1, sizeof(buf), wav))) {
		aiengine_feed(agn, buf, bytes);
	}

	fclose(wav);
	start = clock_get();
	aiengine_stop(agn);

	aiengine_delete(agn);
	sleep(1);
	return 0;
}
