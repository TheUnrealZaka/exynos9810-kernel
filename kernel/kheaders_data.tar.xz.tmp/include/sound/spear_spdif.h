

#ifndef __SOUND_SPDIF_H
#define __SOUND_SPDIF_H

struct spear_spdif_platform_data {
	
	void *dma_params;
	bool (*filter)(struct dma_chan *chan, void *slave);
	void (*reset_perip)(void);
};

#endif 
