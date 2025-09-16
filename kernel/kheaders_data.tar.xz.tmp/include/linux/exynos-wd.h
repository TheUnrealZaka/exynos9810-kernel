

#ifdef CONFIG_EXYNOS_WD_DVFS
void exynos_wd_call_chain(void);
#else
#define exynos_wd_call_chain() do {} while(0)
#endif

