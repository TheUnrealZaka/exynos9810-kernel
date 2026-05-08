

#ifndef __EXYNOS_PMU_H
#define __EXYNOS_PMU_H __FILE__


struct exynos_cpu_power_ops {
        void (*power_up)(unsigned int cpu);
        void (*power_down)(unsigned int cpu);
        int (*power_state)(unsigned int cpu);
        void (*cluster_up)(unsigned int cpu);
        void (*cluster_down)(unsigned int cpu);
        int (*cluster_state)(unsigned int cpu);
};
extern struct exynos_cpu_power_ops exynos_cpu;


#ifdef CONFIG_EXYNOS_PMU
extern int exynos_pmu_read(unsigned int offset, unsigned int *val);
extern int exynos_pmu_write(unsigned int offset, unsigned int val);
extern int exynos_pmu_update(unsigned int offset, unsigned int mask, unsigned int val);

extern void exynos_cpu_reset_enable(unsigned int cpu);
extern void exynos_cpu_reset_disable(unsigned int cpu);
extern int exynos_check_cp_status(void);
#else
static inline int exynos_pmu_read(unsigned int offset, unsigned int *val) {return 0;}
static inline int exynos_pmu_write(unsigned int offset, unsigned int val) {return 0;}
static inline int exynos_pmu_update(unsigned int offset, unsigned int mask, unsigned int val) {return 0;}

static inline void exynos_cpu_reset_enable(unsigned int cpu) {return ;}
static inline void exynos_cpu_reset_disable(unsigned int cpu) {return ;}
static inline int exynos_check_cp_status(void) {return 0;}
#endif
#endif 
