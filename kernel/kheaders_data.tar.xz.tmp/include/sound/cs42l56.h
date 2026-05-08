

#ifndef __CS42L56_H
#define __CS42L56_H

struct cs42l56_platform_data {

	
	unsigned int gpio_nreset;

	
	unsigned int micbias_lvl;

	
	unsigned int ain1a_ref_cfg;

	
	unsigned int ain2a_ref_cfg;

	
	unsigned int ain1b_ref_cfg;

	
	unsigned int ain2b_ref_cfg;

	
	unsigned int chgfreq;

	
	unsigned int hpfb_freq;

	
	unsigned int hpfa_freq;

	
	unsigned int adaptive_pwr;

};

#endif 
