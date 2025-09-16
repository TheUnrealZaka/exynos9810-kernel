

#ifndef __LINUX_POWER_SBS_BATTERY_H_
#define __LINUX_POWER_SBS_BATTERY_H_

#include <linux/power_supply.h>
#include <linux/types.h>


struct sbs_platform_data {
	u32 i2c_retry_count;
	u32 poll_retry_count;
};

#endif
