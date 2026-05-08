

#ifndef __SOC_TEGRA_CPUIDLE_H__
#define __SOC_TEGRA_CPUIDLE_H__

#if defined(CONFIG_ARM) && defined(CONFIG_CPU_IDLE)
void tegra_cpuidle_pcie_irqs_in_use(void);
#else
static inline void tegra_cpuidle_pcie_irqs_in_use(void)
{
}
#endif

#endif 
