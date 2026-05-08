

#ifndef __LINUX_PLATFORM_DATA_I2C_MUX_REG_H
#define __LINUX_PLATFORM_DATA_I2C_MUX_REG_H


struct i2c_mux_reg_platform_data {
	int parent;
	int base_nr;
	const unsigned int *values;
	int n_values;
	bool little_endian;
	bool write_only;
	const unsigned int *classes;
	u32 idle;
	bool idle_in_use;
	void __iomem *reg;
	resource_size_t reg_size;
};

#endif	
