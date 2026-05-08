

#ifndef _TIMB_RADIO_
#define _TIMB_RADIO_ 1

#include <linux/i2c.h>

struct timb_radio_platform_data {
	int i2c_adapter; 
	struct i2c_board_info *tuner;
	struct i2c_board_info *dsp;
};

#endif
