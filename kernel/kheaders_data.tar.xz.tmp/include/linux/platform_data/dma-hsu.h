

#ifndef _PLATFORM_DATA_DMA_HSU_H
#define _PLATFORM_DATA_DMA_HSU_H

#include <linux/device.h>

struct hsu_dma_slave {
	struct device	*dma_dev;
	int		chan_id;
};

#endif 
