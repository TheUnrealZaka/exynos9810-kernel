

#ifndef _PMBUS_H_
#define _PMBUS_H_




#define PMBUS_SKIP_STATUS_CHECK	(1 << 0)

struct pmbus_platform_data {
	u32 flags;		

	
	int num_regulators;
	struct regulator_init_data *reg_init_data;
};

#endif 
