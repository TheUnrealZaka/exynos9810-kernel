

#ifndef __LINUX_CLK_RENESAS_H_
#define __LINUX_CLK_RENESAS_H_

#include <linux/types.h>

struct device;
struct device_node;
struct generic_pm_domain;

void r8a7778_clocks_init(u32 mode);
void r8a7779_clocks_init(u32 mode);
void rcar_gen2_clocks_init(u32 mode);

void cpg_mstp_add_clk_domain(struct device_node *np);
#ifdef CONFIG_CLK_RENESAS_CPG_MSTP
int cpg_mstp_attach_dev(struct generic_pm_domain *unused, struct device *dev);
void cpg_mstp_detach_dev(struct generic_pm_domain *unused, struct device *dev);
#else
#define cpg_mstp_attach_dev	NULL
#define cpg_mstp_detach_dev	NULL
#endif

#ifdef CONFIG_CLK_RENESAS_CPG_MSSR
int cpg_mssr_attach_dev(struct generic_pm_domain *unused, struct device *dev);
void cpg_mssr_detach_dev(struct generic_pm_domain *unused, struct device *dev);
#else
#define cpg_mssr_attach_dev	NULL
#define cpg_mssr_detach_dev	NULL
#endif
#endif
