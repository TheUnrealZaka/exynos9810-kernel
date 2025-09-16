

#ifndef __GPU_COOLING_H__
#define __GPU_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>
#include <linux/platform_device.h>

#define GPU_TABLE_END     ~1

#ifdef CONFIG_GPU_THERMAL

struct thermal_cooling_device *
gpufreq_cooling_register(const struct cpumask *clip_gpus);

struct thermal_cooling_device *
gpufreq_power_cooling_register(const struct cpumask *clip_gpus,
			       u32 capacitance, get_static_t plat_static_func);


#ifdef CONFIG_THERMAL_OF
struct thermal_cooling_device *
of_gpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus);

struct thermal_cooling_device *
of_gpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_gpus,
				  u32 capacitance,
				  get_static_t plat_static_func);
#else
static inline struct thermal_cooling_device *
of_gpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus)
{
	return NULL;
}

static inline struct thermal_cooling_device *
of_gpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_gpus,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}
#endif


void gpufreq_cooling_unregister(struct thermal_cooling_device *cdev);

unsigned long gpufreq_cooling_get_level(unsigned int gpu, unsigned int freq);
#else 
static inline struct thermal_cooling_device *
gpufreq_cooling_register(const struct cpumask *clip_gpus)
{
	return NULL;
}

static inline struct thermal_cooling_device *
gpufreq_power_cooling_register(const struct cpumask *clip_gpus,
			       u32 capacitance, get_static_t plat_static_func)
{
	return NULL;
}

static inline struct thermal_cooling_device *
of_gpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus)
{
	return NULL;
}

static inline struct thermal_cooling_device *
of_gpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_gpus,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}

static inline
void gpufreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return;
}
static inline
unsigned long gpufreq_cooling_get_level(unsigned int gpu, unsigned int freq)
{
	return THERMAL_CSTATE_INVALID;
}
#endif	
#ifdef CONFIG_MALI_DVFS
extern int gpu_dvfs_get_clock(int level);
extern int gpu_dvfs_get_voltage(int clock);
extern int gpu_dvfs_get_step(void);
extern int gpu_dvfs_get_cur_clock(void);
extern int gpu_dvfs_get_utilization(void);
extern int gpu_dvfs_get_max_freq(void);
#else
static inline int gpu_dvfs_get_clock(int level) { return 0; }
static inline int gpu_dvfs_get_voltage(int clock) { return 0; }
static inline int gpu_dvfs_get_step(void) { return 0; }
static inline int gpu_dvfs_get_cur_clock(void) { return 0; }
static inline int gpu_dvfs_get_utilization(void) { return 0; }
static inline int gpu_dvfs_get_max_freq(void) { return 0; }
#endif
#endif 
