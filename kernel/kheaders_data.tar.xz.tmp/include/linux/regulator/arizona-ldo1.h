

#ifndef ARIZONA_LDO1_H
#define ARIZONA_LDO1_H

struct regulator_init_data;

struct arizona_ldo1_pdata {
	
	int ldoena;

	
	const struct regulator_init_data *init_data;
};

#endif
