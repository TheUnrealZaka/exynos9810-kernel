

#ifndef __LINUX_LEDS_REGULATOR_H
#define __LINUX_LEDS_REGULATOR_H



#include <linux/leds.h>

struct led_regulator_platform_data {
	char *name;                     
	enum led_brightness brightness; 
};

#endif 
