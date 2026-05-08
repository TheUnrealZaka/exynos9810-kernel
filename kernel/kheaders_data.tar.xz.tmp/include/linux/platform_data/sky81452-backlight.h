

#ifndef _SKY81452_BACKLIGHT_H
#define _SKY81452_BACKLIGHT_H


struct sky81452_bl_platform_data {
	const char *name;
	int gpio_enable;
	unsigned int enable;
	bool ignore_pwm;
	bool dpwm_mode;
	bool phase_shift;
	unsigned int short_detection_threshold;
	unsigned int boost_current_limit;
};

#endif
