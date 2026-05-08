

#ifndef __DPADO_H
#define __DPADO_H

#ifdef CONFIG_SOC_EXYNOS9810
void dp_ado_switch_set_state(int state);
#else
#define dp_ado_set_state(state) do { } while (0)
#endif

#endif 
