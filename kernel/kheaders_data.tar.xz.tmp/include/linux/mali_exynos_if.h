/* SPDX-License-Identifier: GPL-2.0 */



#ifndef _MALI_EXYNOS_IF_H_
#define _MALI_EXYNOS_IF_H_

#ifdef CONFIG_MALI_EXYNOS_DVFS
int gpu_dvfs_get_clock(int level);
int gpu_dvfs_get_voltage(int clock);
int gpu_dvfs_get_step(void);
int gpu_dvfs_get_cur_clock(void);
int gpu_dvfs_get_utilization(void);
int gpu_dvfs_get_max_freq(void);
int gpu_dvfs_get_sustainable_info_array(int index);
int gpu_dvfs_get_max_lock(void); // This is also declared in internal interface
bool gpu_dvfs_get_need_cpu_qos(void);
#else
static inline int gpu_dvfs_get_clock(int level)
{
	return 0;
}
static inline int gpu_dvfs_get_voltage(int clock)
{
	return 0;
}
static inline int gpu_dvfs_get_step(void)
{
	return 0;
}
static inline int gpu_dvfs_get_cur_clock(void)
{
	return 0;
}
static inline int gpu_dvfs_get_utilization(void)
{
	return 0;
}
static inline int gpu_dvfs_get_max_freq(void)
{
	return 0;
}
static inline int gpu_dvfs_get_sustainable_info_array(int index)
{
	return 0;
}
static inline int gpu_dvfs_get_max_lock(void)
{
	return 0;
}
static inline bool gpu_dvfs_get_need_cpu_qos(void)
{
	return false;
}
#endif

#endif 
