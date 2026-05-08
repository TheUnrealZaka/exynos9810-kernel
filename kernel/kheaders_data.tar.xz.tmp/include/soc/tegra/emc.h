

#ifndef __SOC_TEGRA_EMC_H__
#define __SOC_TEGRA_EMC_H__

struct tegra_emc;

int tegra_emc_prepare_timing_change(struct tegra_emc *emc,
				    unsigned long rate);
void tegra_emc_complete_timing_change(struct tegra_emc *emc,
				      unsigned long rate);

#endif 
