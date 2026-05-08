

#ifndef __LINUX_SND_RT5670_H
#define __LINUX_SND_RT5670_H

struct rt5670_platform_data {
	int jd_mode;
	bool in2_diff;
	bool dev_gpio;

	bool dmic_en;
	unsigned int dmic1_data_pin;
	
	unsigned int dmic2_data_pin;
	
	unsigned int dmic3_data_pin;
	
};

#endif
