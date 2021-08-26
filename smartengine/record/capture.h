/* Use the newer ALSA API */  
#include <stdio.h>
#include <alsa/asoundlib.h>

int record_init();
struct record_str{
	snd_pcm_t *handle;  
	snd_pcm_hw_params_t *params;  
	int dir;  
	snd_pcm_uframes_t frames;  
};

extern struct record_str rec_obj;
