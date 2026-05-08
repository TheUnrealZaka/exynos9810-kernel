

#ifndef __LINUX_SND_RT5640_H
#define __LINUX_SND_RT5640_H

struct rt5640_platform_data {
	
	bool in1_diff;
	bool in2_diff;
	bool in3_diff;

	bool dmic_en;
	bool dmic1_data_pin; 
	bool dmic2_data_pin; 

	int ldo1_en; 
};

#endif
