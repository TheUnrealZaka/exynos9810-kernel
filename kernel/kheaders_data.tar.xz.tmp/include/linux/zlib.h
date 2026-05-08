

#ifndef _ZLIB_H
#define _ZLIB_H

#include <linux/zconf.h>








struct internal_state;

typedef struct z_stream_s {
    const Byte *next_in;   
	uLong avail_in;  
    uLong    total_in;  

    Byte    *next_out;  
	uLong avail_out; 
    uLong    total_out; 

    char     *msg;      
    struct internal_state *state; 

    void     *workspace; 

    int     data_type;  
    uLong   adler;      
    uLong   reserved;   
} z_stream;

typedef z_stream *z_streamp;



                        

#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1 
#define Z_PACKET_FLUSH  2
#define Z_SYNC_FLUSH    3
#define Z_FULL_FLUSH    4
#define Z_FINISH        5
#define Z_BLOCK         6 


#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)


#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)


#define Z_FILTERED            1
#define Z_HUFFMAN_ONLY        2
#define Z_DEFAULT_STRATEGY    0


#define Z_BINARY   0
#define Z_ASCII    1
#define Z_UNKNOWN  2


#define Z_DEFLATED   8


                        

extern int zlib_deflate_workspacesize (int windowBits, int memLevel);





extern int zlib_deflate (z_streamp strm, int flush);



extern int zlib_deflateEnd (z_streamp strm);



extern int zlib_inflate_workspacesize (void);





extern int zlib_inflate (z_streamp strm, int flush);



extern int zlib_inflateEnd (z_streamp strm);


                        





extern int zlib_deflateReset (z_streamp strm);


static inline unsigned long deflateBound(unsigned long s)
{
	return s + ((s + 7) >> 3) + ((s + 63) >> 6) + 11;
}



extern int zlib_inflateReset (z_streamp strm);


extern int zlib_inflateIncomp (z_stream *strm);


#define zlib_deflateInit(strm, level) \
	zlib_deflateInit2((strm), (level), Z_DEFLATED, MAX_WBITS, \
			      DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY)
#define zlib_inflateInit(strm) \
	zlib_inflateInit2((strm), DEF_WBITS)

extern int zlib_deflateInit2(z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy);
extern int zlib_inflateInit2(z_streamp strm, int  windowBits);

#if !defined(_Z_UTIL_H) && !defined(NO_DUMMY_DECL)
    struct internal_state {int dummy;}; 
#endif


extern int zlib_inflate_blob(void *dst, unsigned dst_sz, const void *src, unsigned src_sz);

#endif 
