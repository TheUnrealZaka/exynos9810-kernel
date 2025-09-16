

#ifndef _PDATA_SHT15_H
#define _PDATA_SHT15_H


struct sht15_platform_data {
	int gpio_data;
	int gpio_sck;
	int supply_mv;
	bool checksum;
	bool no_otp_reload;
	bool low_resolution;
};

#endif 
