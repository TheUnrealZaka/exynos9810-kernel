



#ifndef _LINUX_I2C_XIIC_H
#define _LINUX_I2C_XIIC_H


struct xiic_i2c_platform_data {
	u8				num_devices;
	struct i2c_board_info const	*devices;
};

#endif 
