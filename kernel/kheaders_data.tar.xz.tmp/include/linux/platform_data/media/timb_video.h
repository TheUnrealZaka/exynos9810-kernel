

#ifndef _TIMB_VIDEO_
#define _TIMB_VIDEO_ 1

#include <linux/i2c.h>

struct timb_video_platform_data {
	int dma_channel;
	int i2c_adapter; 
	struct {
		const char *module_name;
		struct i2c_board_info *info;
	} encoder;
};

#endif
