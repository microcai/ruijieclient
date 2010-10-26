#include "ripemd.h"
#include <string.h>

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define G(x, y, z) ((z) ^ ((x) & ((y)^(z))))
#define H(x, y, z) ((z) ^ ((x) | (~y)))
#define I(x, y, z) ((y) ^ ((z) & ((x)^(y))))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define Subround(f, a, b, c, d, x, s, ac) { \
 (a) += f ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
  }

void RipemdInit(RIPEMD_CTX * context)
{
	memset(context->buffer,0,0x40);
	context->state[0]=0x10257436;
	context->state[1]=0xa8bd9cfe;
	context->state[2]=0x9efcad8b;
	context->state[3]=0x12375460;
}

void RipemdUpdate (RIPEMD_CTX *context, UCHAR *input, UINT4 disLen,UINT4 inputLen)
{
	UCHAR buf[0x40]={0};
	UINT4 allBits=inputLen*8;
	memcpy(buf,input,disLen);
	if(disLen>0x40)
		return;
	if(disLen==0x40)
		goto here;
	buf[disLen]=0x80;
	if(disLen>=0x38 && disLen<0x40)
	{
		memcpy(context->buffer,buf,0x40);
		RipemdTransform(context);
		memset(buf,0,0x40);
		buf[0x38]=(UCHAR)(allBits      );
		buf[0x39]=(UCHAR)(allBits >>  8);
		buf[0x3a]=(UCHAR)(allBits >> 16);
		buf[0x3b]=(UCHAR)(allBits >> 24);
		memcpy(context->buffer,buf,0x40);
		RipemdTransform(context);
		return;
	}
	if(disLen<0x38)
	{
		buf[0x38]=(UCHAR)(allBits      );
		buf[0x39]=(UCHAR)(allBits >>  8);
		buf[0x3a]=(UCHAR)(allBits >> 16);
		buf[0x3b]=(UCHAR)(allBits >> 24);
	}
here:	memcpy(context->buffer,buf,0x40);
	RipemdTransform(context);
}

