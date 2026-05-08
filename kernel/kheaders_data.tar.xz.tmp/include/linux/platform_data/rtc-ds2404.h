

#ifndef __LINUX_DS2404_H
#define __LINUX_DS2404_H

struct ds2404_platform_data {

	unsigned int gpio_rst;
	unsigned int gpio_clk;
	unsigned int gpio_dq;
};
#endif
