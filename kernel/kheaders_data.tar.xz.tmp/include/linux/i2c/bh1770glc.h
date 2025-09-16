

#ifndef __BH1770_H__
#define __BH1770_H__



struct bh1770_platform_data {
#define BH1770_LED_5mA	0
#define BH1770_LED_10mA	1
#define BH1770_LED_20mA	2
#define BH1770_LED_50mA	3
#define BH1770_LED_100mA 4
#define BH1770_LED_150mA 5
#define BH1770_LED_200mA 6
	__u8 led_def_curr;
#define BH1770_NEUTRAL_GA 16384 
	__u32 glass_attenuation;
	int (*setup_resources)(void);
	int (*release_resources)(void);
};
#endif
