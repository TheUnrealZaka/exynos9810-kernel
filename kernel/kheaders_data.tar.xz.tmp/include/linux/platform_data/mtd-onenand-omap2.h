

#ifndef	__MTD_ONENAND_OMAP2_H
#define	__MTD_ONENAND_OMAP2_H

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#define ONENAND_SYNC_READ	(1 << 0)
#define ONENAND_SYNC_READWRITE	(1 << 1)
#define	ONENAND_IN_OMAP34XX	(1 << 2)

struct omap_onenand_platform_data {
	int			cs;
	int			gpio_irq;
	struct mtd_partition	*parts;
	int			nr_parts;
	int			(*onenand_setup)(void __iomem *, int *freq_ptr);
	int			dma_channel;
	u8			flags;
	u8			regulator_can_sleep;
	u8			skip_initial_unlocking;

	
	struct device_node	*of_node;
};
#endif
