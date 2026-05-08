

#ifndef _AT91_ADC_H_
#define _AT91_ADC_H_

enum atmel_adc_ts_type {
	ATMEL_ADC_TOUCHSCREEN_NONE = 0,
	ATMEL_ADC_TOUCHSCREEN_4WIRE = 4,
	ATMEL_ADC_TOUCHSCREEN_5WIRE = 5,
};


struct at91_adc_trigger {
	const char	*name;
	u8		value;
	bool		is_external;
};


struct at91_adc_data {
	unsigned long			channels_used;
	u8				startup_time;
	struct at91_adc_trigger		*trigger_list;
	u8				trigger_number;
	bool				use_external_triggers;
	u16				vref;
	enum atmel_adc_ts_type		touchscreen_type;
};

extern void __init at91_add_device_adc(struct at91_adc_data *data);
#endif
