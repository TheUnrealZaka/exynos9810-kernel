

#ifndef _LINUX_TIMB_GPIO_H
#define _LINUX_TIMB_GPIO_H


struct timbgpio_platform_data {
	int gpio_base;
	int nr_pins;
	int irq_base;
};

#endif
