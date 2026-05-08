

#ifndef __LINUX_REGULATOR_TPS62360_H
#define __LINUX_REGULATOR_TPS62360_H


struct tps62360_regulator_platform_data {
	struct regulator_init_data *reg_init_data;
	bool en_discharge;
	bool en_internal_pulldn;
	int vsel0_gpio;
	int vsel1_gpio;
	int vsel0_def_state;
	int vsel1_def_state;
};

#endif 
