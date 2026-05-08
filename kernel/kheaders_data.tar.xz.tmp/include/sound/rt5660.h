

#ifndef __LINUX_SND_RT5660_H
#define __LINUX_SND_RT5660_H

enum rt5660_dmic1_data_pin {
	RT5660_DMIC1_NULL,
	RT5660_DMIC1_DATA_GPIO2,
	RT5660_DMIC1_DATA_IN1P,
};

struct rt5660_platform_data {
	
	bool in1_diff;
	bool in3_diff;
	bool use_ldo2;
	bool poweroff_codec_in_suspend;

	enum rt5660_dmic1_data_pin dmic1_data_pin;
};

#endif
