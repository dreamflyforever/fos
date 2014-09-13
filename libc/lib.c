#include "include/lib.h"

/* Simple C functions */

/* memcpy */

void* memcpy( void* s1, void* s2, size_t n)
{
  char* r1 = (char *) s1;
  const char* r2 =  (const char*) s2;
#ifdef __BCC__
  while (n--) {
    *r1++ = *r2++;
  }
#else
  while (n) {
    *r1++ = *r2++;
    --n;
  }
#endif
  return s1;
}

/* strlen */
size_t strlen(const char*s)
{
  const char* p;
  for (p=s; *p; p++);
  return p - s;
}

/* memchr */
void *memchr(const void *s, int c, size_t n)
{
         const unsigned char *r = (const unsigned char *) s;
#ifdef __BCC__
        /* bcc can optimize the counter if it thinks it is a pointer... */
        const char *np = (const char *) n;
#else
# define np n
#endif

        while (np) {
                if (*r == ((unsigned char)c)) {
                        return (void *) r;     /* silence the warning */
                }
                ++r;
                --np;
        }

        return NULL;
}

/* --------------------------------------------------------------------------*/
/*!Pseudo-random number generator

   This should return pseudo-random numbers, based on a Galois LFSR

   @return The next pseudo-random number                                     */
/* --------------------------------------------------------------------------*/
unsigned long int
rand ()
{
  static unsigned long int lfsr = 0x14b6bc3c;
  static int period = 0;
  /* taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 */
  lfsr = (lfsr >> 1) ^ (unsigned long int)((0 - (lfsr & 1u)) & 0xd0000001u); 
  ++period;
  return lfsr;
}

void *memset(void *src, unsigned char byte, unsigned int count)
{
	char *xs = src;

	while (count--) {
		
		*xs++ = byte;
	}
	
	return src;
}

#if 1
int strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}
#endif
