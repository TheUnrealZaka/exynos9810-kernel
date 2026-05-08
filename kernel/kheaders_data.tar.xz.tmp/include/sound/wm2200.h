

#ifndef __LINUX_SND_WM2200_H
#define __LINUX_SND_WM2200_H

#define WM2200_GPIO_SET 0x10000
#define WM2200_MAX_MICBIAS 2

enum wm2200_in_mode {
	WM2200_IN_SE = 0,
	WM2200_IN_DIFF = 1,
	WM2200_IN_DMIC = 2,
};

enum wm2200_dmic_sup {
	WM2200_DMIC_SUP_MICVDD = 0,
	WM2200_DMIC_SUP_MICBIAS1 = 1,
	WM2200_DMIC_SUP_MICBIAS2 = 2,
};

enum wm2200_mbias_lvl {
	WM2200_MBIAS_LVL_1V5 = 1,
	WM2200_MBIAS_LVL_1V8 = 2,
	WM2200_MBIAS_LVL_1V9 = 3,
	WM2200_MBIAS_LVL_2V0 = 4,
	WM2200_MBIAS_LVL_2V2 = 5,
	WM2200_MBIAS_LVL_2V4 = 6,
	WM2200_MBIAS_LVL_2V5 = 7,
	WM2200_MBIAS_LVL_2V6 = 8,
};

struct wm2200_micbias {
	enum wm2200_mbias_lvl mb_lvl;      
	unsigned int discharge:1;          
	unsigned int fast_start:1;         
	unsigned int bypass:1;             
};

struct wm2200_pdata {
	int reset;      
	int ldo_ena;    
	int irq_flags;

	int gpio_defaults[4];

	enum wm2200_in_mode in_mode[3];
	enum wm2200_dmic_sup dmic_sup[3];

	
	struct wm2200_micbias micbias[WM2200_MAX_MICBIAS];
};

#endif
