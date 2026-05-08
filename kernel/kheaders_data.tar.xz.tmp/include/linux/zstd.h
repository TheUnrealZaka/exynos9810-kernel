

#ifndef ZSTD_H
#define ZSTD_H


#include <linux/types.h>   







typedef enum {
	ZSTD_error_no_error,
	ZSTD_error_GENERIC,
	ZSTD_error_prefix_unknown,
	ZSTD_error_version_unsupported,
	ZSTD_error_parameter_unknown,
	ZSTD_error_frameParameter_unsupported,
	ZSTD_error_frameParameter_unsupportedBy32bits,
	ZSTD_error_frameParameter_windowTooLarge,
	ZSTD_error_compressionParameter_unsupported,
	ZSTD_error_init_missing,
	ZSTD_error_memory_allocation,
	ZSTD_error_stage_wrong,
	ZSTD_error_dstSize_tooSmall,
	ZSTD_error_srcSize_wrong,
	ZSTD_error_corruption_detected,
	ZSTD_error_checksum_wrong,
	ZSTD_error_tableLog_tooLarge,
	ZSTD_error_maxSymbolValue_tooLarge,
	ZSTD_error_maxSymbolValue_tooSmall,
	ZSTD_error_dictionary_corrupted,
	ZSTD_error_dictionary_wrong,
	ZSTD_error_dictionaryCreation_failed,
	ZSTD_error_maxCode
} ZSTD_ErrorCode;


int ZSTD_maxCLevel(void);

size_t ZSTD_compressBound(size_t srcSize);

static __attribute__((unused)) unsigned int ZSTD_isError(size_t code)
{
	return code > (size_t)-ZSTD_error_maxCode;
}

static __attribute__((unused)) ZSTD_ErrorCode ZSTD_getErrorCode(
	size_t functionResult)
{
	if (!ZSTD_isError(functionResult))
		return (ZSTD_ErrorCode)0;
	return (ZSTD_ErrorCode)(0 - functionResult);
}


typedef enum {
	ZSTD_fast,
	ZSTD_dfast,
	ZSTD_greedy,
	ZSTD_lazy,
	ZSTD_lazy2,
	ZSTD_btlazy2,
	ZSTD_btopt,
	ZSTD_btopt2
} ZSTD_strategy;


typedef struct {
	unsigned int windowLog;
	unsigned int chainLog;
	unsigned int hashLog;
	unsigned int searchLog;
	unsigned int searchLength;
	unsigned int targetLength;
	ZSTD_strategy strategy;
} ZSTD_compressionParameters;


typedef struct {
	unsigned int contentSizeFlag;
	unsigned int checksumFlag;
	unsigned int noDictIDFlag;
} ZSTD_frameParameters;


typedef struct {
	ZSTD_compressionParameters cParams;
	ZSTD_frameParameters fParams;
} ZSTD_parameters;


ZSTD_compressionParameters ZSTD_getCParams(int compressionLevel,
	unsigned long long estimatedSrcSize, size_t dictSize);


ZSTD_parameters ZSTD_getParams(int compressionLevel,
	unsigned long long estimatedSrcSize, size_t dictSize);




size_t ZSTD_CCtxWorkspaceBound(ZSTD_compressionParameters cParams);


typedef struct ZSTD_CCtx_s ZSTD_CCtx;

ZSTD_CCtx *ZSTD_initCCtx(void *workspace, size_t workspaceSize);


size_t ZSTD_compressCCtx(ZSTD_CCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, ZSTD_parameters params);


size_t ZSTD_DCtxWorkspaceBound(void);


typedef struct ZSTD_DCtx_s ZSTD_DCtx;

ZSTD_DCtx *ZSTD_initDCtx(void *workspace, size_t workspaceSize);


size_t ZSTD_decompressDCtx(ZSTD_DCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);




size_t ZSTD_compress_usingDict(ZSTD_CCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, const void *dict, size_t dictSize,
	ZSTD_parameters params);


size_t ZSTD_decompress_usingDict(ZSTD_DCtx *ctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, const void *dict, size_t dictSize);




size_t ZSTD_CDictWorkspaceBound(ZSTD_compressionParameters cParams);


typedef struct ZSTD_CDict_s ZSTD_CDict;


