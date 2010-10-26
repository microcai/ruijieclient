#ifndef RIPEMD_H
#define RIPEMD_H

#include "types.h"

typedef struct
{
	UINT4 state[4];	
	UCHAR buffer[64];
} RIPEMD_CTX;

void RipemdInit(RIPEMD_CTX * context);
void RipemdUpdate (RIPEMD_CTX *context, UCHAR *input, UINT4 disLen,UINT4 inputLen);
void RipemdTransform(RIPEMD_CTX *context);
#endif
