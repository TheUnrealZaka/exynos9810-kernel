

#ifndef __CPUIDLE_EXYNOS_H
#define __CPUIDLE_EXYNOS_H

struct cpuidle_exynos_data {
	int (*cpu0_enter_aftr)(void);
	int (*cpu1_powerdown)(void);
	void (*pre_enter_aftr)(void);
	void (*post_enter_aftr)(void);
};

#endif
