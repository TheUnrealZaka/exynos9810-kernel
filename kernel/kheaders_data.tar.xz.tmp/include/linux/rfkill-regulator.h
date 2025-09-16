

#ifndef __LINUX_RFKILL_REGULATOR_H
#define __LINUX_RFKILL_REGULATOR_H



#include <linux/rfkill.h>

struct rfkill_regulator_platform_data {
	char *name;             
	enum rfkill_type type;  
};

#endif 
