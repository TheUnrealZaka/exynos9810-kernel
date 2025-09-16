

#ifndef __MVEBU_PMSU_H__
#define __MVEBU_PMSU_H__

#ifdef CONFIG_MACH_MVEBU_V7
int mvebu_pmsu_dfs_request(int cpu);
#else
static inline int mvebu_pmsu_dfs_request(int cpu) { return -ENODEV; }
#endif

#endif 
