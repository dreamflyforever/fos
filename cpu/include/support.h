#ifndef _SUPPORT_H_
#define _SUPPORT_H_


/* include common defines for or1200 */
#include "or1200-utils.h"

/* Max and min functions */
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define MAX(a,b) (((a)>(b))?(a):(b))
	
/* Register access macros */
#define REG8(add) *((volatile unsigned char *)(add))
#define REG16(add) *((volatile unsigned short *)(add))
#define REG32(add) *((volatile unsigned long *)(add))

/* For writing into SPR. */
void mtspr(unsigned long spr, unsigned long value);

/* For reading SPR. */
unsigned long mfspr(unsigned long spr);



#endif /* _SUPPORT_H_ */
