

#ifndef __LINUX_SND_RT5659_H
#define __LINUX_SND_RT5659_H

enum rt5659_dmic1_data_pin {
	RT5659_DMIC1_NULL,
	RT5659_DMIC1_DATA_IN2N,
	RT5659_DMIC1_DATA_GPIO5,
	RT5659_DMIC1_DATA_GPIO9,
	RT5659_DMIC1_DATA_GPIO11,
};

enum rt5659_dmic2_data_pin {
	RT5659_DMIC2_NULL,
	RT5659_DMIC2_DATA_IN2P,
	RT5659_DMIC2_DATA_GPIO6,
	RT5659_DMIC2_DATA_GPIO10,
	RT5659_DMIC2_DATA_GPIO12,
};

enum rt5659_jd_src {
	RT5659_JD_NULL,
	RT5659_JD3,
};

struct rt5659_platform_data {
	bool in1_diff;
	bool in3_diff;
	bool in4_diff;

	int ldo1_en; 
	int reset; 

	enum rt5659_dmic1_data_pin dmic1_data_pin;
	enum rt5659_dmic2_data_pin dmic2_data_pin;
	enum rt5659_jd_src jd_src;
};

#endif

