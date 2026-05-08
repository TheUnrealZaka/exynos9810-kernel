

#ifndef __AUO_PIXCIR_TS_H__
#define __AUO_PIXCIR_TS_H__


#define AUO_PIXCIR_INT_PERIODICAL	0x00
#define AUO_PIXCIR_INT_COMP_COORD	0x01
#define AUO_PIXCIR_INT_TOUCH_IND	0x02


struct auo_pixcir_ts_platdata {
	int gpio_int;
	int gpio_rst;

	int int_setting;

	unsigned int x_max;
	unsigned int y_max;
};

#endif
