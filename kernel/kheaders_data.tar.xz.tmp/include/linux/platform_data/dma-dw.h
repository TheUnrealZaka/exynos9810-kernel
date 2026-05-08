
#ifndef _PLATFORM_DATA_DMA_DW_H
#define _PLATFORM_DATA_DMA_DW_H

#include <linux/device.h>

#define DW_DMA_MAX_NR_MASTERS	4


struct dw_dma_slave {
	struct device		*dma_dev;
	u8			src_id;
	u8			dst_id;
	u8			m_master;
	u8			p_master;
	bool			hs_polarity;
};


struct dw_dma_platform_data {
	unsigned int	nr_channels;
	bool		is_private;
	bool		is_memcpy;
	bool		is_nollp;
#define CHAN_ALLOCATION_ASCENDING	0	
#define CHAN_ALLOCATION_DESCENDING	1	
	unsigned char	chan_allocation_order;
#define CHAN_PRIORITY_ASCENDING		0	
#define CHAN_PRIORITY_DESCENDING	1	
	unsigned char	chan_priority;
	unsigned int	block_size;
	unsigned char	nr_masters;
	unsigned char	data_width[DW_DMA_MAX_NR_MASTERS];
};

#endif 
