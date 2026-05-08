

#ifndef __LEDS_MAX77705_RGB_H__
#define __LEDS_MAX77705_RGB_H__

struct max77705_rgb_platform_data {
	char *name[4];
};

extern int get_lcd_info(char *arg);

#endif 
