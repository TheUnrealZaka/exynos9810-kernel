

#ifndef __LINUX_I2C_TPS6507X_TS_H
#define __LINUX_I2C_TPS6507X_TS_H


struct touchscreen_init_data {
	int	poll_period;	
	__u16	min_pressure;	
	__u16	vendor;
	__u16	product;
	__u16	version;
};

#endif 
