/**************************************************************
 *                                                            *
 *   Fichier : tiger.h                                        *
 *   Header de tiger.c                                        *
 *                                                            *
 **************************************************************/

#ifndef ___TIGER_H___
#define ___TIGER_H___
#include <sys/types.h>

#ifdef _MSC_VER
typedef unsigned __int64 ulong64;
#define CONST64(v)   (v##i64)
#else  /* !_MSC_VER */
typedef unsigned long long ulong64;
#define CONST64(v)   (v##ULL)
#endif /* ?_MSC_VER */

#ifndef NULL
#define NULL 0
#endif

#ifndef LOAD64L
#define LOAD64L(x, y)                                                       \
{ x = (((ulong64)((y)[7] & 255))<<56)|(((ulong64)((y)[6] & 255))<<48)| \
	(((ulong64)((y)[5] & 255))<<40)|(((ulong64)((y)[4] & 255))<<32)| \
	(((ulong64)((y)[3] & 255))<<24)|(((ulong64)((y)[2] & 255))<<16)| \
(((ulong64)((y)[1] & 255))<<8)|(((ulong64)((y)[0] & 255))); }
#define STORE64L(x, y)                                                                   \
{ (y)[7] = (unsigned char)(((x)>>56)&255); (y)[6] = (unsigned char)(((x)>>48)&255); \
	(y)[5] = (unsigned char)(((x)>>40)&255); (y)[4] = (unsigned char)(((x)>>32)&255); \
	(y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255); \
(y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }
#endif

#ifndef MIN
#define MAX(x, y) (((x)>(y))?(x):(y))
#define MIN(x, y) (((x)<(y))?(x):(y))
#endif

typedef struct _tiger_hash_state
{
	ulong64 state[3];
	unsigned char buf[64];
} TIGER_CTX;

void TIGER_Init(TIGER_CTX *md);
void TIGER_Update(TIGER_CTX *context, UCHAR *input, UINT4 disLen,UINT4 inputLen);
void _tiger_compress(TIGER_CTX *md);

#endif /* ___TIGER_H___ */
