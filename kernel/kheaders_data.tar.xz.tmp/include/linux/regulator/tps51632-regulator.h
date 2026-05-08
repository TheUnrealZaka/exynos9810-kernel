

#ifndef __LINUX_REGULATOR_TPS51632_H
#define __LINUX_REGULATOR_TPS51632_H


struct tps51632_regulator_platform_data {
	struct regulator_init_data *reg_init_data;
	bool enable_pwm_dvfs;
	bool dvfs_step_20mV;
	int max_voltage_uV;
	int base_voltage_uV;
};

#endif 
