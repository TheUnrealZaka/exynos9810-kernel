

#ifndef __ASM_ARCH_MX3FB_H__
#define __ASM_ARCH_MX3FB_H__

#include <linux/device.h>
#include <linux/fb.h>


#define FB_SYNC_OE_ACT_HIGH	0x80000000
#define FB_SYNC_CLK_INVERT	0x40000000
#define FB_SYNC_DATA_INVERT	0x20000000
#define FB_SYNC_CLK_IDLE_EN	0x10000000
#define FB_SYNC_SHARP_MODE	0x08000000
#define FB_SYNC_SWAP_RGB	0x04000000
#define FB_SYNC_CLK_SEL_EN	0x02000000


enum disp_data_mapping {
	
	IPU_DISP_DATA_MAPPING_RGB666,
	
	IPU_DISP_DATA_MAPPING_RGB565,
	
	IPU_DISP_DATA_MAPPING_RGB888,
};


struct mx3fb_platform_data {
	struct device			*dma_dev;
	const char			*name;
	const struct fb_videomode	*mode;
	int				num_modes;
	enum disp_data_mapping		disp_data_fmt;
};

#endif
