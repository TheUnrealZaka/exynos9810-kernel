

#ifndef __OV7670_H
#define __OV7670_H

struct ov7670_config {
	int min_width;			
	int min_height;			
	int clock_speed;		
	bool use_smbus;			
	bool pll_bypass;		
	bool pclk_hb_disable;		
};

#endif
