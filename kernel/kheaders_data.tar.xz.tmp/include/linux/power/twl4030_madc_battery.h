

#ifndef __TWL4030_MADC_BATTERY_H
#define __TWL4030_MADC_BATTERY_H



struct twl4030_madc_bat_calibration {
	short voltage;	
	short level;	
};

struct twl4030_madc_bat_platform_data {
	unsigned int capacity;	
	struct twl4030_madc_bat_calibration *charging;
	int charging_size;
	struct twl4030_madc_bat_calibration *discharging;
	int discharging_size;
};

#endif
