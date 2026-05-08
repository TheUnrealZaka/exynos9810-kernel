

#ifndef __LINUX_MDIO_GPIO_H
#define __LINUX_MDIO_GPIO_H

#include <linux/mdio-bitbang.h>

struct mdio_gpio_platform_data {
	
	unsigned int mdc;
	unsigned int mdio;
	unsigned int mdo;

	bool mdc_active_low;
	bool mdio_active_low;
	bool mdo_active_low;

	u32 phy_mask;
	u32 phy_ignore_ta_mask;
	int irqs[PHY_MAX_ADDR];
	
	int (*reset)(struct mii_bus *bus);
};

#endif 
