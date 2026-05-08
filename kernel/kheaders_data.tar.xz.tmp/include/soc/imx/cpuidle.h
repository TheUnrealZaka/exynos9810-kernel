

#ifndef __SOC_IMX_CPUIDLE_H__
#define __SOC_IMX_CPUIDLE_H__

#if defined(CONFIG_CPU_IDLE) && defined(CONFIG_SOC_IMX6Q)
void imx6q_cpuidle_fec_irqs_used(void);
void imx6q_cpuidle_fec_irqs_unused(void);
#else
static inline void imx6q_cpuidle_fec_irqs_used(void) { }
static inline void imx6q_cpuidle_fec_irqs_unused(void) { }
#endif

#endif 
