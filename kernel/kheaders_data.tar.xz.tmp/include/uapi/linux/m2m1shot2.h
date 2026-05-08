

#ifndef _UAPI__M2M1SHOT2_H_
#define _UAPI__M2M1SHOT2_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/videodev2.h>


#define M2M1SHOT2_IMGTRFORM_ROT90		(1 << 0)
#define M2M1SHOT2_IMGTRFORM_ROT270CCW		M2M1SHOT2_IMGTRFORM_ROT90
#define M2M1SHOT2_IMGTRFORM_ROT180		(1 << 1)
#define M2M1SHOT2_IMGTRFORM_ROT270		\
			(M2M1SHOT2_IMGTRFORM_ROT90 + M2M1SHOT2_IMGTRFORM_ROT180)
#define M2M1SHOT2_IMGTRFORM_ROT90CCW		M2M1SHOT2_IMGTRFORM_ROT270
#define M2M1SHOT2_IMGTRFORM_XFLIP		(1 << 4)
#define M2M1SHOT2_IMGTRFORM_YFLIP		(1 << 5)

#define M2M1SHOT2_IMGTRFORM_ROTVAL(val)		((val) & 0xF)
#define M2M1SHOT2_IMGTRFORM_SET_ROTVAL(var, val) \
					((var) = ((val) & 0xF) | ((val) & ~0xF))


#define M2M1SHOT2_SCFILTER_DEFAULT	0
#define M2M1SHOT2_SCFILTER_NONE		1
#define M2M1SHOT2_SCFILTER_NEAREST	2
#define M2M1SHOT2_SCFILTER_BILINEAR	3
#define M2M1SHOT2_SCFILTER_BICUBIC	4


#define M2M1SHOT2_REPEAT_NONE		0
#define M2M1SHOT2_REPEAT_REPEAT		1
#define M2M1SHOT2_REPEAT_PAD		2
#define M2M1SHOT2_REPEAT_REFLECT	3
#define M2M1SHOT2_REPEAT_CLAMP		4


#define M2M1SHOT2_BLEND_NONE		0
#define M2M1SHOT2_BLEND_CLEAR		1
#define M2M1SHOT2_BLEND_SRC		2
#define M2M1SHOT2_BLEND_DST		3
#define M2M1SHOT2_BLEND_SRCOVER		4
#define M2M1SHOT2_BLEND_DSTOVER		5
#define M2M1SHOT2_BLEND_SRCIN		6
#define M2M1SHOT2_BLEND_DSTIN		7
#define M2M1SHOT2_BLEND_SRCOUT		8
#define M2M1SHOT2_BLEND_DSTOUT		9
#define M2M1SHOT2_BLEND_SRCATOP		10
#define M2M1SHOT2_BLEND_DSTATOP		11
#define M2M1SHOT2_BLEND_XOR		12
#define M2M1SHOT2_BLEND_PLUS		13
#define M2M1SHOT2_BLEND_MULTIPLY	14
#define M2M1SHOT2_BLEND_SCREEN		15
#define M2M1SHOT2_BLEND_DARKEN		16
#define M2M1SHOT2_BLEND_LIGHTEN		17
#define M2M1SHOT2_BLEND_SRCOVER_DISJ	18
#define M2M1SHOT2_BLEND_DSTOVER_DISJ	19
#define M2M1SHOT2_BLEND_SRCIN_DISJ	20
#define M2M1SHOT2_BLEND_DSTIN_DISJ	21
#define M2M1SHOT2_BLEND_SRCOUT_DISJ	22
#define M2M1SHOT2_BLEND_DSTOUT_DISJ	23
#define M2M1SHOT2_BLEND_SRCATOP_DISJ	24
#define M2M1SHOT2_BLEND_DSTATOP_DISJ	25
#define M2M1SHOT2_BLEND_XOR_DISJ	26
#define M2M1SHOT2_BLEND_SRCOVER_CONJ	27
#define M2M1SHOT2_BLEND_DSTOVER_CONJ	28
#define M2M1SHOT2_BLEND_SRCIN_CONJ	29
#define M2M1SHOT2_BLEND_DSTIN_CONJ	30
#define M2M1SHOT2_BLEND_SRCOUT_CONJ	31
#define M2M1SHOT2_BLEND_DSTOUT_CONJ	32
#define M2M1SHOT2_BLEND_SRCATOP_CONJ	33
#define M2M1SHOT2_BLEND_DSTATOP_CONJ	34
#define M2M1SHOT2_BLEND_XOR_CONJ	35

struct m2m1shot2_custom_data {
	__u32 cmd;
	__u32 arg;
};


struct m2m1shot2_extra {
	__u32			horizontal_factor;
	__u32			vertical_factor;
	__u32			fillcolor;
	__u16			transform;
	__u16			composit_mode;
	__u8			galpha;
	__u8			galpha_red;
	__u8			galpha_green;
	__u8			galpha_blue;
	__u8			xrepeat;
	__u8			yrepeat;
	__u8			scaler_filter;
};