ZSTD_CDict *ZSTD_initCDict(const void *dictBuffer, size_t dictSize,
	ZSTD_parameters params, void *workspace, size_t workspaceSize);


size_t ZSTD_compress_usingCDict(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize, const ZSTD_CDict *cdict);



size_t ZSTD_DDictWorkspaceBound(void);


typedef struct ZSTD_DDict_s ZSTD_DDict;


ZSTD_DDict *ZSTD_initDDict(const void *dictBuffer, size_t dictSize,
	void *workspace, size_t workspaceSize);


size_t ZSTD_decompress_usingDDict(ZSTD_DCtx *dctx, void *dst,
	size_t dstCapacity, const void *src, size_t srcSize,
	const ZSTD_DDict *ddict);





typedef struct ZSTD_inBuffer_s {
	const void *src;
	size_t size;
	size_t pos;
} ZSTD_inBuffer;


typedef struct ZSTD_outBuffer_s {
	void *dst;
	size_t size;
	size_t pos;
} ZSTD_outBuffer;






size_t ZSTD_CStreamWorkspaceBound(ZSTD_compressionParameters cParams);


typedef struct ZSTD_CStream_s ZSTD_CStream;



ZSTD_CStream *ZSTD_initCStream(ZSTD_parameters params,
	unsigned long long pledgedSrcSize, void *workspace,
	size_t workspaceSize);


ZSTD_CStream *ZSTD_initCStream_usingCDict(const ZSTD_CDict *cdict,
	unsigned long long pledgedSrcSize, void *workspace,
	size_t workspaceSize);



size_t ZSTD_resetCStream(ZSTD_CStream *zcs, unsigned long long pledgedSrcSize);

size_t ZSTD_compressStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output,
	ZSTD_inBuffer *input);

size_t ZSTD_flushStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output);

size_t ZSTD_endStream(ZSTD_CStream *zcs, ZSTD_outBuffer *output);


size_t ZSTD_CStreamInSize(void);

size_t ZSTD_CStreamOutSize(void);






size_t ZSTD_DStreamWorkspaceBound(size_t maxWindowSize);


typedef struct ZSTD_DStream_s ZSTD_DStream;


ZSTD_DStream *ZSTD_initDStream(size_t maxWindowSize, void *workspace,
	size_t workspaceSize);

ZSTD_DStream *ZSTD_initDStream_usingDDict(size_t maxWindowSize,
	const ZSTD_DDict *ddict, void *workspace, size_t workspaceSize);



size_t ZSTD_resetDStream(ZSTD_DStream *zds);

size_t ZSTD_decompressStream(ZSTD_DStream *zds, ZSTD_outBuffer *output,
	ZSTD_inBuffer *input);


size_t ZSTD_DStreamInSize(void);

size_t ZSTD_DStreamOutSize(void);



#define ZSTD_MAGICNUMBER            0xFD2FB528   
#define ZSTD_MAGIC_SKIPPABLE_START  0x184D2A50U

#define ZSTD_CONTENTSIZE_UNKNOWN (0ULL - 1)
#define ZSTD_CONTENTSIZE_ERROR   (0ULL - 2)

#define ZSTD_WINDOWLOG_MAX_32  27
#define ZSTD_WINDOWLOG_MAX_64  27
#define ZSTD_WINDOWLOG_MAX \
	((unsigned int)(sizeof(size_t) == 4 \
		? ZSTD_WINDOWLOG_MAX_32 \
		: ZSTD_WINDOWLOG_MAX_64))
#define ZSTD_WINDOWLOG_MIN 10
#define ZSTD_HASHLOG_MAX ZSTD_WINDOWLOG_MAX
#define ZSTD_HASHLOG_MIN        6
#define ZSTD_CHAINLOG_MAX     (ZSTD_WINDOWLOG_MAX+1)
#define ZSTD_CHAINLOG_MIN      ZSTD_HASHLOG_MIN
#define ZSTD_HASHLOG3_MAX      17
#define ZSTD_SEARCHLOG_MAX    (ZSTD_WINDOWLOG_MAX-1)
#define ZSTD_SEARCHLOG_MIN      1

#define ZSTD_SEARCHLENGTH_MAX   7

