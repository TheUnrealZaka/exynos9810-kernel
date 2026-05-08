
#ifndef MEDIA_S5C73M3__
#define MEDIA_S5C73M3__

#include <linux/videodev2.h>
#include <media/v4l2-mediabus.h>


struct s5c73m3_gpio {
	int gpio;
	int level;
};



struct s5c73m3_platform_data {
	unsigned long mclk_frequency;

	struct s5c73m3_gpio gpio_reset;
	struct s5c73m3_gpio gpio_stby;

	enum v4l2_mbus_type bus_type;
	u8 nlanes;
	u8 horiz_flip;
	u8 vert_flip;
};

#endif 
