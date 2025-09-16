

#ifndef MADERA_PDATA_H
#define MADERA_PDATA_H

#include <linux/kernel.h>
#include <linux/irqchip/irq-madera-pdata.h>
#include <linux/regulator/arizona-ldo1.h>
#include <linux/regulator/arizona-micsupp.h>
#include <linux/regulator/machine.h>
#include <sound/madera-pdata.h>
#include <linux/extcon/extcon-madera-pdata.h>

#define MADERA_MAX_MICBIAS		4
#define MADERA_MAX_CHILD_MICBIAS	4

#define MADERA_MAX_GPSW			2

struct pinctrl_map;
struct regulator_init_data;


struct madera_micbias_pin_pdata {
	struct regulator_init_data *init_data;
	u32 active_discharge;
};


struct madera_micbias_pdata {
	struct regulator_init_data *init_data;
	u32 active_discharge;
	bool ext_cap;

	struct madera_micbias_pin_pdata pin[MADERA_MAX_CHILD_MICBIAS];
};


struct madera_pdata {
	int reset;

	struct arizona_ldo1_pdata ldo1;
	struct arizona_micsupp_pdata micvdd;

	struct madera_irqchip_pdata irqchip;

	int gpio_base;

	const struct pinctrl_map *gpio_configs;
	int n_gpio_configs;

	
	struct madera_micbias_pdata micbias[MADERA_MAX_MICBIAS];

	struct madera_codec_pdata codec;

	u32 gpsw[MADERA_MAX_GPSW];

	
	struct madera_accdet_pdata accdet[MADERA_MAX_ACCESSORY];
};

#endif

