

#ifndef _DMA_HSU_H
#define _DMA_HSU_H

#include <linux/device.h>
#include <linux/interrupt.h>

#include <linux/platform_data/dma-hsu.h>

struct hsu_dma;


struct hsu_dma_chip {
	struct device			*dev;
	int				irq;
	void __iomem			*regs;
	unsigned int			length;
	unsigned int			offset;
	struct hsu_dma			*hsu;
};

#if IS_ENABLED(CONFIG_HSU_DMA)

int hsu_dma_get_status(struct hsu_dma_chip *chip, unsigned short nr,
		       u32 *status);
int hsu_dma_do_irq(struct hsu_dma_chip *chip, unsigned short nr, u32 status);


int hsu_dma_probe(struct hsu_dma_chip *chip);
int hsu_dma_remove(struct hsu_dma_chip *chip);
#else
static inline int hsu_dma_get_status(struct hsu_dma_chip *chip,
				     unsigned short nr, u32 *status)
{
	return 0;
}
static inline int hsu_dma_do_irq(struct hsu_dma_chip *chip, unsigned short nr,
				 u32 status)
{
	return 0;
}
static inline int hsu_dma_probe(struct hsu_dma_chip *chip) { return -ENODEV; }
static inline int hsu_dma_remove(struct hsu_dma_chip *chip) { return 0; }
#endif 

#endif 
