

#ifndef MAX6697_H
#define MAX6697_H

#include <linux/types.h>


struct max6697_platform_data {
	bool smbus_timeout_disable;	
	bool extended_range_enable;	
	bool beta_compensation;		
	u8 alert_mask;			
	u8 over_temperature_mask;	
	u8 resistance_cancellation;	
	u8 ideality_mask;		
	u8 ideality_value;		
};

#endif 
