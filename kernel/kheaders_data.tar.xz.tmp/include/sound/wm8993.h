

#ifndef __LINUX_SND_WM8993_H
#define __LINUX_SND_WM8993_H


struct wm8993_retune_mobile_setting {
	const char *name;
	unsigned int rate;
	u16 config[24];
};

struct wm8993_platform_data {
	struct wm8993_retune_mobile_setting *retune_configs;
	int num_retune_configs;

	
	unsigned int lineout1_diff:1;
	unsigned int lineout2_diff:1;

	
	unsigned int lineout1fb:1;
	unsigned int lineout2fb:1;

	
	int micbias1_delay;
	int micbias2_delay;

	
	unsigned int micbias1_lvl:1;
	unsigned int micbias2_lvl:1;

	
	unsigned int jd_scthr:2;
	unsigned int jd_thr:2;
};

#endif
