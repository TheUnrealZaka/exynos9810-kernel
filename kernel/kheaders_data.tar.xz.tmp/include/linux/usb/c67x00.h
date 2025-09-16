

#ifndef _LINUX_USB_C67X00_H
#define _LINUX_USB_C67X00_H


#define C67X00_SIE_UNUSED	0
#define C67X00_SIE_HOST		1
#define C67X00_SIE_PERIPHERAL_A	2	
#define C67X00_SIE_PERIPHERAL_B	3	

#define c67x00_sie_config(config, n)  (((config)>>(4*(n)))&0x3)

#define C67X00_SIE1_UNUSED	        (C67X00_SIE_UNUSED		<< 0)
#define C67X00_SIE1_HOST	        (C67X00_SIE_HOST		<< 0)
#define C67X00_SIE1_PERIPHERAL_A	(C67X00_SIE_PERIPHERAL_A	<< 0)
#define C67X00_SIE1_PERIPHERAL_B	(C67X00_SIE_PERIPHERAL_B	<< 0)

#define C67X00_SIE2_UNUSED		(C67X00_SIE_UNUSED		<< 4)
#define C67X00_SIE2_HOST		(C67X00_SIE_HOST		<< 4)
#define C67X00_SIE2_PERIPHERAL_A	(C67X00_SIE_PERIPHERAL_A	<< 4)
#define C67X00_SIE2_PERIPHERAL_B	(C67X00_SIE_PERIPHERAL_B	<< 4)

struct c67x00_platform_data {
	int sie_config;			
	unsigned long hpi_regstep;	
};

#endif 
