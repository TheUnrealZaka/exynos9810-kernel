

#ifndef __LINUX_SND_RT5677_H
#define __LINUX_SND_RT5677_H

enum rt5677_dmic2_clk {
	RT5677_DMIC_CLK1 = 0,
	RT5677_DMIC_CLK2 = 1,
};


struct rt5677_platform_data {
	
	bool in1_diff;
	bool in2_diff;
	bool lout1_diff;
	bool lout2_diff;
	bool lout3_diff;
	
	enum rt5677_dmic2_clk dmic2_clk_pin;

	
	u8 gpio_config[6];

	
	unsigned int jd1_gpio;
	
	unsigned int jd2_gpio;
	unsigned int jd3_gpio;

	
	bool micbias1_vdd_3v3;
};

#endif
