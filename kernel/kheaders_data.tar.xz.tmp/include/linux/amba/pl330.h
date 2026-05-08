

#ifndef	__AMBA_PL330_H_
#define	__AMBA_PL330_H_

#include <linux/dmaengine.h>

struct dma_pl330_platdata {
	
	u8 nr_valid_peri;
	
	u8 *peri_id;
	
	dma_cap_mask_t cap_mask;
	
	unsigned mcbuf_sz;
};

extern bool pl330_filter(struct dma_chan *chan, void *param);
extern int pl330_dma_getposition(struct dma_chan *chan,
		dma_addr_t *src, dma_addr_t *dst);
extern int pl330_dma_debug(struct dma_chan *chan);
#endif	
