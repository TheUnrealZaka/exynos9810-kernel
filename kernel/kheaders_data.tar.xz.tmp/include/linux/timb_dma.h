



#ifndef _LINUX_TIMB_DMA_H
#define _LINUX_TIMB_DMA_H


struct timb_dma_platform_data_channel {
	bool rx;
	unsigned int bytes_per_line;
	unsigned int descriptors;
	unsigned int descriptor_elements;
};


struct timb_dma_platform_data {
	unsigned nr_channels;
	struct timb_dma_platform_data_channel channels[32];
};

#endif
