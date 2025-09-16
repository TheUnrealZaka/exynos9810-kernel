

#ifndef __OMAP_PANEL_DATA_H
#define __OMAP_PANEL_DATA_H

#include <video/display_timing.h>


struct connector_atv_platform_data {
	const char *name;
	const char *source;

	bool invert_polarity;
};


struct panel_dpi_platform_data {
	const char *name;
	const char *source;

	int data_lines;

	const struct display_timing *display_timing;

	int backlight_gpio;
	int enable_gpio;
};


struct panel_acx565akm_platform_data {
	const char *name;
	const char *source;

	int reset_gpio;

	int datapairs;
};

#endif 
