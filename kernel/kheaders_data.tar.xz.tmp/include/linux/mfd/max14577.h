

#ifndef __MAX14577_H__
#define __MAX14577_H__

#include <linux/regulator/consumer.h>


enum max14577_regulators {
	MAX14577_SAFEOUT = 0,
	MAX14577_CHARGER,

	MAX14577_REGULATOR_NUM,
};


enum max77836_regulators {
	MAX77836_SAFEOUT = 0,
	MAX77836_CHARGER,
	MAX77836_LDO1,
	MAX77836_LDO2,

	MAX77836_REGULATOR_NUM,
};

struct max14577_regulator_platform_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *of_node;
};

struct max14577_charger_platform_data {
	u32 constant_uvolt;
	u32 fast_charge_uamp;
	u32 eoc_uamp;
	u32 ovp_uvolt;
};


struct max14577_platform_data {
	
	int irq_base;

	
	int gpio_pogo_vbatt_en;
	int gpio_pogo_vbus_en;

	
	int (*set_gpio_pogo_vbatt_en) (int gpio_val);
	int (*set_gpio_pogo_vbus_en) (int gpio_val);

	int (*set_gpio_pogo_cb) (int new_dev);

	struct max14577_regulator_platform_data *regulators;
};


struct maxim_charger_current {
	
	unsigned int min;
	
	unsigned int high_start;
	
	unsigned int high_step;
	
	unsigned int max;
};

extern const struct maxim_charger_current maxim_charger_currents[];
extern int maxim_charger_calc_reg_current(const struct maxim_charger_current *limits,
		unsigned int min_ua, unsigned int max_ua, u8 *dst);

#endif 
