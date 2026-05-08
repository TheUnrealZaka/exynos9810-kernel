

#ifndef S5K6AA_H
#define S5K6AA_H

#include <media/v4l2-mediabus.h>


struct s5k6aa_gpio {
	int gpio;
	int level;
};



struct s5k6aa_platform_data {
	int (*set_power)(int enable);
	unsigned long mclk_frequency;
	struct s5k6aa_gpio gpio_reset;
	struct s5k6aa_gpio gpio_stby;
	enum v4l2_mbus_type bus_type;
	u8 nlanes;
	u8 horiz_flip;
	u8 vert_flip;
};

#endif 
