

#ifndef _EXTCON_ADC_JACK_H_
#define _EXTCON_ADC_JACK_H_ __FILE__

#include <linux/module.h>
#include <linux/extcon.h>


struct adc_jack_cond {
	unsigned int id;
	u32 min_adc;
	u32 max_adc;
};


struct adc_jack_pdata {
	const char *name;
	const char *consumer_channel;

	const unsigned int *cable_names;

	
	struct adc_jack_cond *adc_conditions;

	unsigned long irq_flags;
	unsigned long handling_delay_ms; 
	bool wakeup_source;
};

#endif 
