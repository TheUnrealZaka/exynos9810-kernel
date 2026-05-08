

#ifndef __LINUX_REGULATOR_MACHINE_H_
#define __LINUX_REGULATOR_MACHINE_H_

#include <linux/regulator/consumer.h>
#include <linux/suspend.h>

struct regulator;



#define REGULATOR_CHANGE_VOLTAGE	0x1
#define REGULATOR_CHANGE_CURRENT	0x2
#define REGULATOR_CHANGE_MODE		0x4
#define REGULATOR_CHANGE_STATUS		0x8
#define REGULATOR_CHANGE_DRMS		0x10
#define REGULATOR_CHANGE_BYPASS		0x20


enum regulator_active_discharge {
	REGULATOR_ACTIVE_DISCHARGE_DEFAULT,
	REGULATOR_ACTIVE_DISCHARGE_DISABLE,
	REGULATOR_ACTIVE_DISCHARGE_ENABLE,
};


struct regulator_state {
	int uV;	
	unsigned int mode; 
	int enabled; 
	int disabled; 
};


struct regulation_constraints {

	const char *name;

	
	int min_uV;
	int max_uV;

	int uV_offset;

	
	int min_uA;
	int max_uA;
	int ilim_uA;

	int system_load;

	
	unsigned int valid_modes_mask;

	
	unsigned int valid_ops_mask;

	
	int input_uV;

	
	struct regulator_state state_disk;
	struct regulator_state state_mem;
	struct regulator_state state_standby;
	suspend_state_t initial_state; 

	
	unsigned int initial_mode;

	unsigned int ramp_delay;
	unsigned int enable_time;

	unsigned int active_discharge;

	
	unsigned always_on:1;	
	unsigned boot_on:1;	
	unsigned apply_uV:1;	
	unsigned ramp_disable:1; 
	unsigned soft_start:1;	
	unsigned pull_down:1;	
	unsigned over_current_protection:1; 
};


struct regulator_consumer_supply {
	const char *dev_name;   
	const char *supply;	
};


#define REGULATOR_SUPPLY(_name, _dev_name)			\
{								\
	.supply		= _name,				\
	.dev_name	= _dev_name,				\
}


struct regulator_init_data {
	const char *supply_regulator;        

	struct regulation_constraints constraints;

	int num_consumer_supplies;
	struct regulator_consumer_supply *consumer_supplies;

	
	int (*regulator_init)(void *driver_data);
	void *driver_data;	
};

#ifdef CONFIG_REGULATOR
void regulator_has_full_constraints(void);
int regulator_suspend_prepare(suspend_state_t state);
int regulator_suspend_finish(void);
#else
static inline void regulator_has_full_constraints(void)
{
}
static inline int regulator_suspend_prepare(suspend_state_t state)
{
	return 0;
}
static inline int regulator_suspend_finish(void)
{
	return 0;
}
#endif

#endif
