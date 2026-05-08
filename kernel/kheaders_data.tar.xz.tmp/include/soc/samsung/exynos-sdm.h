

#ifndef EXYNOS_SDM_H
#define EXYNOS_SDM_H

#ifdef CONFIG_EXYNOS_SDM
int exynos_sdm_dump_secure_region(void);
int exynos_sdm_flush_secdram(void);
#else
#define exynos_sdm_dump_secure_region()		do { } while(0)
#define exynos_sdm_flush_secdram()		do { } while(0)
#endif

#endif 
