

#ifndef _BU21013_H
#define _BU21013_H


struct bu21013_platform_device {
	int touch_x_max;
	int touch_y_max;
	unsigned int cs_pin;
	unsigned int touch_pin;
	bool ext_clk;
	bool x_flip;
	bool y_flip;
	bool wakeup;
};

#endif
