

#ifndef __ISP_COOLING_H__
#define __ISP_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>
#include <linux/platform_device.h>

#define ISP_FPS_INVALID     ~1

#ifdef CONFIG_ISP_THERMAL

#define ISP_FPS_ENTRY_INVALID ~0
#define ISP_FPS_TABLE_END     ~1

struct isp_fps_table {
	unsigned int	flags;
	unsigned int	driver_data; 
	unsigned int	fps; 
};

static inline bool isp_fps_next_valid(struct isp_fps_table **pos)
{
	while ((*pos)->fps != ISP_FPS_TABLE_END)
		if ((*pos)->fps != ISP_FPS_ENTRY_INVALID)
			return true;
		else
			(*pos)++;
	return false;
}



#define isp_fps_for_each_entry(pos, table)	\
	for (pos = table; pos->fps != ISP_FPS_TABLE_END; pos++)



#define isp_fps_for_each_valid_entry(pos, table)	\
	for (pos = table; isp_fps_next_valid(&pos); pos++)



struct thermal_cooling_device *
isp_cooling_register(const struct cpumask *clip_gpus);


#ifdef CONFIG_THERMAL_OF
struct thermal_cooling_device *
of_isp_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus);
#else
static inline struct thermal_cooling_device *
of_isp_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus)
{
	return NULL;
}
#endif


void isp_cooling_unregister(struct thermal_cooling_device *cdev);

unsigned long isp_cooling_get_level(unsigned int isp, unsigned int fps);
unsigned long isp_cooling_get_fps(unsigned int isp, unsigned long level);
#else 
static inline struct thermal_cooling_device *
isp_cooling_register(const struct cpumask *clip_gpus)
{
	return NULL;
}
static inline struct thermal_cooling_device *
of_isp_cooling_register(struct device_node *np,
			    const struct cpumask *clip_gpus)
{
	return NULL;
}
static inline
void isp_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return;
}
static inline
unsigned long isp_cooling_get_level(unsigned int isp, unsigned int fps)
{
	return THERMAL_CSTATE_INVALID;
}
static inline
unsigned long isp_cooling_get_fps(unsigned int isp, unsigned long level)
{
	return ISP_FPS_INVALID;
}
#endif	

#endif 
