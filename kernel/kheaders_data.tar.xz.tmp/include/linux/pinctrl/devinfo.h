

#ifndef PINCTRL_DEVINFO_H
#define PINCTRL_DEVINFO_H

#ifdef CONFIG_PINCTRL


#include <linux/pinctrl/consumer.h>


struct dev_pin_info {
	struct pinctrl *p;
	struct pinctrl_state *default_state;
	struct pinctrl_state *init_state;
#ifdef CONFIG_PM
	struct pinctrl_state *sleep_state;
	struct pinctrl_state *idle_state;
#endif
};

extern int pinctrl_bind_pins(struct device *dev);
extern int pinctrl_init_done(struct device *dev);

#else



static inline int pinctrl_bind_pins(struct device *dev)
{
	return 0;
}

static inline int pinctrl_init_done(struct device *dev)
{
	return 0;
}

#endif 
#endif 
