

#ifndef __EXYNOS_MCINFO_H
#define __EXYNOS_MCINFO_H

#if defined(CONFIG_EXYNOS_MCINFO)
unsigned int get_mcinfo_base_count(void);
void get_refresh_rate(unsigned int *result);
#else
static inline
unsigned int get_mcinfo_base_count(void)
{
	return 0;
}
static inline
void get_refresh_rate(unsigned int *result)
{
	return;
}
#endif

#endif 
