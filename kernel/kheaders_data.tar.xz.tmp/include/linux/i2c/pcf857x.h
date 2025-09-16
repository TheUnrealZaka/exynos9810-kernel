#ifndef __LINUX_PCF857X_H
#define __LINUX_PCF857X_H


struct pcf857x_platform_data {
	unsigned	gpio_base;
	unsigned	n_latch;

	int		(*setup)(struct i2c_client *client,
					int gpio, unsigned ngpio,
					void *context);
	int		(*teardown)(struct i2c_client *client,
					int gpio, unsigned ngpio,
					void *context);
	void		*context;
};

#endif 
