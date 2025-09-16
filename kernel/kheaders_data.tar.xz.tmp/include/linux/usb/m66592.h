

#ifndef __LINUX_USB_M66592_H
#define __LINUX_USB_M66592_H

#define M66592_PLATDATA_XTAL_12MHZ	0x01
#define M66592_PLATDATA_XTAL_24MHZ	0x02
#define M66592_PLATDATA_XTAL_48MHZ	0x03

struct m66592_platdata {
	
	unsigned	on_chip:1;

	
	unsigned	endian:1;

	
	unsigned	xtal:2;

	
	unsigned	vif:1;

	
	unsigned	wr0_shorted_to_wr1:1;
};

#endif 

