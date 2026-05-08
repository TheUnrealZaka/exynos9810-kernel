
#ifndef __EXTCON_GPIO_H__
#define __EXTCON_GPIO_H__ __FILE__

#include <linux/extcon.h>


struct gpio_extcon_pdata {
	unsigned int extcon_id;
	unsigned gpio;
	bool gpio_active_low;
	unsigned long debounce;
	unsigned long irq_flags;

	bool check_on_resume;
};

#endif 
