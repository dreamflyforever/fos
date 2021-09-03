/* 
   This example reads from the default PCM device 
   and writes to standard output  of data. 
 */

#include <capture.h>
#include <stdlib.h>

/* Use the newer ALSA API */  
struct record_str rec_obj;

int record_init()
{
	int rc = 0;
	int val;
	/* Open PCM device for recording (capture). */  
	rc = snd_pcm_open(&rec_obj.handle, "default",  
			SND_PCM_STREAM_CAPTURE, 0);  
	if (rc < 0) {  
		fprintf(stderr,
			"unable to open pcm device: %s/n",
			snd_strerror(rc));  
		exit(1);  
	}  
	/* Allocate a hardware parameters object. */  
	snd_pcm_hw_params_alloca(&rec_obj.params);  
	/* Fill it in with default values. */  
	snd_pcm_hw_params_any(rec_obj.handle, rec_obj.params);  
	/* Set the desired hardware parameters. */  
	/* Interleaved mode */  
	snd_pcm_hw_params_set_access(rec_obj.handle, rec_obj.params,
			SND_PCM_ACCESS_RW_INTERLEAVED);
	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(rec_obj.handle, rec_obj.params,  
			SND_PCM_FORMAT_S16_LE);
	/* one channels */
	snd_pcm_hw_params_set_channels(rec_obj.handle, rec_obj.params, 1);
	/* 16000 bits/second sampling rate (CD quality) */
	val = 16000;  
	snd_pcm_hw_params_set_rate_near(rec_obj.handle,
			rec_obj.params, &val, &rec_obj.dir);

	/* Set period size to 32 frames. */  
	rec_obj.frames = 32;
	snd_pcm_hw_params_set_period_size_near(rec_obj.handle,
					rec_obj.params,
					&rec_obj.frames, &rec_obj.dir);

	/* Write the parameters to the driver */  
	rc = snd_pcm_hw_params(rec_obj.handle, rec_obj.params);  
	if (rc < 0) {  
		fprintf(stderr,  "unable to set hw parameters: %s/n",  
				snd_strerror(rc));  
		exit(1);  
	}  
	/* Use a buffer large enough to hold one period */  
	//snd_pcm_hw_params_get_period_size(rec_obj.params,  &rec_obj.frames, &rec_obj.dir);

	//snd_pcm_hw_params_get_period_time(rec_obj.params,  &val, &rec_obj.dir);
	return 0;
}

int record_deinit()
{
	int val, val2;
	int frames;
#if 0
	/******************打印参数*********************/
	snd_pcm_hw_params_get_channels(rec_obj.params, &val);  
	printf("channels = %d\n", val);  
	snd_pcm_hw_params_get_rate(rec_obj.params, &val, &rec_obj.dir);  
	printf("rate = %d bps\n", val);  
	snd_pcm_hw_params_get_period_time(rec_obj.params,  
			&val, &rec_obj.dir);  
	printf("period time = %d us\n", val);  
	snd_pcm_hw_params_get_period_size(rec_obj.params,  
			(snd_pcm_uframes_t *)&frames, &rec_obj.dir);  
	printf("period size = %d frames\n", (int)frames);  
	snd_pcm_hw_params_get_buffer_time(rec_obj.params,  
			&val, &rec_obj.dir);  
	printf("buffer time = %d us\n", val);  
	snd_pcm_hw_params_get_buffer_size(rec_obj.params,  
			(snd_pcm_uframes_t *) &val);  
	printf("buffer size = %d frames\n", val);  
	snd_pcm_hw_params_get_periods(rec_obj.params, &val, &rec_obj.dir);  
	printf("periods per buffer = %d frames\n", val);  
	snd_pcm_hw_params_get_rate_numden(rec_obj.params,
			&val, &val2);  
	printf("exact rate = %d/%d bps\n", val, val2);  
	val = snd_pcm_hw_params_get_sbits(rec_obj.params);  
	printf("significant bits = %d\n", val);  
	//snd_pcm_hw_params_get_tick_time(params,  &val, &dir);  
	printf("tick time = %d us\n", val);  
	val = snd_pcm_hw_params_is_batch(rec_obj.params);  
	printf("is batch = %d\n", val);  
	val = snd_pcm_hw_params_is_block_transfer(rec_obj.params);  
	printf("is block transfer = %d\n", val);  
	val = snd_pcm_hw_params_is_double(rec_obj.params);  
	printf("is double = %d\n", val);  
	val = snd_pcm_hw_params_is_half_duplex(rec_obj.params);  
	printf("is half duplex = %d\n", val);  
	val = snd_pcm_hw_params_is_joint_duplex(rec_obj.params);  
	printf("is joint duplex = %d\n", val);  
	val = snd_pcm_hw_params_can_overrange(rec_obj.params);  
	printf("can overrange = %d\n", val);  
	val = snd_pcm_hw_params_can_mmap_sample_resolution(rec_obj.params);  
	printf("can mmap = %d\n", val);  
	val = snd_pcm_hw_params_can_pause(rec_obj.params);
	printf("can pause = %d\n", val);
	val = snd_pcm_hw_params_can_resume(rec_obj.params);  
	printf("can resume = %d\n", val);  
	val = snd_pcm_hw_params_can_sync_start(rec_obj.params);  
	printf("can sync start = %d\n", val);  
	/*******************************************************************/
#endif
	snd_pcm_drop(rec_obj.handle);
	snd_pcm_close(rec_obj.handle); 
	return 0;
}

#if 0
int main()
{
	long loops;  
	int rc;
	int size;  
	FILE *fp ;
	char *buffer;
start:
	system("rm sound.pcm");
	record_init();
	if ((fp = fopen("sound.pcm","w")) < 0)
		printf("open sound.pcm fial\n");

	size = rec_obj.frames * 2; /* 2 bytes/sample, 1 channels */  

	printf(">>>>>>>>>>>>>>>record start<<<<<<<<<<<<<<<\n");
	//printf("size = %d\n",size);
	buffer = (char *) malloc(size);  
	loops = 1120;
	while (loops > 0) {  
		loops--;  
		rc = snd_pcm_readi(rec_obj.handle, buffer, rec_obj.frames); 
		if (rc == -EPIPE) {  
			/* EPIPE means overrun */  
			fprintf(stderr, "overrun occurred/n");  
			snd_pcm_prepare(rec_obj.handle);  
		} else if (rc < 0) {  
			fprintf(stderr,  
					"error from read: %s/n",  
					snd_strerror(rc));  
		} else if (rc != (int)rec_obj.frames) {  
			fprintf(stderr, "short read, read %d frames/n", rc);  
		}  
		rc = fwrite( buffer,1, size,fp);  
		if (rc != size)  
			fprintf(stderr,  "short write: wrote %d bytes/n", rc);  
	}
	record_deinit();
	printf(">>>>>>>>>>>>>>>record end<<<<<<<<<<<<<<<\n");
	fclose(fp); 
	free(buffer);
	printf(">>>>>>>>>>>>>>>player start<<<<<<<<<<<<<<<\n");
	system("ffplay -f s16le -ar 16000 -ac 1 -autoexit sound.pcm");
	printf(">>>>>>>>>>>>>>>player end<<<<<<<<<<<<<<<\n");
	goto start;
	return 0;
}
#endif
