
#ifndef __INCLUDE_SOUND_ATMEL_ABDAC_H
#define __INCLUDE_SOUND_ATMEL_ABDAC_H

#include <linux/platform_data/dma-dw.h>


struct atmel_abdac_pdata {
	struct dw_dma_slave	dws;
};

#endif 
