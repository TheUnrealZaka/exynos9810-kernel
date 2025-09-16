

#ifndef __LINUX_MFD_MAX8998_H
#define __LINUX_MFD_MAX8998_H

#include <linux/regulator/machine.h>


enum {
	MAX8998_LDO2 = 2,
	MAX8998_LDO3,
	MAX8998_LDO4,
	MAX8998_LDO5,
	MAX8998_LDO6,
	MAX8998_LDO7,
	MAX8998_LDO8,
	MAX8998_LDO9,
	MAX8998_LDO10,
	MAX8998_LDO11,
	MAX8998_LDO12,
	MAX8998_LDO13,
	MAX8998_LDO14,
	MAX8998_LDO15,
	MAX8998_LDO16,
	MAX8998_LDO17,
	MAX8998_BUCK1,
	MAX8998_BUCK2,
	MAX8998_BUCK3,
	MAX8998_BUCK4,
	MAX8998_EN32KHZ_AP,
	MAX8998_EN32KHZ_CP,
	MAX8998_ENVICHG,
	MAX8998_ESAFEOUT1,
	MAX8998_ESAFEOUT2,
};


struct max8998_regulator_data {
	int				id;
	struct regulator_init_data	*initdata;
	struct device_node		*reg_node;
};


struct max8998_platform_data {
	struct max8998_regulator_data	*regulators;
	int				num_regulators;
	unsigned int			irq_base;
	int				ono;
	bool				buck_voltage_lock;
	int				buck1_voltage[4];
	int				buck2_voltage[2];
	int				buck1_set1;
	int				buck1_set2;
	int				buck1_default_idx;
	int				buck2_set3;
	int				buck2_default_idx;
	bool				wakeup;
	bool				rtc_delay;
	int				eoc;
	int				restart;
	int				timeout;
};

#endif 
