

#ifndef ARIZONA_MICSUPP_H
#define ARIZONA_MICSUPP_H

struct regulator_init_data;

struct arizona_micsupp_pdata {
	
	const struct regulator_init_data *init_data;
};

struct arizona_micsupp_forced_bypass {
	bool regulated;
	bool enabled;
	bool forced;
	struct mutex lock;
};
#endif