#define M2M1SHOT2_MAX_PLANES 4
#define M2M1SHOT2_MAX_IMAGES 8


#define M2M1SHOT2_BUFTYPE_NONE		0
#define M2M1SHOT2_BUFTYPE_EMPTY		1 
#define M2M1SHOT2_BUFTYPE_USERPTR	2 
#define M2M1SHOT2_BUFTYPE_DMABUF	3 

#define M2M1SHOT2_BUFTYPE_VALID(type)	(((type) > 0) && ((type) < 4))


struct m2m1shot2_buffer {
	union {
		unsigned long userptr;
		__s32 fd;
	};
	__u32 offset;
	__u32 length;
	__u32 payload;
	unsigned long reserved;
};


struct m2m1shot2_format {
	__u32			width;
	__u32			height;
	__u32			pixelformat; 
	struct v4l2_rect	crop;
	struct v4l2_rect	window;
};


#define M2M1SHOT2_IMGFLAG_ACQUIRE_FENCE		(1 << 1)

#define M2M1SHOT2_IMGFLAG_RELEASE_FENCE		(1 << 2)

#define M2M1SHOT2_IMGFLAG_PREMUL_ALPHA		(1 << 4)

#define M2M1SHOT2_IMGFLAG_GLOBAL_ALPHA		(1 << 5)

#define M2M1SHOT2_IMGFLAG_UORDER_ADDR		(1 << 6)

#define M2M1SHOT2_IMGFLAG_COMPRESSED		(1 << 7)

#define M2M1SHOT2_IMGFLAG_NO_RESCALING		(1 << 8)

#define M2M1SHOT2_IMGFLAG_XSCALE_FACTOR		(1 << 9)

#define M2M1SHOT2_IMGFLAG_YSCALE_FACTOR		(1 << 10)

#define M2M1SHOT2_IMGFLAG_COLORFILL		(1 << 11)

#define M2M1SHOT2_IMGFLAG_SECURE		(1 << 12)

#define M2M1SHOT2_IMGFLAG_NO_CACHECLEAN		(1 << 16)

#define M2M1SHOT2_IMGFLAG_NO_CACHEINV		(1 << 17)


struct m2m1shot2_image {
	__u32			flags;
	__s32			fence;
	__u8			memory;
	__u8			num_planes;
	struct m2m1shot2_buffer	plane[M2M1SHOT2_MAX_PLANES];
	struct m2m1shot2_format	fmt;
	struct m2m1shot2_extra	ext;
	__u32			colorspace;
	__u32			reserved[3];
};



#define M2M1SHOT2_FLAG_DITHER		(1 << 1)

#define M2M1SHOT2_FLAG_NONBLOCK		(1 << 2)

#define M2M1SHOT2_FLAG_ERROR		(1 << 4)


struct m2m1shot2 {
	struct m2m1shot2_image	*sources;
	struct m2m1shot2_image	target;
	__u8			num_sources;
	__u32			flags;

	__u32			reserved1;
	__u32			reserved2;
	unsigned long		work_delay_in_nsec;
	unsigned long		reserved4;
};

#define M2M1SHOT2_IOC_PROCESS		_IOWR('M', 4, struct m2m1shot2)

#define M2M1SHOT2_IOC_WAIT_PROCESS	_IOR('M', 5, struct m2m1shot2)

enum m2m1shot2_priority {
	M2M1SHOT2_LOW_PRIORITY,
	M2M1SHOT2_MEDIUM_PRIORITY,
	M2M1SHOT2_DEFAULT_PRIORITY = M2M1SHOT2_MEDIUM_PRIORITY,
	M2M1SHOT2_HIGH_PRIORITY,
	M2M1SHOT2_HIGHEST_PRIORITY,
	M2M1SHOT2_PRIORITY_END
};

#define M2M1SHOT2_IOC_SET_PRIORITY	_IOR('M', 6, int32_t)

#define M2M1SHOT2_PERF_MAX_FRAMES 4


#define M2M1SHOT2_PERF_LAYER_ROTATE		1

struct m2m1shot2_performance_layer_data {
	__u32 pixelcount;
	__u32 layer_attr;
};


#define M2M1SHOT2_PERF_FRAME_SOLIDCOLORFILL	1

struct m2m1shot2_performance_frame_data {
	struct m2m1shot2_performance_layer_data layer[M2M1SHOT2_MAX_IMAGES];
	__u32 bandwidth_read;
	__u32 bandwidth_write;
	__u32 target_pixelcount;
	__u32 frame_rate;
	__u32 frame_attr;
	__u32 num_layers;
};

struct m2m1shot2_performance_data {
	struct m2m1shot2_performance_frame_data frame[M2M1SHOT2_PERF_MAX_FRAMES];
	__u32 num_frames;
	__u32 reserved;
};

#define M2M1SHOT2_IOC_REQUEST_PERF	_IOR('M', 7, struct m2m1shot2_performance_data)
#define M2M1SHOT2_IOC_CUSTOM		_IOR('M', 8, struct m2m1shot2_custom_data)

#endif 
