/*******************************************************************************
* Copyright (C), 2008-2013, AISpeech Tech. Co., Ltd.
* 
* FileName    : test_audioenc.c
* Author      : hongbin.liu
* Date        : Tue 27 Aug 2013 08:07:42 PM CST
* Description : speex压缩测试模块 
*------------------------------------------------------------------------------*
* Record      : 
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "agn_audioenc.h"

static FILE *ogg;
static FILE *wav;

/* #define SAVE_OGG */

static int _test_audioenc_notify(void *user_data, unsigned char *body, int body_len, unsigned char *head, int head_len)
{
#ifdef SAVE_OGG
    fwrite(head, 1, head_len, ogg);
    fwrite(body, 1, body_len, ogg);
#endif
    return 0;
}


static void _test_audioenc_wav(char *wavpath, agn_audioenc_s *audioenc)
{
    int bytes;
    char oggpath[1024] = {0};
    char buf[3200]  = {0};

    sprintf(oggpath, "%.*s.ogg", (int)strlen(wavpath) - 4, wavpath);

    wav = fopen(wavpath, "r");
    if (!wav) {
        printf("open wav : %s failed\n", wavpath);
        goto end;
    }

#ifdef SAVE_OGG
    ogg = fopen(oggpath, "w");
    if (!ogg) {
        printf("open ogg : %s failed\n", oggpath);
        goto end;
    }
#endif

    fseek(wav, 44, SEEK_SET);
    agn_audioenc_cfg_t *cfg = NULL;
	cfg = (agn_audioenc_cfg_t *)calloc(1, sizeof(*cfg));
	if(!cfg) {
		printf("calloc cfg failed!\n");
		goto end;	
	} else {
		cfg->spx_quality = 8;
    	cfg->spx_complexity = 2;
    	cfg->spx_vbr = 0;
	}
    audioenc_start(audioenc, 16000, 1, 16, cfg);
    while((bytes = fread(buf, 1, sizeof(buf), wav))) {
        audioenc_encode(audioenc, buf, bytes);
    }
    audioenc_stop(audioenc);

    printf("converted %s to %s\n", wavpath, oggpath);

end:
    if (wav) {
        fclose(wav);
        wav = NULL;
    }

    if (ogg) {
        fclose(ogg);
        ogg = NULL;
    } 
	if(cfg) {
		free(cfg);
		cfg = NULL;		
	}
}


int test_audioenc(int argc, char *argv)
{
    char line[1024];
    FILE *file = NULL;
    agn_audioenc_s *audioenc = NULL;


    if (argc < 2) {
        printf("usage: %s wav.scp\n", argv);
        goto end;
    }
 
    file = fopen(argv, "r");
    if (!file) {
        printf("can not open file : %s\n", argv);
        goto end;
    }
    
    printf("start");
    audioenc = audioenc_new(NULL, _test_audioenc_notify);
    while (fgets(line, sizeof(line), file)) {

        char *p;

        p = strrchr(line, '\n');
        if (p) {
            *p = '\0';
        }

        p = strrchr(line, '\r');
        if (p) {
            *p = '\0';
        } 

        _test_audioenc_wav(line, audioenc);
    }
    printf("end");

end:
    if (audioenc) {
        audioenc_delete(audioenc);
    }

    if (file) {
        fclose(file);
    }
 
    return 0;
}

#if 0
int main(int argc, char** argv)
{
	test_audioenc(argc, argv);
	
	return 0;		
}
#endif
