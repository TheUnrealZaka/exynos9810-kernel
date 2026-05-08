

#ifndef __CS4271_H
#define __CS4271_H

struct cs4271_platform_data {
	int gpio_nreset;	
	bool amutec_eq_bmutec;	

	
	bool enable_soft_reset;
};

#endif 
