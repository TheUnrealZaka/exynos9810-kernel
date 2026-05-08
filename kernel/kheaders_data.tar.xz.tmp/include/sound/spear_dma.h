

#ifndef SPEAR_DMA_H
#define SPEAR_DMA_H

#include <linux/dmaengine.h>

struct spear_dma_data {
	void *data;
	dma_addr_t addr;
	u32 max_burst;
	enum dma_slave_buswidth addr_width;
};

#endif 
