

#ifndef MEDIA_M5MOLS_H
#define MEDIA_M5MOLS_H


struct m5mols_platform_data {
	int gpio_reset;
	u8 reset_polarity;
	int (*set_power)(struct device *dev, int on);
};

#endif	
