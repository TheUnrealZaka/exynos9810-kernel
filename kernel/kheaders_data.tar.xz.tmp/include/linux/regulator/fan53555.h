

#ifndef __FAN53555_H__
#define __FAN53555_H__


enum {
	FAN53555_VSEL_ID_0 = 0,
	FAN53555_VSEL_ID_1,
};


enum {
	FAN53555_SLEW_RATE_64MV = 0,
	FAN53555_SLEW_RATE_32MV,
	FAN53555_SLEW_RATE_16MV,
	FAN53555_SLEW_RATE_8MV,
	FAN53555_SLEW_RATE_4MV,
	FAN53555_SLEW_RATE_2MV,
	FAN53555_SLEW_RATE_1MV,
	FAN53555_SLEW_RATE_0_5MV,
};

struct fan53555_platform_data {
	struct regulator_init_data *regulator;
	unsigned int slew_rate;
	
	unsigned int sleep_vsel_id;
};

#endif 
