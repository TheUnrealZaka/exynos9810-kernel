

#ifndef __SOUND_MAX9768_PDATA_H__
#define __SOUND_MAX9768_PDATA_H__


struct max9768_pdata {
	int shdn_gpio;
	int mute_gpio;
	unsigned flags;
#define MAX9768_FLAG_CLASSIC_PWM	(1 << 0)
};

#endif 
