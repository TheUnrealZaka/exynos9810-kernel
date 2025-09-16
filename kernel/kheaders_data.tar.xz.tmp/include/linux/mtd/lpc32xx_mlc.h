

#ifndef __LINUX_MTD_LPC32XX_MLC_H
#define __LINUX_MTD_LPC32XX_MLC_H

#include <linux/dmaengine.h>

struct lpc32xx_mlc_platform_data {
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
};

#endif  
