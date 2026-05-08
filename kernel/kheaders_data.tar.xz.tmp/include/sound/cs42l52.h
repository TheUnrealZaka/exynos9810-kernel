

#ifndef __CS42L52_H
#define __CS42L52_H

struct cs42l52_platform_data {

	
	unsigned int micbias_lvl;

	
	bool mica_diff_cfg;

	
	bool micb_diff_cfg;

	
	unsigned int chgfreq;

	
	unsigned int reset_gpio;
};

#endif 
