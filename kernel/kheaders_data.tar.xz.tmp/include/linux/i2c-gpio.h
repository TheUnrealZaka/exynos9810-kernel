
#ifndef _LINUX_I2C_GPIO_H
#define _LINUX_I2C_GPIO_H


struct i2c_gpio_platform_data {
	unsigned int	sda_pin;
	unsigned int	scl_pin;
	int		udelay;
	int		timeout;
	unsigned int	sda_is_open_drain:1;
	unsigned int	scl_is_open_drain:1;
	unsigned int	scl_is_output_only:1;
};

#endif 
