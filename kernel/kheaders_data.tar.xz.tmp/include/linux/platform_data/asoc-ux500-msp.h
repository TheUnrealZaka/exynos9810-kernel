

#ifndef __MSP_H
#define __MSP_H

#include <linux/platform_data/dma-ste-dma40.h>


struct msp_i2s_platform_data {
	int id;
	struct stedma40_chan_cfg *msp_i2s_dma_rx;
	struct stedma40_chan_cfg *msp_i2s_dma_tx;
};

#endif
