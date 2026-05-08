#ifndef DECOMPRESS_GENERIC_H
#define DECOMPRESS_GENERIC_H

typedef int (*decompress_fn) (unsigned char *inbuf, long len,
			      long (*fill)(void*, unsigned long),
			      long (*flush)(void*, unsigned long),
			      unsigned char *outbuf,
			      long *posp,
			      void(*error)(char *x));





decompress_fn decompress_method(const unsigned char *inbuf, long len,
				const char **name);

#endif
