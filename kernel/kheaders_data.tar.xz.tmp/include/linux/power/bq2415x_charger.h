

#ifndef BQ2415X_CHARGER_H
#define BQ2415X_CHARGER_H




enum bq2415x_mode {
	BQ2415X_MODE_OFF,		
	BQ2415X_MODE_NONE,		
	BQ2415X_MODE_HOST_CHARGER,	
	BQ2415X_MODE_DEDICATED_CHARGER, 
	BQ2415X_MODE_BOOST,		
};

struct bq2415x_platform_data {
	int current_limit;		
	int weak_battery_voltage;	
	int battery_regulation_voltage;	
	int charge_current;		
	int termination_current;	
	int resistor_sense;		
	const char *notify_device;	
};

#endif
