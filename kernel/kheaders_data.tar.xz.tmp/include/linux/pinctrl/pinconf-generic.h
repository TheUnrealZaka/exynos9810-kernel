
#ifndef __LINUX_PINCTRL_PINCONF_GENERIC_H
#define __LINUX_PINCTRL_PINCONF_GENERIC_H


#ifdef CONFIG_GENERIC_PINCONF


enum pin_config_param {
	PIN_CONFIG_BIAS_BUS_HOLD,
	PIN_CONFIG_BIAS_DISABLE,
	PIN_CONFIG_BIAS_HIGH_IMPEDANCE,
	PIN_CONFIG_BIAS_PULL_DOWN,
	PIN_CONFIG_BIAS_PULL_PIN_DEFAULT,
	PIN_CONFIG_BIAS_PULL_UP,
	PIN_CONFIG_DRIVE_OPEN_DRAIN,
	PIN_CONFIG_DRIVE_OPEN_SOURCE,
	PIN_CONFIG_DRIVE_PUSH_PULL,
	PIN_CONFIG_DRIVE_STRENGTH,
	PIN_CONFIG_INPUT_DEBOUNCE,
	PIN_CONFIG_INPUT_ENABLE,
	PIN_CONFIG_INPUT_SCHMITT,
	PIN_CONFIG_INPUT_SCHMITT_ENABLE,
	PIN_CONFIG_LOW_POWER_MODE,
	PIN_CONFIG_OUTPUT,
	PIN_CONFIG_POWER_SOURCE,
	PIN_CONFIG_SLEW_RATE,
	PIN_CONFIG_END = 0x7FFF,
};

#ifdef CONFIG_DEBUG_FS
#define PCONFDUMP(a, b, c, d) { .param = a, .display = b, .format = c, \
				.has_arg = d }

struct pin_config_item {
	const enum pin_config_param param;
	const char * const display;
	const char * const format;
	bool has_arg;
};
#endif 


#define PIN_CONF_PACKED(p, a) ((a << 16) | ((unsigned long) p & 0xffffUL))



static inline enum pin_config_param pinconf_to_config_param(unsigned long config)
{
	return (enum pin_config_param) (config & 0xffffUL);
}

static inline u16 pinconf_to_config_argument(unsigned long config)
{
	return (enum pin_config_param) ((config >> 16) & 0xffffUL);
}

static inline unsigned long pinconf_to_config_packed(enum pin_config_param param,
						     u16 argument)
{
	return PIN_CONF_PACKED(param, argument);
}

#ifdef CONFIG_OF

#include <linux/device.h>
#include <linux/pinctrl/machine.h>
struct pinctrl_dev;
struct pinctrl_map;

struct pinconf_generic_params {
	const char * const property;
	enum pin_config_param param;
	u32 default_value;
};

int pinconf_generic_dt_subnode_to_map(struct pinctrl_dev *pctldev,
		struct device_node *np, struct pinctrl_map **map,
		unsigned *reserved_maps, unsigned *num_maps,
		enum pinctrl_map_type type);
int pinconf_generic_dt_node_to_map(struct pinctrl_dev *pctldev,
		struct device_node *np_config, struct pinctrl_map **map,
		unsigned *num_maps, enum pinctrl_map_type type);
void pinconf_generic_dt_free_map(struct pinctrl_dev *pctldev,
		struct pinctrl_map *map, unsigned num_maps);

static inline int pinconf_generic_dt_node_to_map_group(
		struct pinctrl_dev *pctldev, struct device_node *np_config,
		struct pinctrl_map **map, unsigned *num_maps)
{
	return pinconf_generic_dt_node_to_map(pctldev, np_config, map, num_maps,
			PIN_MAP_TYPE_CONFIGS_GROUP);
}

static inline int pinconf_generic_dt_node_to_map_pin(
		struct pinctrl_dev *pctldev, struct device_node *np_config,
		struct pinctrl_map **map, unsigned *num_maps)
{
	return pinconf_generic_dt_node_to_map(pctldev, np_config, map, num_maps,
			PIN_MAP_TYPE_CONFIGS_PIN);
}

static inline int pinconf_generic_dt_node_to_map_all(
		struct pinctrl_dev *pctldev, struct device_node *np_config,
		struct pinctrl_map **map, unsigned *num_maps)
{
	
	return pinconf_generic_dt_node_to_map(pctldev, np_config, map, num_maps,
			PIN_MAP_TYPE_INVALID);
}
#endif

#endif 

#endif 
