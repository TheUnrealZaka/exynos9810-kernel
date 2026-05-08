
#ifndef _LINUX_DAVINCI_EMAC_H
#define _LINUX_DAVINCI_EMAC_H

#include <linux/if_ether.h>
#include <linux/nvmem-consumer.h>

struct mdio_platform_data {
	unsigned long		bus_freq;
};

struct emac_platform_data {
	char mac_addr[ETH_ALEN];
	u32 ctrl_reg_offset;
	u32 ctrl_mod_reg_offset;
	u32 ctrl_ram_offset;
	u32 hw_ram_addr;
	u32 ctrl_ram_size;

	
	const char *phy_id;

	u8 rmii_en;
	u8 version;
	bool no_bd_ram;
	void (*interrupt_enable) (void);
	void (*interrupt_disable) (void);
};

enum {
	EMAC_VERSION_1,	
	EMAC_VERSION_2,	
};

void davinci_get_mac_addr(struct nvmem_device *nvmem, void *context);
#endif
