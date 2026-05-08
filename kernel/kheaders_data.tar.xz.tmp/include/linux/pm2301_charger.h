

#ifndef __LINUX_PM2301_H
#define __LINUX_PM2301_H


struct pm2xxx_bm_charger_parameters {
	int ac_volt_max;
	int ac_curr_max;
};


struct pm2xxx_bm_data {
	bool enable_overshoot;
	const struct pm2xxx_bm_charger_parameters *chg_params;
};

struct pm2xxx_charger_platform_data {
	char **supplied_to;
	size_t num_supplicants;
	int i2c_bus;
	const char *label;
	int gpio_irq_number;
	unsigned int lpn_gpio;
	int irq_type;
};

struct pm2xxx_platform_data {
	struct pm2xxx_charger_platform_data *wall_charger;
	struct pm2xxx_bm_data *battery;
};

#endif 