#define ZSTD_SEARCHLENGTH_MIN   3
#define ZSTD_TARGETLENGTH_MIN   4
#define ZSTD_TARGETLENGTH_MAX 999


#define ZSTD_FRAMEHEADERSIZE_MAX 18
#define ZSTD_FRAMEHEADERSIZE_MIN  6
static const size_t ZSTD_frameHeaderSize_prefix = 5;
static const size_t ZSTD_frameHeaderSize_min = ZSTD_FRAMEHEADERSIZE_MIN;
static const size_t ZSTD_frameHeaderSize_max = ZSTD_FRAMEHEADERSIZE_MAX;

static const size_t ZSTD_skippableHeaderSize = 8;





size_t ZSTD_findFrameCompressedSize(const void *src, size_t srcSize);



unsigned long long ZSTD_getFrameContentSize(const void *src, size_t srcSize);


unsigned long long ZSTD_findDecompressedSize(const void *src, size_t srcSize);



size_t ZSTD_checkCParams(ZSTD_compressionParameters cParams);


ZSTD_compressionParameters ZSTD_adjustCParams(
	ZSTD_compressionParameters cParams, unsigned long long srcSize,
	size_t dictSize);




unsigned int ZSTD_isFrame(const void *buffer, size_t size);


unsigned int ZSTD_getDictID_fromDict(const void *dict, size_t dictSize);


unsigned int ZSTD_getDictID_fromDDict(const ZSTD_DDict *ddict);


unsigned int ZSTD_getDictID_fromFrame(const void *src, size_t srcSize);


typedef struct {
	unsigned long long frameContentSize;
	unsigned int windowSize;
	unsigned int dictID;
	unsigned int checksumFlag;
} ZSTD_frameParams;


size_t ZSTD_getFrameParams(ZSTD_frameParams *fparamsPtr, const void *src,
	size_t srcSize);






size_t ZSTD_compressBegin(ZSTD_CCtx *cctx, int compressionLevel);
size_t ZSTD_compressBegin_usingDict(ZSTD_CCtx *cctx, const void *dict,
	size_t dictSize, int compressionLevel);
size_t ZSTD_compressBegin_advanced(ZSTD_CCtx *cctx, const void *dict,
	size_t dictSize, ZSTD_parameters params,
	unsigned long long pledgedSrcSize);
size_t ZSTD_copyCCtx(ZSTD_CCtx *cctx, const ZSTD_CCtx *preparedCCtx,
	unsigned long long pledgedSrcSize);
size_t ZSTD_compressBegin_usingCDict(ZSTD_CCtx *cctx, const ZSTD_CDict *cdict,
	unsigned long long pledgedSrcSize);
size_t ZSTD_compressContinue(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
size_t ZSTD_compressEnd(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);






size_t ZSTD_decompressBegin(ZSTD_DCtx *dctx);
size_t ZSTD_decompressBegin_usingDict(ZSTD_DCtx *dctx, const void *dict,
	size_t dictSize);
void   ZSTD_copyDCtx(ZSTD_DCtx *dctx, const ZSTD_DCtx *preparedDCtx);
size_t ZSTD_nextSrcSizeToDecompress(ZSTD_DCtx *dctx);
size_t ZSTD_decompressContinue(ZSTD_DCtx *dctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
typedef enum {
	ZSTDnit_frameHeader,
	ZSTDnit_blockHeader,
	ZSTDnit_block,
	ZSTDnit_lastBlock,
	ZSTDnit_checksum,
	ZSTDnit_skippableFrame
} ZSTD_nextInputType_e;
ZSTD_nextInputType_e ZSTD_nextInputType(ZSTD_DCtx *dctx);




#define ZSTD_BLOCKSIZE_ABSOLUTEMAX (128 * 1024)

size_t ZSTD_getBlockSizeMax(ZSTD_CCtx *cctx);
size_t ZSTD_compressBlock(ZSTD_CCtx *cctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
size_t ZSTD_decompressBlock(ZSTD_DCtx *dctx, void *dst, size_t dstCapacity,
	const void *src, size_t srcSize);
size_t ZSTD_insertBlock(ZSTD_DCtx *dctx, const void *blockStart,
	size_t blockSize);

#endif  
