

#ifndef __LINUX_TCA6507_H
#define __LINUX_TCA6507_H
#include <linux/leds.h>

struct tca6507_platform_data {
	struct led_platform_data leds;
#ifdef CONFIG_GPIOLIB
	int gpio_base;
	void (*setup)(unsigned gpio_base, unsigned ngpio);
#endif
};

#define	TCA6507_MAKE_GPIO 1
#endif 
