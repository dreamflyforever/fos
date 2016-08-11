#include "agn_hmac_sha1.h"
#include "sha1.c"
#include <string.h>
#include <stdlib.h>

#define SHA_DIGESTSIZE (20)
#define SHA_BLOCKSIZE  (64)

int agn_hmac_sha1(const char *key, int key_len, const char *text, int text_len, char out[40])
{
	if (key == NULL || text == NULL || out == NULL) return -1;
	unsigned char k_ipad[SHA_BLOCKSIZE]; /*inner padding, key XORd with ipad*/
	unsigned char k_opad[SHA_BLOCKSIZE]; /*outer padding, key XORd with opad*/
	unsigned char digest[SHA_DIGESTSIZE];
	int len = text_len + SHA_BLOCKSIZE;
	unsigned char *content = (unsigned char *)malloc(sizeof(unsigned char) * len);
	if(content == NULL) return -1;
	memset(k_ipad, 0x00, sizeof(k_ipad));
	memset(k_opad, 0x00, sizeof(k_opad));
	memset(digest, 0x00, sizeof(digest));
	memset(content, 0x00, len);    

	struct sha1_context sha;
	if (key_len > SHA_BLOCKSIZE) {
		sha1_init(&sha);
		sha1_update(&sha, (unsigned char *)key, key_len);
		sha1_final(&sha, digest);
		memcpy(k_ipad, digest, SHA_DIGESTSIZE);
		memcpy(k_opad, digest, SHA_DIGESTSIZE);
	} else {
		memcpy(k_ipad, key, key_len);
		memcpy(k_opad, key, key_len);
	}
	/*XOR key with ipad and opad values*/
	int i = 0;
	for (i = 0; i < SHA_BLOCKSIZE; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	/*first sha1*/
	memcpy(content, k_ipad, SHA_BLOCKSIZE);
	memcpy(content + SHA_BLOCKSIZE, text, text_len);
	sha1_init(&sha);
	sha1_update(&sha, (unsigned char *)content, len);
	sha1_final(&sha, digest);

	/*second sha1*/
	memcpy(content, k_opad, SHA_BLOCKSIZE);
	memcpy(content + SHA_BLOCKSIZE, digest, SHA_DIGESTSIZE);
	sha1(content, SHA_BLOCKSIZE + SHA_DIGESTSIZE, out);
	free(content);
	return 0;
}


/*attention: must free the digest in your application*/
char *hmac_sha1(char *k, char *text)
{
	char *digest = malloc(512);
	memset(digest, 0, 512);
	int key_len = strlen(k);
	char *key = (char *)malloc(sizeof(char) * key_len) ;   
	if (key == NULL) {
		printf("ERROR: malloc for key error.\n");
		return 0;
	}
	memcpy(key, k, key_len);
	agn_hmac_sha1(key, key_len, text, strlen(text), digest);
	//printf("out: %.*s\n", (int)sizeof(digest),digest);
	free(key);
	return digest;
}
