/**
 * The Whirlpool hashing function.
 *
 * The Whirlpool algorithm was developed by
 * Paulo S. L. M. Barreto and Vincent Rijmen.
 *
 * See
 *      P.S.L.M. Barreto, V. Rijmen,
 *      ``The Whirlpool hashing function,''
 *      NESSIE submission, 2000 (tweaked version, 2001),
 *      <https://www.cosic.esat.kuleuven.ac.be/nessie/workshop/submissions/whirlpool.zip>
 *
 * @version 3.0 (2003.03.12)
 *
 * Modified for use in this software package.
 */
#ifndef _WHIRLPOOL_ALGORITHM_H_
#define _WHIRLPOOL_ALGORITHM_H_

typedef signed char s8;
typedef unsigned char u8;

#if UINT_MAX >= 4294967295UL

typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

#define ONE32   0xffffffffU

#else

typedef signed int s16;
typedef signed long s32;
typedef unsigned int u16;
typedef unsigned long u32;

#define ONE32   0xffffffffUL

#endif

#ifdef _MSC_VER
typedef unsigned __int64 u64;
typedef signed __int64 s64;
#define LL(v)   (v##i64)
#define ONE64   LL(0xffffffffffffffff)
#else  /* !_MSC_VER */
typedef unsigned long long u64;
typedef signed long long s64;
#define LL(v)   (v##ULL)
#define ONE64   LL(0xffffffffffffffff)
#endif /* ?_MSC_VER */

#define WP_DIGEST_SIZE 64
#define DIGESTBYTES WP_DIGEST_SIZE
#define DIGESTBITS  (8*DIGESTBYTES) /* 512 */

#define WBLOCKBYTES 64
#define WBLOCKBITS  (8*WBLOCKBYTES) /* 512 */

#define LENGTHBYTES 32
#define LENGTHBITS  (8*LENGTHBYTES) /* 256 */

typedef struct WP_Struct {
	u64 hash[DIGESTBYTES/8];	/* the hashing state */
	u8  buffer[WBLOCKBYTES];	/* buffer of data to hash */
}WP_Struct;

/**
 * (Re-)Initialize the hashing state.
 *
 * @param wp  A WP_Struct handle, as created by WP_Create()
 * @require   wp != NULL
 */
void WP_Init(WP_Struct *wp);

/**
 * Delivers input data to the hashing algorithm.
 *
 * @param source      Plaintext data to hash.
 * @param sourceBits  How many bits of plaintext to process.
 * @param wp          A WP_Struct handle, as created by WP_Create()
 * @require source != NULL && wp != NULL
 */
void WP_Add(const unsigned char * const source, unsigned int sourceBytes,
				unsigned int allBytes, WP_Struct * const wp);

/**
 * Get the hash value from the hashing state.
 *
 * @param wp      A WP_Struct handle, as created by WP_Create()
 * @param result  A string to store the hash to.
 * @require
 *     wp != NULL
 *     result != NULL
 *     result must be able to hold at least WP_DIGEST_SIZE bytes.
 */
void WP_Finalize(WP_Struct * const wp, unsigned char * const result);
void processBuffer(WP_Struct * const structpointer);

#endif
