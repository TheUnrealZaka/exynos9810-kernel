

#include <linux/regulator/machine.h>

struct device;

#ifndef MFD_ABX500_H
#define MFD_ABX500_H


struct abx500_init_settings {
	u8 bank;
	u8 reg;
	u8 setting;
};



enum abx500_adc_therm {
	ABx500_ADC_THERM_BATCTRL,
	ABx500_ADC_THERM_BATTEMP,
};


struct abx500_res_to_temp {
	int temp;
	int resist;
};


struct abx500_v_to_cap {
	int voltage;
	int capacity;
};


struct abx500_fg;


struct abx500_fg_parameters {
	int recovery_sleep_timer;
	int recovery_total_time;
	int init_timer;
	int init_discard_time;
	int init_total_time;
	int high_curr_time;
	int accu_charging;
	int accu_high_curr;
	int high_curr_threshold;
	int lowbat_threshold;
	int overbat_threshold;
	int battok_falling_th_sel0;
	int battok_raising_th_sel1;
	int user_cap_limit;
	int maint_thres;
	bool pcut_enable;
	u8 pcut_max_time;
	u8 pcut_flag_time;
	u8 pcut_max_restart;
	u8 pcut_debounce_time;
};


struct abx500_maxim_parameters {
	bool ena_maxi;
	int chg_curr;
	int wait_cycles;
	int charger_curr_step;
};


struct abx500_battery_type {
	int name;
	int resis_high;
	int resis_low;
	int charge_full_design;
	int nominal_voltage;
	int termination_vol;
	int termination_curr;
	int recharge_cap;
	int normal_cur_lvl;
	int normal_vol_lvl;
	int maint_a_cur_lvl;
	int maint_a_vol_lvl;
	int maint_a_chg_timer_h;
	int maint_b_cur_lvl;
	int maint_b_vol_lvl;
	int maint_b_chg_timer_h;
	int low_high_cur_lvl;
	int low_high_vol_lvl;
	int battery_resistance;
	int n_temp_tbl_elements;
	const struct abx500_res_to_temp *r_to_t_tbl;
	int n_v_cap_tbl_elements;
	const struct abx500_v_to_cap *v_to_cap_tbl;
	int n_batres_tbl_elements;
	const struct batres_vs_temp *batres_tbl;
};


struct abx500_bm_capacity_levels {
	int critical;
	int low;
	int normal;
	int high;
	int full;
};


struct abx500_bm_charger_parameters {
	int usb_volt_max;
	int usb_curr_max;
	int ac_volt_max;
	int ac_curr_max;
};


struct abx500_bm_data {
	int temp_under;
	int temp_low;
	int temp_high;
	int temp_over;
	int temp_now;
	int temp_interval_chg;
	int temp_interval_nochg;
	int main_safety_tmr_h;
	int usb_safety_tmr_h;
	int bkup_bat_v;
	int bkup_bat_i;
	bool autopower_cfg;
	bool ac_enabled;
	bool usb_enabled;
	bool usb_power_path;
	bool no_maintenance;
	bool capacity_scaling;
	bool chg_unknown_bat;
	bool enable_overshoot;
	bool auto_trig;
	enum abx500_adc_therm adc_therm;
	int fg_res;
	int n_btypes;
	int batt_id;
	int interval_charging;
	int interval_not_charging;
	int temp_hysteresis;
	int gnd_lift_resistance;
	int n_chg_out_curr;
	int n_chg_in_curr;
	int *chg_output_curr;
	int *chg_input_curr;
	const struct abx500_maxim_parameters *maxi;
	const struct abx500_bm_capacity_levels *cap_levels;
	struct abx500_battery_type *bat_type;
	const struct abx500_bm_charger_parameters *chg_params;
	const struct abx500_fg_parameters *fg_params;
};

enum {
	NTC_EXTERNAL = 0,
	NTC_INTERNAL,
};

int ab8500_bm_of_probe(struct device *dev,
		       struct device_node *np,
		       struct abx500_bm_data *bm);

int abx500_set_register_interruptible(struct device *dev, u8 bank, u8 reg,
	u8 value);
int abx500_get_register_interruptible(struct device *dev, u8 bank, u8 reg,
	u8 *value);
int abx500_get_register_page_interruptible(struct device *dev, u8 bank,
	u8 first_reg, u8 *regvals, u8 numregs);
int abx500_set_register_page_interruptible(struct device *dev, u8 bank,
	u8 first_reg, u8 *regvals, u8 numregs);

int abx500_mask_and_set_register_interruptible(struct device *dev, u8 bank,
	u8 reg, u8 bitmask, u8 bitvalues);
int abx500_get_chip_id(struct device *dev);
int abx500_event_registers_startup_state_get(struct device *dev, u8 *event);
int abx500_startup_irq_enabled(struct device *dev, unsigned int irq);

struct abx500_ops {
	int (*get_chip_id) (struct device *);
	int (*get_register) (struct device *, u8, u8, u8 *);
	int (*set_register) (struct device *, u8, u8, u8);
	int (*get_register_page) (struct device *, u8, u8, u8 *, u8);
	int (*set_register_page) (struct device *, u8, u8, u8 *, u8);
	int (*mask_and_set_register) (struct device *, u8, u8, u8, u8);
	int (*event_registers_startup_state_get) (struct device *, u8 *);
	int (*startup_irq_enabled) (struct device *, unsigned int);
	void (*dump_all_banks) (struct device *);
};

int abx500_register_ops(struct device *core_dev, struct abx500_ops *ops);
void abx500_remove_ops(struct device *dev);
#endif
