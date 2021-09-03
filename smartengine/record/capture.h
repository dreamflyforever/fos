#include <stdio.h>
#include <alsa/asoundlib.h>

struct record_str{
	snd_pcm_t *handle;  
	snd_pcm_hw_params_t *params;  
	int dir;  
	snd_pcm_uframes_t frames;  
};

int record_init();
int record_deinit();

extern struct record_str rec_obj;
