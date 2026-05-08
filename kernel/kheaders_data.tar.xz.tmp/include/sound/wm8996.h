

#ifndef __LINUX_SND_WM8996_H
#define __LINUX_SND_WM8996_H

enum wm8996_inmode {
	WM8996_DIFFERRENTIAL_1 = 0,   
	WM8996_INVERTING = 1,         
	WM8996_NON_INVERTING = 2,     
	WM8996_DIFFERENTIAL_2 = 3,    
};


struct wm8996_retune_mobile_config {
	const char *name;
	int rate;
	u16 regs[20];
};

#define WM8996_SET_DEFAULT 0x10000

struct wm8996_pdata {
	int irq_flags;  

	int ldo_ena;  

	int micdet_def;  

	enum wm8996_inmode inl_mode;
	enum wm8996_inmode inr_mode;

	u32 spkmute_seq;  

	int gpio_base;
	u32 gpio_default[5];

	int num_retune_mobile_cfgs;
	struct wm8996_retune_mobile_config *retune_mobile_cfgs;
};

#endif
