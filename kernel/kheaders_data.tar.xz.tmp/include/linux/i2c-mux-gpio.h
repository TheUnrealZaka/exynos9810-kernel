

#ifndef _LINUX_I2C_MUX_GPIO_H
#define _LINUX_I2C_MUX_GPIO_H


#define I2C_MUX_GPIO_NO_IDLE	((unsigned)-1)


struct i2c_mux_gpio_platform_data {
	int parent;
	int base_nr;
	const unsigned *values;
	int n_values;
	const unsigned *classes;
	char *gpio_chip;
	const unsigned *gpios;
	int n_gpios;
	unsigned idle;
};

#endif 
