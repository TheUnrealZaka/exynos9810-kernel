

#ifndef S5K4ECGX_H
#define S5K4ECGX_H


struct s5k4ecgx_gpio {
	int gpio;
	int level;
};



struct s5k4ecgx_platform_data {
	struct s5k4ecgx_gpio gpio_reset;
	struct s5k4ecgx_gpio gpio_stby;
};

#endif 
