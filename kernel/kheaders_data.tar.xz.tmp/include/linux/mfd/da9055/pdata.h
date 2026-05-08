
#ifndef __DA9055_PDATA_H
#define __DA9055_PDATA_H

#define DA9055_MAX_REGULATORS	8

struct da9055;

enum gpio_select {
	NO_GPIO = 0,
	GPIO_1,
	GPIO_2
};

struct da9055_pdata {
	int (*init) (struct da9055 *da9055);
	int irq_base;
	int gpio_base;

	struct regulator_init_data *regulators[DA9055_MAX_REGULATORS];
	
	bool reset_enable;
	
	int *gpio_ren;
	
	int *gpio_rsel;
	
	enum gpio_select *reg_ren;
	
	enum gpio_select *reg_rsel;
	
	int *ena_gpio;
};
#endif 
