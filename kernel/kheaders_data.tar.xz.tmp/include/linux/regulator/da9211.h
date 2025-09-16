

#ifndef __LINUX_REGULATOR_DA9211_H
#define __LINUX_REGULATOR_DA9211_H

#include <linux/regulator/machine.h>

#define DA9211_MAX_REGULATORS	2

enum da9211_chip_id {
	DA9211,
	DA9212,
	DA9213,
	DA9214,
	DA9215,
};

struct da9211_pdata {
	
	int num_buck;
	int gpio_ren[DA9211_MAX_REGULATORS];
	struct device_node *reg_node[DA9211_MAX_REGULATORS];
	struct regulator_init_data *init_data[DA9211_MAX_REGULATORS];
};
#endif
