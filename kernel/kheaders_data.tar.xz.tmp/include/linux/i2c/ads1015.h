

#ifndef LINUX_ADS1015_H
#define LINUX_ADS1015_H

#define ADS1015_CHANNELS 8

struct ads1015_channel_data {
	bool enabled;
	unsigned int pga;
	unsigned int data_rate;
};

struct ads1015_platform_data {
	struct ads1015_channel_data channel_data[ADS1015_CHANNELS];
};

#endif 
