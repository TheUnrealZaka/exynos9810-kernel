
#ifndef _LINUX_NTC_H
#define _LINUX_NTC_H

struct iio_channel;

enum ntc_thermistor_type {
	TYPE_NCPXXWB473,
	TYPE_NCPXXWL333,
	TYPE_B57330V2103,
	TYPE_NCPXXWF104,
	TYPE_NCPXXXH103,
};

struct ntc_thermistor_platform_data {
	
	int (*read_uv)(struct ntc_thermistor_platform_data *);
	unsigned int pullup_uv;

	unsigned int pullup_ohm;
	unsigned int pulldown_ohm;
	enum { NTC_CONNECTED_POSITIVE, NTC_CONNECTED_GROUND } connect;
	struct iio_channel *chan;

	int (*read_ohm)(void);
};

#endif 
