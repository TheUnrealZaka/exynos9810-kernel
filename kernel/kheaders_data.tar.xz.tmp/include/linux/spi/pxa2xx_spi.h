
#ifndef __linux_pxa2xx_spi_h
#define __linux_pxa2xx_spi_h

#include <linux/pxa2xx_ssp.h>

#define PXA2XX_CS_ASSERT (0x01)
#define PXA2XX_CS_DEASSERT (0x02)

struct dma_chan;


struct pxa2xx_spi_master {
	u16 num_chipselect;
	u8 enable_dma;

	
	bool (*dma_filter)(struct dma_chan *chan, void *param);
	void *tx_param;
	void *rx_param;

	
	struct ssp_device ssp;
};


struct pxa2xx_spi_chip {
	u8 tx_threshold;
	u8 tx_hi_threshold;
	u8 rx_threshold;
	u8 dma_burst_size;
	u32 timeout;
	u8 enable_loopback;
	int gpio_cs;
	void (*cs_control)(u32 command);
};

#if defined(CONFIG_ARCH_PXA) || defined(CONFIG_ARCH_MMP)

#include <linux/clk.h>

extern void pxa2xx_set_spi_info(unsigned id, struct pxa2xx_spi_master *info);

#endif
#endif
