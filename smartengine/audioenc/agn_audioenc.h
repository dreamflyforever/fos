#ifndef AGN_AUDIOENC_H_
#define AGN_AUDIOENC_H_

#include "third/ogg/ogg.h"
#include "third/speex/speex.h"
#include "third/speex/speex_header.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*audioenc_callback_f)(void *user_data, unsigned char *body, int body_len, unsigned char *head, int head_len);

typedef struct audioenc_cfg
{
	int spx_quality;
	int spx_complexity;
	int spx_vbr;
}agn_audioenc_cfg_t;

typedef struct audioenc 
{
	const SpeexMode  *spx_mode;
	void       *spx_state;
	SpeexBits   spx_bits;
	SpeexHeader spx_header;
	spx_int32_t spx_frame_size;
	spx_int32_t spx_lookahead;

	spx_int32_t spx_quality;
	spx_int32_t spx_rate;
	spx_int32_t spx_channels;
	spx_int32_t spx_bits_per_sample;
	spx_int32_t spx_frames_per_packet;
	spx_int32_t spx_vbr;
	spx_int32_t spx_complexity;

	int         spx_frame_id;
	int         spx_frame_id_pageout;   /* 4 pageout per second */

	char       *spx_version;
	char        spx_vendor[64];
	char       *spx_comments;
	char        spx_bits_buf[2048];

	spx_int16_t spx_frame_buf[1024];
	int         spx_frame_buf_pos;


	ogg_stream_state og_stream_state;
	ogg_page         og_page;
	ogg_packet       og_packet;

	float            og_pages_per_second;
	void             *user_data;

	volatile int status;
	audioenc_callback_f audioenc_data_notify;
}agn_audioenc_s;

struct audioenc* audioenc_new(void *data, audioenc_callback_f cb); 
int audioenc_delete(struct audioenc *enc);
void audioenc_stop(struct audioenc *enc);
int audioenc_start(struct audioenc *enc, int rate, int channels, int bits, agn_audioenc_cfg_t *cfg); 
int audioenc_encode(struct audioenc *enc, const void *data, int size);

int audioenc_notify(void *user_data,
			unsigned char *body,
			int body_len,
			unsigned char *head,
			int head_len);
#if 0
#define pf(format, ...) \
	{printf("[%s : %s : %d] ", \
	__FILE__, __func__, __LINE__); \
	printf(format, ##__VA_ARGS__);}
#else
#define pf(format, ...) 
#endif

#ifdef __cplusplus
}
#endif
#endif
