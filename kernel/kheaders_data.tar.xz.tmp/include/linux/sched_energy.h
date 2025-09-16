#ifndef _LINUX_SCHED_ENERGY_H
#define _LINUX_SCHED_ENERGY_H

#include <linux/sched.h>
#include <linux/slab.h>


#define NR_SD_LEVELS 8

#define SD_LEVEL0   0
#define SD_LEVEL1   1
#define SD_LEVEL2   2
#define SD_LEVEL3   3
#define SD_LEVEL4   4
#define SD_LEVEL5   5
#define SD_LEVEL6   6
#define SD_LEVEL7   7


#define for_each_possible_sd_level(level)		    \
	for (level = 0; level < NR_SD_LEVELS; level++)

#ifdef CONFIG_SMP

extern struct sched_group_energy *sge_array[NR_CPUS][NR_SD_LEVELS];

void init_sched_energy_costs(void);

#else

#define init_sched_energy_costs() do { } while (0)

#endif 

#endif
