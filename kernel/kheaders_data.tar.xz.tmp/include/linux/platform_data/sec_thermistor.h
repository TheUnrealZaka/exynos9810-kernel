
#ifndef __LINUX_SEC_THERMISTOR_H
#define __LINUX_SEC_THERMISTOR_H __FILE__


struct sec_therm_adc_table {
	int adc;
	int temperature;
};


struct sec_therm_platform_data {
	unsigned int adc_channel;
	unsigned int adc_arr_size;
	struct sec_therm_adc_table *adc_table;
};

#endif 
