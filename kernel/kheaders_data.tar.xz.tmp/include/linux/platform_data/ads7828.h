

#ifndef _PDATA_ADS7828_H
#define _PDATA_ADS7828_H


struct ads7828_platform_data {
	bool diff_input;
	bool ext_vref;
	unsigned int vref_mv;
};

#endif 
