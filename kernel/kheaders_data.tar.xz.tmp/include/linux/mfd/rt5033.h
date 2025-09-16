

#ifndef __RT5033_H__
#define __RT5033_H__

#include <linux/regulator/consumer.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/power_supply.h>


enum rt5033_regulators {
	RT5033_BUCK = 0,
	RT5033_LDO,
	RT5033_SAFE_LDO,

	RT5033_REGULATOR_NUM,
};

struct rt5033_dev {
	struct device *dev;

	struct regmap *regmap;
	struct regmap_irq_chip_data *irq_data;
	int irq;
	bool wakeup;
};

struct rt5033_battery {
	struct i2c_client	*client;
	struct rt5033_dev	*rt5033;
	struct regmap		*regmap;
	struct power_supply	*psy;
};


struct rt5033_charger_data {
	unsigned int pre_uamp;
	unsigned int pre_uvolt;
	unsigned int const_uvolt;
	unsigned int eoc_uamp;
	unsigned int fast_uamp;
};

struct rt5033_charger {
	struct device		*dev;
	struct rt5033_dev	*rt5033;
	struct power_supply	psy;

	struct rt5033_charger_data	*chg;
};

#endif 
