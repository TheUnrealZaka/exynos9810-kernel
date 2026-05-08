#ifndef _ASM_GENERIC_GPIO_H
#define _ASM_GENERIC_GPIO_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/of.h>

#ifdef CONFIG_GPIOLIB

#include <linux/compiler.h>
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>



#ifndef ARCH_NR_GPIOS
#if defined(CONFIG_ARCH_NR_GPIO) && CONFIG_ARCH_NR_GPIO > 0
#define ARCH_NR_GPIOS CONFIG_ARCH_NR_GPIO
#else
#define ARCH_NR_GPIOS		512
#endif
#endif



static inline bool gpio_is_valid(int number)
{
	return number >= 0 && number < ARCH_NR_GPIOS;
}

struct device;
struct gpio;
struct seq_file;
struct module;
struct device_node;
struct gpio_desc;


static inline struct gpio_chip *gpio_to_chip(unsigned gpio)
{
	return gpiod_to_chip(gpio_to_desc(gpio));
}


extern int gpio_request(unsigned gpio, const char *label);
extern void gpio_free(unsigned gpio);

static inline int gpio_direction_input(unsigned gpio)
{
	return gpiod_direction_input(gpio_to_desc(gpio));
}
static inline int gpio_direction_output(unsigned gpio, int value)
{
	return gpiod_direction_output_raw(gpio_to_desc(gpio), value);
}

static inline int gpio_set_debounce(unsigned gpio, unsigned debounce)
{
	return gpiod_set_debounce(gpio_to_desc(gpio), debounce);
}

static inline int gpio_get_value_cansleep(unsigned gpio)
{
	return gpiod_get_raw_value_cansleep(gpio_to_desc(gpio));
}
static inline void gpio_set_value_cansleep(unsigned gpio, int value)
{
	return gpiod_set_raw_value_cansleep(gpio_to_desc(gpio), value);
}



static inline int __gpio_get_value(unsigned gpio)
{
	return gpiod_get_raw_value(gpio_to_desc(gpio));
}
static inline void __gpio_set_value(unsigned gpio, int value)
{
	return gpiod_set_raw_value(gpio_to_desc(gpio), value);
}

static inline int __gpio_cansleep(unsigned gpio)
{
	return gpiod_cansleep(gpio_to_desc(gpio));
}

static inline int __gpio_to_irq(unsigned gpio)
{
	return gpiod_to_irq(gpio_to_desc(gpio));
}

extern int gpio_request_one(unsigned gpio, unsigned long flags, const char *label);
extern int gpio_request_array(const struct gpio *array, size_t num);
extern void gpio_free_array(const struct gpio *array, size_t num);


static inline int gpio_export(unsigned gpio, bool direction_may_change)
{
	return gpiod_export(gpio_to_desc(gpio), direction_may_change);
}

static inline int gpio_export_link(struct device *dev, const char *name,
				   unsigned gpio)
{
	return gpiod_export_link(dev, name, gpio_to_desc(gpio));
}

static inline void gpio_unexport(unsigned gpio)
{
	gpiod_unexport(gpio_to_desc(gpio));
}

#else	

static inline bool gpio_is_valid(int number)
{
	
	return number >= 0;
}



static inline int gpio_cansleep(unsigned gpio)
{
	return 0;
}

static inline int gpio_get_value_cansleep(unsigned gpio)
{
	might_sleep();
	return __gpio_get_value(gpio);
}

static inline void gpio_set_value_cansleep(unsigned gpio, int value)
{
	might_sleep();
	__gpio_set_value(gpio, value);
}

#endif 

#endif 
