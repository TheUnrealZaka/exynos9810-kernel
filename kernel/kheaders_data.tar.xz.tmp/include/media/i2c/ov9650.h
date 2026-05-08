
#ifndef OV9650_H_
#define OV9650_H_


struct ov9650_platform_data {
	unsigned long mclk_frequency;
	int gpio_pwdn;
	int gpio_reset;
};
#endif 
