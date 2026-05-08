

#ifndef __CPU_COOLING_H__
#define __CPU_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>


struct cpufreq_cooling_device {
	int id;
	struct thermal_cooling_device *cool_dev;
	unsigned int cpufreq_state;
	unsigned int clipped_freq;
	unsigned int max_level;
	unsigned int *freq_table;	
	struct cpumask allowed_cpus;
	struct list_head node;
	u32 last_load;
	u64 *time_in_idle;
	u64 *time_in_idle_timestamp;
	struct power_table *dyn_power_table;
	int dyn_power_table_entries;
	struct device *cpu_dev;
	get_static_t plat_get_static_power;
	int *var_table;
	int *var_coeff;
	int *asv_coeff;
	unsigned int var_volt_size;
	unsigned int var_temp_size;
};

#ifdef CONFIG_CPU_THERMAL

struct thermal_cooling_device *
cpufreq_cooling_register(const struct cpumask *clip_cpus);

struct thermal_cooling_device *
cpufreq_power_cooling_register(const struct cpumask *clip_cpus,
			       u32 capacitance, get_static_t plat_static_func);


#ifdef CONFIG_THERMAL_OF
struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_cpus);

struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_cpus,
				  u32 capacitance,
				  get_static_t plat_static_func);

struct thermal_cooling_device *
exynos_cpufreq_cooling_register(struct device_node *np, const struct cpumask *clip_cpus);

#else
static inline struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_cpus)
{
	return ERR_PTR(-ENOSYS);
}

static inline struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_cpus,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}

static inline struct thermal_cooling_device *
exynos_cpufreq_cooling_register(struct device_node *np, const struct cpumask *clip_cpus);
{
	return NULL;
}

#endif


void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev);

unsigned long cpufreq_cooling_get_level(unsigned int cpu, unsigned int freq);
#else 
static inline struct thermal_cooling_device *
cpufreq_cooling_register(const struct cpumask *clip_cpus)
{
	return ERR_PTR(-ENOSYS);
}
static inline struct thermal_cooling_device *
cpufreq_power_cooling_register(const struct cpumask *clip_cpus,
			       u32 capacitance, get_static_t plat_static_func)
{
	return NULL;
}

static inline struct thermal_cooling_device *
of_cpufreq_cooling_register(struct device_node *np,
			    const struct cpumask *clip_cpus)
{
	return ERR_PTR(-ENOSYS);
}

static inline struct thermal_cooling_device *
of_cpufreq_power_cooling_register(struct device_node *np,
				  const struct cpumask *clip_cpus,
				  u32 capacitance,
				  get_static_t plat_static_func)
{
	return NULL;
}

static inline struct thermal_cooling_device *
exynos_cpufreq_cooling_register(struct device_node *np, const struct cpumask *clip_cpus)
{
	return NULL;
}

static inline
void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return;
}
static inline
unsigned long cpufreq_cooling_get_level(unsigned int cpu, unsigned int freq)
{
	return THERMAL_CSTATE_INVALID;
}
#endif	

#endif 
