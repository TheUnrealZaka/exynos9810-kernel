

#ifndef SR030PC30_H
#define SR030PC30_H

struct sr030pc30_platform_data {
	unsigned long clk_rate;	
	int (*set_power)(struct device *dev, int on);
};

#endif 
