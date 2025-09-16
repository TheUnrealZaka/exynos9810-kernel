
#ifndef __UX500_WDT_H
#define __UX500_WDT_H


struct ux500_wdt_data {
	unsigned int timeout;
	bool has_28_bits_resolution;
};

#endif 
