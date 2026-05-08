

#ifndef __CS42L73_H
#define __CS42L73_H

struct cs42l73_platform_data {
	
	unsigned int reset_gpio;
	unsigned int chgfreq;
	int jack_detection;
	unsigned int mclk_freq;
};

#endif 
