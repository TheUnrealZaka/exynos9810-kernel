

#ifndef _UAPI_LINUX_UUID_H_
#define _UAPI_LINUX_UUID_H_

#include <linux/types.h>
#include <linux/string.h>

typedef struct {
	__u8 b[16];
} uuid_le;

typedef struct {
	__u8 b[16];
} uuid_be;

#define UUID_LE(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)		\
((uuid_le)								\
{{ (a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff, ((a) >> 24) & 0xff, \
   (b) & 0xff, ((b) >> 8) & 0xff,					\
   (c) & 0xff, ((c) >> 8) & 0xff,					\
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})

#define UUID_BE(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)		\
((uuid_be)								\
{{ ((a) >> 24) & 0xff, ((a) >> 16) & 0xff, ((a) >> 8) & 0xff, (a) & 0xff, \
   ((b) >> 8) & 0xff, (b) & 0xff,					\
   ((c) >> 8) & 0xff, (c) & 0xff,					\
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})

#define NULL_UUID_LE							\
	UUID_LE(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00,	\
		0x00, 0x00, 0x00, 0x00)

#define NULL_UUID_BE							\
	UUID_BE(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00,	\
		0x00, 0x00, 0x00, 0x00)


#endif 
