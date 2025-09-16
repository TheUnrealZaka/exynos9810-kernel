

#ifndef __MAX8903_CHARGER_H__
#define __MAX8903_CHARGER_H__

struct max8903_pdata {
	
	int cen;	
	int dok;	
	int uok;	
	int chg;	
	int flt;	
	int dcm;	
	int usus;	

	
	bool dc_valid;
	
	bool usb_valid;
};

#endif 
