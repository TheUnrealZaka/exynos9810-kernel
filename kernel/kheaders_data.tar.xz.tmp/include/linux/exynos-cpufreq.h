

#ifdef CONFIG_ARM_EXYNOS_ACME
extern unsigned int exynos_cpufreq_get_max_freq(struct cpumask *mask);
extern bool exynos_cpufreq_allow_change_max(unsigned int cpu, unsigned long max);
#else
static inline unsigned int exynos_cpufreq_get_max_freq(struct cpumask *mask)
{
	return 0;
}
static inline bool exynos_cpufreq_allow_change_max(unsigned int cpu, unsigned long max)
{
	return 0;
}
#endif
