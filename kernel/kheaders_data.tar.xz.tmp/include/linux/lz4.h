

#ifndef __LZ4_H__
#define __LZ4_H__

#include <linux/types.h>
#include <linux/string.h> 



#define LZ4_MEMORY_USAGE 14

#define LZ4_MAX_INPUT_SIZE 0x7E000000 
#define LZ4_COMPRESSBOUND(isize)                                    \
	((unsigned int)(isize) > (unsigned int)LZ4_MAX_INPUT_SIZE ? \
		 0 :                                                \
		 (isize) + ((isize) / 255) + 16)

#define LZ4_ACCELERATION_DEFAULT 1
#define LZ4_ACCELERATION_MAX 65537
#define LZ4_HASHLOG (LZ4_MEMORY_USAGE - 2)
#define LZ4_HASHTABLESIZE (1 << LZ4_MEMORY_USAGE)
#define LZ4_HASH_SIZE_U32 (1 << LZ4_HASHLOG)

#define LZ4HC_MIN_CLEVEL 3
#define LZ4HC_DEFAULT_CLEVEL 9
#define LZ4HC_MAX_CLEVEL 16

#define LZ4HC_DICTIONARY_LOGSIZE 16
#define LZ4HC_MAXD (1 << LZ4HC_DICTIONARY_LOGSIZE)
#define LZ4HC_MAXD_MASK (LZ4HC_MAXD - 1)
#define LZ4HC_HASH_LOG (LZ4HC_DICTIONARY_LOGSIZE - 1)
#define LZ4HC_HASHTABLESIZE (1 << LZ4HC_HASH_LOG)
#define LZ4HC_HASH_MASK (LZ4HC_HASHTABLESIZE - 1)


#define LZ4_STREAM_MINSIZE           \
	((1UL << LZ4_MEMORY_USAGE) + \
	 32) 

#define LZ4_STREAMHCSIZE_SIZET (262192 / sizeof(size_t))




typedef struct LZ4_stream_t_internal LZ4_stream_t_internal;
struct LZ4_stream_t_internal {
	uint32_t hashTable[LZ4_HASH_SIZE_U32];
	const uint8_t *dictionary;
	const LZ4_stream_t_internal *dictCtx;
	uint32_t currentOffset;
	uint32_t tableType;
	uint32_t dictSize;
	
};
typedef union {
	char minStateSize[LZ4_STREAM_MINSIZE];
	LZ4_stream_t_internal internal_donotuse;
} LZ4_stream_t;


typedef struct {
	unsigned int hashTable[LZ4HC_HASHTABLESIZE];
	unsigned short chainTable[LZ4HC_MAXD];
	
	const unsigned char *end;
	
	const unsigned char *base;
	
	const unsigned char *dictBase;
	
	unsigned int dictLimit;
	
	unsigned int lowLimit;
	
	unsigned int nextToUpdate;
	unsigned int compressionLevel;
} LZ4HC_CCtx_internal;
typedef union {
	size_t table[LZ4_STREAMHCSIZE_SIZET];
	LZ4HC_CCtx_internal internal_donotuse;
} LZ4_streamHC_t;



typedef struct {
	const uint8_t *externalDict;
	const uint8_t *prefixEnd;
	size_t extDictSize;
	size_t prefixSize;
} LZ4_streamDecode_t_internal;
#define LZ4_STREAMDECODE_MINSIZE 32
typedef union {
	char minStateSize[LZ4_STREAMDECODE_MINSIZE];
	LZ4_streamDecode_t_internal internal_donotuse;
} LZ4_streamDecode_t;


#define LZ4_MEM_COMPRESS sizeof(LZ4_stream_t)
#define LZ4HC_MEM_COMPRESS LZ4_STREAMHCSIZE




static inline int LZ4_compressBound(size_t isize)
{
	return LZ4_COMPRESSBOUND(isize);
}


static inline int lz4_compressbound(size_t isize)
{
	return LZ4_COMPRESSBOUND(isize);
}


int LZ4_compress_default(const char *source, char *dest, int inputSize,
			 int maxOutputSize, void *wrkmem);


int LZ4_compress_fast(const char *source, char *dest, int inputSize,
		      int maxOutputSize, int acceleration, void *wrkmem);


int LZ4_compress_destSize(const char *source, char *dest, int *sourceSizePtr,
			  int targetDestSize, void *wrkmem);


int lz4_compress(const unsigned char *src, size_t src_len, unsigned char *dst,
	size_t *dst_len, void *wrkmem);




int LZ4_decompress_safe(const char *source, char *dest, int compressedSize,
			int maxDecompressedSize);


int LZ4_decompress_safe_partial(const char *source, char *dest,
				int compressedSize, int targetOutputSize,
				int maxDecompressedSize);


int lz4_decompress_unknownoutputsize(const unsigned char *src, size_t src_len,
	unsigned char *dest, size_t *dest_len);


int lz4_decompress(const unsigned char *src, size_t *src_len,
	unsigned char *dest, size_t actual_dest_len);




int LZ4_compress_HC(const char *src, char *dst, int srcSize, int dstCapacity,
		    int compressionLevel, void *wrkmem);


int lz4hc_compress(const unsigned char *src, size_t src_len, unsigned char *dst,
	size_t *dst_len, void *wrkmem);


void LZ4_resetStreamHC(LZ4_streamHC_t *streamHCPtr, int compressionLevel);


int LZ4_loadDictHC(LZ4_streamHC_t *streamHCPtr, const char *dictionary,
		   int dictSize);


int LZ4_compress_HC_continue(LZ4_streamHC_t *streamHCPtr, const char *src,
			     char *dst, int srcSize, int maxDstSize);


int LZ4_saveDictHC(LZ4_streamHC_t *streamHCPtr, char *safeBuffer,
		   int maxDictSize);




void LZ4_resetStream(LZ4_stream_t *LZ4_stream);


int LZ4_loadDict(LZ4_stream_t *streamPtr, const char *dictionary, int dictSize);


int LZ4_saveDict(LZ4_stream_t *streamPtr, char *safeBuffer, int dictSize);


int LZ4_compress_fast_continue(LZ4_stream_t *streamPtr, const char *src,
			       char *dst, int srcSize, int maxDstSize,
			       int acceleration);


int LZ4_setStreamDecode(LZ4_streamDecode_t *LZ4_streamDecode,
			const char *dictionary, int dictSize);


int LZ4_decompress_safe_continue(LZ4_streamDecode_t *LZ4_streamDecode,
				 const char *source, char *dest,
				 int compressedSize, int maxDecompressedSize);





//int LZ4_decompress_fast(const char *source, char *dest, int originalSize);



//int LZ4_decompress_fast_continue(LZ4_streamDecode_t *LZ4_streamDecode,
//	const char *source, char *dest, int originalSize);


//int LZ4_decompress_safe_usingDict(const char *source, char *dest,
//	int compressedSize, int maxDecompressedSize, const char *dictStart,
//	int dictSize);


//int LZ4_decompress_fast_usingDict(const char *source, char *dest,
//	int originalSize, const char *dictStart, int dictSize);

ssize_t LZ4_arm64_decompress_safe(const void *source, void *dest, size_t inputSize, size_t outputSize, bool dip);

ssize_t LZ4_arm64_decompress_safe_partial(const void *source, void *dest, size_t inputSize, size_t outputSize, bool dip);

#endif
