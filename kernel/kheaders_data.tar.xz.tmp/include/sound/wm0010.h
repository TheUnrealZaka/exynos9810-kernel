

#ifndef WM0010_PDATA_H
#define WM0010_PDATA_H

struct wm0010_pdata {
	int gpio_reset;

	
	int reset_active_high;
	int irq_flags;
};

#endif
