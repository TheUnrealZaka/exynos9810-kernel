
#ifndef _VPBE_TYPES_H
#define _VPBE_TYPES_H

enum vpbe_version {
	VPBE_VERSION_1 = 1,
	VPBE_VERSION_2,
	VPBE_VERSION_3,
};


enum vpbe_enc_timings_type {
	VPBE_ENC_STD = 0x1,
	VPBE_ENC_DV_TIMINGS = 0x4,
	
	VPBE_ENC_TIMINGS_INVALID = 0x8,
};


struct vpbe_enc_mode_info {
	unsigned char *name;
	enum vpbe_enc_timings_type timings_type;
	v4l2_std_id std_id;
	struct v4l2_dv_timings dv_timings;
	unsigned int interlaced;
	unsigned int xres;
	unsigned int yres;
	struct v4l2_fract aspect;
	struct v4l2_fract fps;
	unsigned int left_margin;
	unsigned int right_margin;
	unsigned int upper_margin;
	unsigned int lower_margin;
	unsigned int hsync_len;
	unsigned int vsync_len;
	unsigned int flags;
};

#endif
