
#ifndef __LINUX_EDMA_H
#define __LINUX_EDMA_H

struct dma_chan;

#if defined(CONFIG_TI_EDMA) || defined(CONFIG_TI_EDMA_MODULE)
bool edma_filter_fn(struct dma_chan *, void *);
#else
static inline bool edma_filter_fn(struct dma_chan *chan, void *param)
{
	return false;
}
#endif

#endif
