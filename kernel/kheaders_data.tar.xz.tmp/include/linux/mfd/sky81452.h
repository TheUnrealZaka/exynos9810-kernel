

#ifndef _SKY81452_H
#define _SKY81452_H

#include <linux/platform_data/sky81452-backlight.h>
#include <linux/regulator/machine.h>

struct sky81452_platform_data {
	struct sky81452_bl_platform_data *bl_pdata;
	struct regulator_init_data *regulator_init_data;
};

#endif
