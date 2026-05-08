
#ifndef _UAPI__M2M1SHOT_H_
#define _UAPI__M2M1SHOT_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/videodev2.h>

#define M2M1SHOT_MAX_PLANES 3

struct m2m1shot_rect {
	__s16 left;
	__s16 top;
	__u16 width;
	__u16 height;
};

struct m2m1shot_pix_format {
	__u32 fmt;
	__u32 width;
	__u32 height;
	struct v4l2_rect crop;
};

enum m2m1shot_buffer_type {
	M2M1SHOT_BUFFER_NONE,	
	M2M1SHOT_BUFFER_DMABUF,
	M2M1SHOT_BUFFER_USERPTR,
};

struct m2m1shot_buffer_plane {
	union {
		__s32 fd;
		unsigned long userptr;
	};
	size_t len;
};

struct m2m1shot_buffer {
	struct m2m1shot_buffer_plane plane[M2M1SHOT_MAX_PLANES];
	__u8 type;
	__u8 num_planes;
};

#define M2M1SHOT_OP_FLIP_VIRT		(1 << 0)
#define M2M1SHOT_OP_FLIP_HORI		(1 << 1)
#define M2M1SHOT_OP_CSC_WIDE		(1 << 8)
#define M2M1SHOT_OP_CSC_NARROW		(1 << 9)
#define M2M1SHOT_OP_CSC_601		(1 << 10)
#define M2M1SHOT_OP_CSC_709		(1 << 11)
#define M2M1SHOT_OP_PREMULTIPLIED_ALPHA	(1 << 16)
#define M2M1SHOT_OP_DITHERING		(1 << 17)

struct m2m1shot_operation {
	__s16 quality_level;
	__s16 rotate;
	__u32 op; 
};

struct m2m1shot {
	struct m2m1shot_pix_format fmt_out;
	struct m2m1shot_pix_format fmt_cap;
	struct m2m1shot_buffer buf_out;
	struct m2m1shot_buffer buf_cap;
	struct m2m1shot_operation op;
	unsigned long reserved[2];
};

struct m2m1shot_custom_data {
	unsigned int cmd;
	unsigned long arg;
};

#define M2M1SHOT_IOC_PROCESS	_IOWR('M',  0, struct m2m1shot)
#define M2M1SHOT_IOC_CUSTOM	_IOWR('M', 16, struct m2m1shot_custom_data)

#endif 
