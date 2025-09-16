

#ifndef _PPS_GPIO_H
#define _PPS_GPIO_H

struct pps_gpio_platform_data {
	bool assert_falling_edge;
	bool capture_clear;
	unsigned int gpio_pin;
	const char *gpio_label;
};

#endif
