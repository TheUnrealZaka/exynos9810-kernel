

#ifndef __CPU_COOLING_H__
#define __CPU_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>

#ifdef CONFIG_CLOCK_THERMAL

struct thermal_cooling_device *
clock_cooling_register(struct device *dev, const char *clock_name);


void clock_cooling_unregister(struct thermal_cooling_device *cdev);

unsigned long clock_cooling_get_level(struct thermal_cooling_device *cdev,
				      unsigned long freq);
#else 
static inline struct thermal_cooling_device *
clock_cooling_register(struct device *dev, const char *clock_name)
{
	return NULL;
}
static inline
void clock_cooling_unregister(struct thermal_cooling_device *cdev)
{
}
static inline
unsigned long clock_cooling_get_level(struct thermal_cooling_device *cdev,
				      unsigned long freq)
{
	return THERMAL_CSTATE_INVALID;
}
#endif	

#endif 
