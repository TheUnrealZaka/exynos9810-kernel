
#ifndef __LINUX_PINCTRL_PINCONF_H
#define __LINUX_PINCTRL_PINCONF_H

#ifdef CONFIG_PINCONF

#include <linux/pinctrl/machine.h>

struct pinctrl_dev;
struct seq_file;


struct pinconf_ops {
#ifdef CONFIG_GENERIC_PINCONF
	bool is_generic;
#endif
	int (*pin_config_get) (struct pinctrl_dev *pctldev,
			       unsigned pin,
			       unsigned long *config);
	int (*pin_config_set) (struct pinctrl_dev *pctldev,
			       unsigned pin,
			       unsigned long *configs,
			       unsigned num_configs);
	int (*pin_config_group_get) (struct pinctrl_dev *pctldev,
				     unsigned selector,
				     unsigned long *config);
	int (*pin_config_group_set) (struct pinctrl_dev *pctldev,
				     unsigned selector,
				     unsigned long *configs,
				     unsigned num_configs);
	int (*pin_config_dbg_parse_modify) (struct pinctrl_dev *pctldev,
					   const char *arg,
					   unsigned long *config);
	void (*pin_config_dbg_show) (struct pinctrl_dev *pctldev,
				     struct seq_file *s,
				     unsigned offset);
	void (*pin_config_group_dbg_show) (struct pinctrl_dev *pctldev,
					   struct seq_file *s,
					   unsigned selector);
	void (*pin_config_config_dbg_show) (struct pinctrl_dev *pctldev,
					    struct seq_file *s,
					    unsigned long config);
};

#endif

#endif 
