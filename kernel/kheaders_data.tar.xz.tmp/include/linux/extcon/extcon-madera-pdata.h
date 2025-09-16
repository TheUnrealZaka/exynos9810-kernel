

#ifndef EXTCON_MADERA_PDATA_H
#define EXTCON_MADERA_PDATA_H

#include <linux/kernel.h>

#define MADERA_MAX_ACCESSORY		1


#define MADERA_HP_Z_OPEN		INT_MAX

struct madera_jd_state;


struct madera_micd_config {
	
	u32 src;

	
	u32 gnd;

	
	u32 bias;

	
	bool gpio;

	
	u32 hp_gnd;
};

struct madera_micd_range {
	int max;  
	int key;  
};


struct madera_accdet_pdata {
	
	bool enabled;

	
	u32 output;

	
	u32 jd_wake_time;

	
	bool jd_use_jd2;

	
	bool jd_invert;

	
	u32 fixed_hpdet_imp_x100;

	
	u32 hpdet_ext_res_x100;

	
	u32 hpdet_short_circuit_imp;

	
	u32 hpdet_channel;

	
	u32 micd_detect_debounce_ms;

	
	u32 micd_manual_debounce;

	
	int micd_pol_gpio;

	
	u32 micd_bias_start_time;

	
	u32 micd_rate;

	
	u32 micd_dbtime;

	
	u32 micd_timeout_ms;

	
	u32 micd_clamp_mode;

	
	bool micd_force_micbias;

	
	bool micd_open_circuit_declare;

	
	bool micd_software_compare;

	
	const struct madera_micd_range *micd_ranges;
	int num_micd_ranges;

	
	const struct madera_micd_config *micd_configs;
	int num_micd_configs;

	
	u32 hpd_pins[4];

	
	u32 init_delay;

	
	u32 hs_mic;

	
	u32 moisture_pin;
	
	u32 moisture_imp;
	
	u32 moisture_debounce;

	
	const struct madera_jd_state *custom_jd;
};

#endif
