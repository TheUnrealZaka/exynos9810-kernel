

#ifndef HWASTC_API
#define HWASTC_API

#include <linux/ioctl.h>
#include <linux/types.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif





enum hwASTC_blk_size {
	ASTC_ENCODE_MODE_BLK_SIZE_4x4,
	ASTC_ENCODE_MODE_BLK_SIZE_6x6,
	ASTC_ENCODE_MODE_BLK_SIZE_8x8,
};


enum hwASTC_wt_ref_iter {
	ASTC_ENCODE_MODE_REF_ITER_ZERO,
	ASTC_ENCODE_MODE_REF_ITER_ONE,
	ASTC_ENCODE_MODE_REF_ITER_TWO,
	ASTC_ENCODE_MODE_REF_ITER_FOUR,
};


enum hwASTC_partitioning {
	ASTC_ENCODE_MODE_PARTITION_ONE,
	ASTC_ENCODE_MODE_PARTITION_TWO,
};


enum hwASTC_no_block_mode {
	ASTC_ENCODE_MODE_NO_BLOCK_MODE_16EA,
	ASTC_ENCODE_MODE_NO_BLOCK_MODE_FULL,
};


enum hwASTC_input_pixel_format {
	ASTC_ENCODE_INPUT_PIXEL_FORMAT_RGBA8888,
	ASTC_ENCODE_INPUT_PIXEL_FORMAT_ARGB8888,
	ASTC_ENCODE_INPUT_PIXEL_FORMAT_BGRA8888,
	ASTC_ENCODE_INPUT_PIXEL_FORMAT_ABGR8888,
};


struct hwASTC_img_info {
	__u32 fmt;
	__u32 width;
	__u32 height;
};


enum hwASTC_buffer_type {
	HWASTC_BUFFER_NONE,	
	HWASTC_BUFFER_DMABUF,
	HWASTC_BUFFER_USERPTR,
};


struct hwASTC_buffer {
	union {
		
		__s32 fd;
		
		unsigned long userptr;
	};
	size_t len;
	__u8 type;
};


struct hwASTC_config {
	__u8       encodeBlockSize;
	__u8       intref_iterations;
	__u8       partitions;
	__u8       num_blk_mode;
	__u8       dual_plane_enable;

	
	__u8       reserved[3];
};


struct hwASTC_task {
	struct hwASTC_img_info info_out;
	struct hwASTC_img_info info_cap;
	struct hwASTC_buffer buf_out;
	struct hwASTC_buffer buf_cap;
	struct hwASTC_config enc_config;

	
	unsigned long reserved[2];
};


#define HWASTC_IOC_PROCESS	_IOWR('M',   0, struct hwASTC_task)

#endif // HWASTC_API

