

#ifndef __LINUX_REGULATOR_MAX8660_H
#define __LINUX_REGULATOR_MAX8660_H

#include <linux/regulator/machine.h>

enum {
	MAX8660_V3,
	MAX8660_V4,
	MAX8660_V5,
	MAX8660_V6,
	MAX8660_V7,
	MAX8660_V_END,
};


struct max8660_subdev_data {
	int				id;
	const char			*name;
	struct regulator_init_data	*platform_data;
};


struct max8660_platform_data {
	int num_subdevs;
	struct max8660_subdev_data *subdevs;
	unsigned en34_is_high:1;
};
#endif
