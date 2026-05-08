

#ifndef __EXYNOS_EL3_MON_H
#define __EXYNOS_EL3_MON_H


#define EXYNOS_ERROR_TZASC_WRONG_REGION		(-1)

#define EXYNOS_ERROR_ALREADY_INITIALIZED	(1)
#define EXYNOS_ERROR_NOT_VALID_ADDRESS		(0x1000)

int exynos_pd_tz_save(unsigned int addr);
int exynos_pd_tz_restore(unsigned int addr);

#endif	
