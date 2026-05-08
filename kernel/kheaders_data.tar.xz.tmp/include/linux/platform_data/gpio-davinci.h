

#ifndef __DAVINCI_GPIO_PLATFORM_H
#define __DAVINCI_GPIO_PLATFORM_H

#include <linux/io.h>
#include <linux/spinlock.h>

#include <asm-generic/gpio.h>

struct davinci_gpio_platform_data {
	u32	ngpio;
	u32	gpio_unbanked;
};


struct davinci_gpio_controller {
	struct gpio_chip	chip;
	struct irq_domain	*irq_domain;
	
	spinlock_t		lock;
	void __iomem		*regs;
	void __iomem		*set_data;
	void __iomem		*clr_data;
	void __iomem		*in_data;
	int			gpio_unbanked;
	unsigned		gpio_irq;
};


#define	GPIO(X)		(X)	


#define GPIO_TO_PIN(bank, gpio)	(16 * (bank) + (gpio))

static inline u32 __gpio_mask(unsigned gpio)
{
	return 1 << (gpio % 32);
}
#endif
