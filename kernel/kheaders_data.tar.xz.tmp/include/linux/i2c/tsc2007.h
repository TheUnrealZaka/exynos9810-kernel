#ifndef __LINUX_I2C_TSC2007_H
#define __LINUX_I2C_TSC2007_H



struct tsc2007_platform_data {
	u16	model;				
	u16	x_plate_ohms;	
	u16	max_rt; 
	unsigned long poll_period; 
	int	fuzzx; 
	int	fuzzy;
	int	fuzzz;

	int	(*get_pendown_state)(struct device *);
	
	void	(*clear_penirq)(void);
	int	(*init_platform_hw)(void);
	void	(*exit_platform_hw)(void);
};

#endif
