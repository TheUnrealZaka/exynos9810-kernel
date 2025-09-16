

#ifndef _LINUX_I2C_MUX_PINCTRL_H
#define _LINUX_I2C_MUX_PINCTRL_H


struct i2c_mux_pinctrl_platform_data {
	int parent_bus_num;
	int base_bus_num;
	int bus_count;
	const char **pinctrl_states;
	const char *pinctrl_state_idle;
};

#endif
