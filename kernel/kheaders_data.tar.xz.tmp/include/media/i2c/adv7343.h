

#ifndef ADV7343_H
#define ADV7343_H

#define ADV7343_COMPOSITE_ID	(0)
#define ADV7343_COMPONENT_ID	(1)
#define ADV7343_SVIDEO_ID	(2)


struct adv7343_power_mode {
	bool sleep_mode;
	bool pll_control;
	u32 dac[6];
};


struct adv7343_sd_config {
	
	u32 sd_dac_out[2];
};


struct adv7343_platform_data {
	struct adv7343_power_mode mode_config;
	struct adv7343_sd_config sd_config;
};

#endif				
