

#ifndef __REGULATOR_GPIO_H
#define __REGULATOR_GPIO_H

struct regulator_init_data;

enum regulator_type;


struct gpio_regulator_state {
	int value;
	int gpios;
};


struct gpio_regulator_config {
	const char *supply_name;

	int enable_gpio;
	unsigned enable_high:1;
	unsigned enabled_at_boot:1;
	unsigned startup_delay;

	struct gpio *gpios;
	int nr_gpios;

	struct gpio_regulator_state *states;
	int nr_states;

	enum regulator_type type;
	struct regulator_init_data *init_data;
};

#endif
