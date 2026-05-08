
#ifndef __INCLUDE_SOUND_ATMEL_AC97C_H
#define __INCLUDE_SOUND_ATMEL_AC97C_H

#include <linux/platform_data/dma-dw.h>

#define AC97C_CAPTURE	0x01
#define AC97C_PLAYBACK	0x02
#define AC97C_BOTH	(AC97C_CAPTURE | AC97C_PLAYBACK)


struct ac97c_platform_data {
	struct dw_dma_slave	rx_dws;
	struct dw_dma_slave	tx_dws;
	int			reset_pin;
};

#endif 
