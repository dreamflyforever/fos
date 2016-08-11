#ifndef __AGN_HMAC_SHA1_H__
#define __AGN_HMAC_SHA1_H__
#ifdef __cplusplus
extern "C"{
#endif

int agn_hmac_sha1(const char *key, int key_len, const char *text, int text_len, char out[40]);
char *hmac_sha1(char *key, char *text);
#ifdef __cplusplus
}
#endif
#endif
