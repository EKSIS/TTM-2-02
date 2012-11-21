#ifndef MSP430_COMMON
#define MSP430_COMMON
typedef double          Flo64;    // Double precision floating point
typedef const double    cFlo64;    // Double precision floating point
typedef double        * pFlo64;
typedef float           Flo32;    // Single precision floating point
typedef const float     cFlo32;    // Single precision floating point
typedef float         * pFlo32;
typedef signed  long    Int32S;   // Signed   32 bit quantity
typedef const signed  long    cInt32S;   // Signed   32 bit quantity
typedef signed  long  * pInt32S;
typedef unsigned long   Int32U;   // Unsigned 32 bit quantity
typedef const unsigned long   cInt32U;   // Unsigned 32 bit quantity
typedef unsigned long * pInt32U;  
typedef signed   int    Int16S;   // Signed   32 bit quantity
typedef const signed   int    cInt16S;   // Signed   32 bit quantity
typedef signed   int  * pInt16S;
typedef unsigned int    Int16U;   // Unsigned 32 bit quantity
typedef const unsigned int    cInt16U;   // Unsigned 32 bit quantity
typedef unsigned int  * pInt16U;
typedef signed   char   Int8S;    // Signed    8 bit quantity
typedef const signed   char   cInt8S;    // Signed    8 bit quantity
typedef signed   char * pInt8S;
typedef unsigned char   Int8U;    // Unsigned  8 bit quantity
typedef const unsigned char   cInt8U;    // Unsigned  8 bit quantity
typedef unsigned char * pInt8U;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define _2BL(a)   (Int8U)(a),(Int8U)(a>>8)
#define _2BB(a)   (Int8U)(a>>8),(Int8U)(a),
#define _3BL(a)   (Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16)
#define _3BB(a)   (Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)
#define _4BL(a)   (Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16),(Int8U)(a>>24)
#define _4BB(a)   (Int8U)(a>>24),(Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)

#endif
