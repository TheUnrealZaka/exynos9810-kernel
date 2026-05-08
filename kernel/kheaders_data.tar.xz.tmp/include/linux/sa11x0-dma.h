
#ifndef __LINUX_SA11X0_DMA_H
#define __LINUX_SA11X0_DMA_H

struct dma_chan;

#if defined(CONFIG_DMA_SA11X0) || defined(CONFIG_DMA_SA11X0_MODULE)
bool sa11x0_dma_filter_fn(struct dma_chan *, void *);
#else
static inline bool sa11x0_dma_filter_fn(struct dma_chan *c, void *d)
{
	return false;
}
#endif

#endif
