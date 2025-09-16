

#ifndef __INTEL_SOC_PMIC_H__
#define __INTEL_SOC_PMIC_H__

#include <linux/regmap.h>

struct intel_soc_pmic {
	int irq;
	struct regmap *regmap;
	struct regmap_irq_chip_data *irq_chip_data;
	struct regmap_irq_chip_data *irq_chip_data_level2;
	struct device *dev;
};

#endif	
