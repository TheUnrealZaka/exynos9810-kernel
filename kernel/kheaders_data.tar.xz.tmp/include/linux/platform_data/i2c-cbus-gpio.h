

#ifndef __INCLUDE_LINUX_I2C_CBUS_GPIO_H
#define __INCLUDE_LINUX_I2C_CBUS_GPIO_H

struct i2c_cbus_platform_data {
	int dat_gpio;
	int clk_gpio;
	int sel_gpio;
};

#endif 
