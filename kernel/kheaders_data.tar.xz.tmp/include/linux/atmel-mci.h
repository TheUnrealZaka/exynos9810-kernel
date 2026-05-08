#ifndef __LINUX_ATMEL_MCI_H
#define __LINUX_ATMEL_MCI_H

#include <linux/types.h>
#include <linux/dmaengine.h>

#define ATMCI_MAX_NR_SLOTS	2


struct mci_slot_pdata {
	unsigned int		bus_width;
	int			detect_pin;
	int			wp_pin;
	bool			detect_is_active_high;
	bool			non_removable;
};


struct mci_platform_data {
	void			*dma_slave;
	dma_filter_fn		dma_filter;
	struct mci_slot_pdata	slot[ATMCI_MAX_NR_SLOTS];
};

#endif 
