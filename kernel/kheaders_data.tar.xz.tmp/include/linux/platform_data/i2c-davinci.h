

#ifndef __ASM_ARCH_I2C_H
#define __ASM_ARCH_I2C_H


struct davinci_i2c_platform_data {
	unsigned int	bus_freq;	
	unsigned int	bus_delay;	
	unsigned int    sda_pin;        
	unsigned int    scl_pin;        
	bool		has_pfunc;	
};


void davinci_init_i2c(struct davinci_i2c_platform_data *);

#endif 
