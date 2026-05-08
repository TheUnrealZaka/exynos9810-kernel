

#ifndef __TLV320DAC33_PLAT_H
#define __TLV320DAC33_PLAT_H

struct tlv320dac33_platform_data {
	int power_gpio;
	int mode1_latency; 
	int auto_fifo_config; 
	int keep_bclk;	
	u8 burst_bclkdiv;
};

#endif 
