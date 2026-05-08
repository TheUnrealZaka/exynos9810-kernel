


#ifndef PINCTRL_ADI2_H
#define PINCTRL_ADI2_H

#include <linux/io.h>
#include <linux/platform_device.h>


struct adi_pinctrl_gpio_platform_data {
	unsigned int port_gpio_base;
	unsigned int port_pin_base;
	unsigned int port_width;
	u8 pinctrl_id;
	u8 pint_id;
	bool pint_assign;
	u8 pint_map;
};

#endif