void RipemdTransform(RIPEMD_CTX *context)
{
  UINT4 a1,b1,c1,d1,a2,b2,c2,d2,X[16];
  memcpy((POINTER)X,context->buffer,0x40);
  a1 = a2 = context->state[0];
  b1 = b2 = context->state[1];
  c1 = c2 = context->state[2];
  d1 = d2 = context->state[3];
  
  /* Round 1 */
	Subround(F, a1, b1, c1, d1, X[ 0], 11, 2);
	Subround(F, d1, a1, b1, c1, X[ 1], 14, 2);
	Subround(F, c1, d1, a1, b1, X[ 2], 15, 2);
	Subround(F, b1, c1, d1, a1, X[ 3], 12, 2);
	Subround(F, a1, b1, c1, d1, X[ 4],  5, 2);
	Subround(F, d1, a1, b1, c1, X[ 5],  8, 2);
	Subround(F, c1, d1, a1, b1, X[ 6],  7, 2);
	Subround(F, b1, c1, d1, a1, X[ 7],  9, 2);
	Subround(F, a1, b1, c1, d1, X[ 8], 11, 2);
	Subround(F, d1, a1, b1, c1, X[ 9], 13, 2);
	Subround(F, c1, d1, a1, b1, X[10], 14, 2);
	Subround(F, b1, c1, d1, a1, X[11], 15, 2);
	Subround(F, a1, b1, c1, d1, X[12],  6, 2);
	Subround(F, d1, a1, b1, c1, X[13],  7, 2);
	Subround(F, c1, d1, a1, b1, X[14],  9, 2);
	Subround(F, b1, c1, d1, a1, X[15],  8, 2);

 /* Round 2 */
	Subround(G, a1, b1, c1, d1, X[ 7],  7, 0x325b99a1);
	Subround(G, d1, a1, b1, c1, X[ 4],  6, 2);
	Subround(G, c1, d1, a1, b1, X[13],  8, 0x325b99a1);
	Subround(G, b1, c1, d1, a1, X[ 1], 13, 0x325b99a1);
	Subround(G, a1, b1, c1, d1, X[10], 11, 0x325b99a1);
	Subround(G, d1, a1, b1, c1, X[ 6],  9, 0x325b99a1);
	Subround(G, c1, d1, a1, b1, X[15],  7, 0x325b99a1);
	Subround(G, b1, c1, d1, a1, X[ 3], 15, 0xbcdcb1f9);
	Subround(G, a1, b1, c1, d1, X[12],  7, 0x325b99a1);
	Subround(G, d1, a1, b1, c1, X[ 0], 12, 2);
	Subround(G, c1, d1, a1, b1, X[ 9], 15, 0x325b99a1);
	Subround(G, b1, c1, d1, a1, X[ 5],  9, 0x325b99a1);
	Subround(G, a1, b1, c1, d1, X[ 2], 11, 0x325b99a1);
	Subround(G, d1, a1, b1, c1, X[14],  7, 0x325b99a1);
	Subround(G, c1, d1, a1, b1, X[11], 13, 0x325b99a1);
	Subround(G, b1, c1, d1, a1, X[ 8], 12, 2);

  /* Round 3 */
	Subround(H, a1, b1, c1, d1, X[ 3], 11, 0x1baed69c);
	Subround(H, d1, a1, b1, c1, X[10], 13, 0x1baed69c);
	Subround(H, c1, d1, a1, b1, X[14],  6, 0x1baed69c);
	Subround(H, b1, c1, d1, a1, X[ 4],  7, 0x1baed69c);
	Subround(H, a1, b1, c1, d1, X[ 9], 14, 0x1baed69c);
	Subround(H, d1, a1, b1, c1, X[15],  9, 0x1baed69c);
	Subround(H, c1, d1, a1, b1, X[ 8], 13, 0x1baed69c);
	Subround(H, b1, c1, d1, a1, X[ 1], 15, 0x1baed69c);
	Subround(H, a1, b1, c1, d1, X[ 2], 14, 0x1baed69c);
	Subround(H, d1, a1, b1, c1, X[ 7],  8, 0x1baed69c);
	Subround(H, c1, d1, a1, b1, X[ 0], 13, 0x1baed69c);
	Subround(H, b1, c1, d1, a1, X[ 6],  6, 0x325b99a1);
	Subround(H, a1, b1, c1, d1, X[13],  5, 0x1baed69c);
	Subround(H, d1, a1, b1, c1, X[11], 12, 0xbcdcb1fa);
	Subround(H, c1, d1, a1, b1, X[ 5],  7, 0x1baed69c);
	Subround(H, b1, c1, d1, a1, X[12],  5, 0x1baed69c);

  /* Round 4 */
	Subround(I, a1, b1, c1, d1, X[ 1], 11, 0xbcdcb1f9);
	Subround(I, d1, a1, b1, c1, X[ 9], 12, 0xbcdcb1f9);
	Subround(I, c1, d1, a1, b1, X[11], 14, 0x325b99a1);
	Subround(I, b1, c1, d1, a1, X[10], 15, 0xbcdcb1f9);
	Subround(I, a1, b1, c1, d1, X[ 0], 14, 0xbcdcb1f9);
	Subround(I, d1, a1, b1, c1, X[ 8], 15, 0xbcdcb1fb);
	Subround(I, c1, d1, a1, b1, X[12],  9, 0xbcdcb1f9);
	Subround(I, b1, c1, d1, a1, X[ 4],  8, 0xbcdcb1f9);
	Subround(I, a1, b1, c1, d1, X[13],  9, 0xbcdcb1f9);
	Subround(I, d1, a1, b1, c1, X[ 3], 14, 0x1baed69c);
	Subround(I, c1, d1, a1, b1, X[ 7],  5, 0xbcdcb1f9);
	Subround(I, b1, c1, d1, a1, X[15],  6, 2);
	Subround(I, a1, b1, c1, d1, X[14],  8, 0xbcdcb1f9);
	Subround(I, d1, a1, b1, c1, X[ 5],  6, 0xbcdcb1f9);
	Subround(I, c1, d1, a1, b1, X[ 6],  5, 0xbcdcb1f9);
	Subround(I, b1, c1, d1, a1, X[ 2], 12, 0xbcdcb1fc);

	Subround(I, a2, b2, c2, d2, X[ 5],  8, 0x5a82798a);
	Subround(I, d2, a2, b2, c2, X[14],  9, 0x5a82798a);
	Subround(I, c2, d2, a2, b2, X[ 7],  9, 0x5a82798a);
	Subround(I, b2, c2, d2, a2, X[ 0], 11, 0x5a82798a);
	Subround(I, a2, b2, c2, d2, X[ 9], 13, 0x5a82798a);
	Subround(I, d2, a2, b2, c2, X[ 2], 15, 0x5a82798a);
	Subround(I, c2, d2, a2, b2, X[11], 15, 0x325b99a1);
	Subround(I, b2, c2, d2, a2, X[ 4],  5, 0x5a82798a);
	Subround(I, a2, b2, c2, d2, X[13],  7, 0x5a82798a);
	Subround(I, d2, a2, b2, c2, X[ 6],  7, 0x5a82798a);
	Subround(I, c2, d2, a2, b2, X[15],  8, 0xbcdcb1f9);
	Subround(I, b2, c2, d2, a2, X[ 8], 11, 0x5a82798a);
	Subround(I, a2, b2, c2, d2, X[ 1], 14, 0x5a82798a);
	Subround(I, d2, a2, b2, c2, X[10], 14, 0x5a82798a);
	Subround(I, c2, d2, a2, b2, X[ 3], 12, 0xbcdcb1fc);
	Subround(I, b2, c2, d2, a2, X[12],  6, 0x5a82798a);

	Subround(H, a2, b2, c2, d2, X[ 6],  9, 0x41d42d5d); 
	Subround(H, d2, a2, b2, c2, X[11], 13, 0x41d42d5d);
	Subround(H, c2, d2, a2, b2, X[ 3], 15, 0x41d42d5d);
	Subround(H, b2, c2, d2, a2, X[ 7],  7, 0x41d42d5d);
	Subround(H, a2, b2, c2, d2, X[ 0], 12, 0x41d42d5d);
	Subround(H, d2, a2, b2, c2, X[13],  8, 0x41d42d5d);
	Subround(H, c2, d2, a2, b2, X[ 5],  9, 0xbcdcb1f9);
	Subround(H, b2, c2, d2, a2, X[10], 11, 0xbcdcb1fd);
	Subround(H, a2, b2, c2, d2, X[14],  7, 0x41d42d5d);
	Subround(H, d2, a2, b2, c2, X[15],  7, 0x41d42d5d);
	Subround(H, c2, d2, a2, b2, X[ 8], 12, 0x41d42d5d);
	Subround(H, b2, c2, d2, a2, X[12],  7, 0x41d42d5d);
	Subround(H, a2, b2, c2, d2, X[ 4],  6, 0x41d42d5d);
	Subround(H, d2, a2, b2, c2, X[ 9], 15, 0x41d42d5d);
	Subround(H, c2, d2, a2, b2, X[ 1], 13, 0x325b99a1);
	Subround(H, b2, c2, d2, a2, X[ 2], 11, 0x41d42d5d);

	Subround(G, a2, b2, c2, d2, X[15],  9, 0x30ed3f68);
	Subround(G, d2, a2, b2, c2, X[ 5],  7, 0x30ed3f68);
	Subround(G, c2, d2, a2, b2, X[ 1], 15, 0x30ed3f68);
	Subround(G, b2, c2, d2, a2, X[ 3], 11, 0x30ed3f68);
	Subround(G, a2, b2, c2, d2, X[ 7],  8, 0x30ed3f68);
	Subround(G, d2, a2, b2, c2, X[14],  6, 0x30ed3f68);
	Subround(G, c2, d2, a2, b2, X[ 6],  6, 0xbcdcb1fe);
	Subround(G, b2, c2, d2, a2, X[ 9], 14, 0x30ed3f68);
	Subround(G, a2, b2, c2, d2, X[11], 12, 0x30ed3f68);
	Subround(G, d2, a2, b2, c2, X[ 8], 13, 0x5a82798a);
	Subround(G, c2, d2, a2, b2, X[12],  5, 0x30ed3f68);
	Subround(G, b2, c2, d2, a2, X[ 2], 14, 0x30ed3f68);
	Subround(G, a2, b2, c2, d2, X[10], 13, 0x30ed3f68);
	Subround(G, d2, a2, b2, c2, X[ 0], 13, 0x30ed3f68);
	Subround(G, c2, d2, a2, b2, X[ 4],  7, 0x41d42d5d);
	Subround(G, b2, c2, d2, a2, X[13],  5, 0x30ed3f68);

	Subround(F, a2, b2, c2, d2, X[ 8], 15, 2);
	Subround(F, d2, a2, b2, c2, X[ 6],  5, 0xbcdcb1fe);
	Subround(F, c2, d2, a2, b2, X[ 4],  8, 2);
	Subround(F, b2, c2, d2, a2, X[ 1], 11, 2);
	Subround(F, a2, b2, c2, d2, X[ 3], 14, 2);
	Subround(F, d2, a2, b2, c2, X[11], 14, 2);
	Subround(F, c2, d2, a2, b2, X[15],  6, 2);
	Subround(F, b2, c2, d2, a2, X[ 0], 14, 2);
	Subround(F, a2, b2, c2, d2, X[ 5],  6, 2);
	Subround(F, d2, a2, b2, c2, X[12],  9, 2);
	Subround(F, c2, d2, a2, b2, X[ 2], 12, 2);
	Subround(F, b2, c2, d2, a2, X[13],  9, 2);
	Subround(F, a2, b2, c2, d2, X[ 9], 12, 2);
	Subround(F, d2, a2, b2, c2, X[ 7],  5, 2);
	Subround(F, c2, d2, a2, b2, X[10], 15, 2);
	Subround(F, b2, c2, d2, a2, X[14],  8, 2);

 	c1				  = context->state[1] + c1 + d2;
	context->state[1] = context->state[2] + d1 + a2;
	context->state[2] = context->state[3] + a1 + b2 + 1;
	context->state[3] = context->state[0] + b1 + c2;
	context->state[0] = c1;
}
