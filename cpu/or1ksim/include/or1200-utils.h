#ifndef _OR1200_UTILS_H_
#define _OR1200_UTILS_H_

/* type defines */
typedef unsigned int          uint;
#if defined (__GNUC__)
    typedef unsigned char     bool;
    typedef signed char       int8;
    typedef signed short      int16;
    typedef signed long       int32;
    typedef unsigned char     uint8;
    typedef unsigned short    uint16;
    typedef unsigned long     uint32;
    typedef unsigned char     char8;
    typedef unsigned short    char16;
#else
    #error Undefined compiler used !
#endif

/* nullpointer is defined if not already done */
#ifndef NULL
    #define NULL          (void *)0
#endif

/* define min and max for all types */
#define INT8_MAX      0x7F
#define UINT8_MAX     0xFF
#define INT16_MAX     0x7FFF
#define UINT16_MAX    0xFFFF
#define INT32_MAX     0x7FFFFFFF
#define UINT32_MAX    0xFFFFFFFF

#ifndef  TRUE
#define FALSE 0
#define TRUE  !FALSE
#endif

/* l.nop constants */
#define NOP_NOP         0x0000      /* Normal nop instruction */
#define NOP_EXIT        0x0001      /* End of simulation */
#define NOP_REPORT      0x0002      /* Simple report */
#define NOP_PRINTF      0x0003      /* Simprintf instruction */
#define NOP_PUTC        0x0004      /* Simulation putc instruction */
#define NOP_REPORT_FIRST 0x0400     /* Report with number */
#define NOP_REPORT_LAST  0x03ff     /* Report with number */

#endif /* _OR1200_UTILS_H_ */
