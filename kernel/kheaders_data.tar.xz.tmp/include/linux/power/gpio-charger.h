

#ifndef __LINUX_POWER_GPIO_CHARGER_H__
#define __LINUX_POWER_GPIO_CHARGER_H__

#include <linux/power_supply.h>
#include <linux/types.h>


struct gpio_charger_platform_data {
	const char *name;
	enum power_supply_type type;

	int gpio;
	int gpio_active_low;

	char **supplied_to;
	size_t num_supplicants;
};

#endif
