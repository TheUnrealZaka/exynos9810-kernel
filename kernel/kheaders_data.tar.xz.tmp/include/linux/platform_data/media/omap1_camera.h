

#ifndef __MEDIA_OMAP1_CAMERA_H_
#define __MEDIA_OMAP1_CAMERA_H_

#include <linux/bitops.h>

#define OMAP1_CAMERA_IOSIZE		0x1c

enum omap1_cam_vb_mode {
	OMAP1_CAM_DMA_CONTIG = 0,
	OMAP1_CAM_DMA_SG,
};

#define OMAP1_CAMERA_MIN_BUF_COUNT(x)	((x) == OMAP1_CAM_DMA_CONTIG ? 3 : 2)

struct omap1_cam_platform_data {
	unsigned long	camexclk_khz;
	unsigned long	lclk_khz_max;
	unsigned long	flags;
};

#define OMAP1_CAMERA_LCLK_RISING	BIT(0)
#define OMAP1_CAMERA_RST_LOW		BIT(1)
#define OMAP1_CAMERA_RST_HIGH		BIT(2)

#endif 
