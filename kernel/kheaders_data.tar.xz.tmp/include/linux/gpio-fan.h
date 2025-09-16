

#ifndef __LINUX_GPIO_FAN_H
#define __LINUX_GPIO_FAN_H

struct gpio_fan_alarm {
	unsigned	gpio;
	unsigned	active_low;
};

struct gpio_fan_speed {
	int rpm;
	int ctrl_val;
};

struct gpio_fan_platform_data {
	int			num_ctrl;
	unsigned		*ctrl;	
	struct gpio_fan_alarm	*alarm;	
	
	int			num_speed;
	struct gpio_fan_speed	*speed;
};

#endif 
