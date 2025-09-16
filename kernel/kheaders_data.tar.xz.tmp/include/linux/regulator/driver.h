

#ifndef __LINUX_REGULATOR_DRIVER_H_
#define __LINUX_REGULATOR_DRIVER_H_

#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/regulator/consumer.h>

struct regmap;
struct regulator_dev;
struct regulator_config;
struct regulator_init_data;
struct regulator_enable_gpio;

enum regulator_status {
	REGULATOR_STATUS_OFF,
	REGULATOR_STATUS_ON,
	REGULATOR_STATUS_ERROR,
	
	REGULATOR_STATUS_FAST,
	REGULATOR_STATUS_NORMAL,
	REGULATOR_STATUS_IDLE,
	REGULATOR_STATUS_STANDBY,
	
	REGULATOR_STATUS_BYPASS,
	
	REGULATOR_STATUS_UNDEFINED,
};


struct regulator_linear_range {
	unsigned int min_uV;
	unsigned int min_sel;
	unsigned int max_sel;
	unsigned int uV_step;
};


#define REGULATOR_LINEAR_RANGE(_min_uV, _min_sel, _max_sel, _step_uV)	\
{									\
	.min_uV		= _min_uV,					\
	.min_sel	= _min_sel,					\
	.max_sel	= _max_sel,					\
	.uV_step	= _step_uV,					\
}


struct regulator_ops {

	
	int (*list_voltage) (struct regulator_dev *, unsigned selector);

	
	int (*set_voltage) (struct regulator_dev *, int min_uV, int max_uV,
			    unsigned *selector);
	int (*map_voltage)(struct regulator_dev *, int min_uV, int max_uV);
	int (*set_voltage_sel) (struct regulator_dev *, unsigned selector);
	int (*get_voltage) (struct regulator_dev *);
	int (*get_voltage_sel) (struct regulator_dev *);

	
	int (*set_current_limit) (struct regulator_dev *,
				 int min_uA, int max_uA);
	int (*get_current_limit) (struct regulator_dev *);
#ifdef CONFIG_SEC_PM
	int (*set_short_detection)(struct regulator_dev *,
				   bool enable, int lv_uA);
#endif
	int (*set_input_current_limit) (struct regulator_dev *, int lim_uA);
	int (*set_over_current_protection) (struct regulator_dev *);
	int (*set_active_discharge) (struct regulator_dev *, bool enable);

	
	int (*enable) (struct regulator_dev *);
	int (*disable) (struct regulator_dev *);
	int (*is_enabled) (struct regulator_dev *);

	
	int (*set_mode) (struct regulator_dev *, unsigned int mode);
	unsigned int (*get_mode) (struct regulator_dev *);

	
	int (*enable_time) (struct regulator_dev *);
	int (*set_ramp_delay) (struct regulator_dev *, int ramp_delay);
	int (*set_voltage_time) (struct regulator_dev *, int old_uV,
				 int new_uV);
	int (*set_voltage_time_sel) (struct regulator_dev *,
				     unsigned int old_selector,
				     unsigned int new_selector);

	int (*set_soft_start) (struct regulator_dev *);

	
	int (*get_status)(struct regulator_dev *);

	
	unsigned int (*get_optimum_mode) (struct regulator_dev *, int input_uV,
					  int output_uV, int load_uA);
	
	int (*set_load)(struct regulator_dev *, int load_uA);

	
	int (*set_bypass)(struct regulator_dev *dev, bool enable);
	int (*get_bypass)(struct regulator_dev *dev, bool *enable);

	

	
	int (*set_suspend_voltage) (struct regulator_dev *, int uV);

	
	int (*set_suspend_enable) (struct regulator_dev *);
	int (*set_suspend_disable) (struct regulator_dev *);

	
	int (*set_suspend_mode) (struct regulator_dev *, unsigned int mode);

	int (*set_pull_down) (struct regulator_dev *);
};


enum regulator_type {
	REGULATOR_VOLTAGE,
	REGULATOR_CURRENT,
};


struct regulator_desc {
	const char *name;
	const char *supply_name;
	const char *of_match;
	const char *regulators_node;
	int (*of_parse_cb)(struct device_node *,
			    const struct regulator_desc *,
			    struct regulator_config *);
	int id;
	unsigned int continuous_voltage_range:1;
	unsigned n_voltages;
	const struct regulator_ops *ops;
	int irq;
	enum regulator_type type;
	struct module *owner;

	unsigned int min_uV;
	unsigned int uV_step;
	unsigned int linear_min_sel;
	int fixed_uV;
	unsigned int ramp_delay;
	int min_dropout_uV;

	const struct regulator_linear_range *linear_ranges;
	int n_linear_ranges;

	const unsigned int *volt_table;

	unsigned int vsel_reg;
	unsigned int vsel_mask;
	unsigned int csel_reg;
	unsigned int csel_mask;
	unsigned int apply_reg;
	unsigned int apply_bit;
	unsigned int enable_reg;
	unsigned int enable_mask;
	unsigned int enable_val;
	unsigned int disable_val;
	bool enable_is_inverted;
	unsigned int bypass_reg;
	unsigned int bypass_mask;
	unsigned int bypass_val_on;
	unsigned int bypass_val_off;
	unsigned int active_discharge_on;
	unsigned int active_discharge_off;
	unsigned int active_discharge_mask;
	unsigned int active_discharge_reg;

	unsigned int enable_time;

	unsigned int off_on_delay;

	unsigned int (*of_map_mode)(unsigned int mode);
};


struct regulator_config {
	struct device *dev;
	const struct regulator_init_data *init_data;
	void *driver_data;
	struct device_node *of_node;
	struct regmap *regmap;

	bool ena_gpio_initialized;
	int ena_gpio;
	unsigned int ena_gpio_invert:1;
	unsigned int ena_gpio_flags;
};


struct regulator_dev {
	const struct regulator_desc *desc;
	int exclusive;
	u32 use_count;
	u32 open_count;
	u32 bypass_count;

	
	struct list_head list; 

	
	struct list_head consumer_list; 

	struct blocking_notifier_head notifier;
	struct mutex mutex; 
	struct module *owner;
	struct device dev;
	struct regulation_constraints *constraints;
	struct regulator *supply;	
	const char *supply_name;
	struct regmap *regmap;

	struct delayed_work disable_work;
	int deferred_disables;

	void *reg_data;		

	struct dentry *debugfs;

	struct regulator_enable_gpio *ena_pin;
	unsigned int ena_gpio_state:1;

	unsigned int is_switch:1;

	
	unsigned long last_off_jiffy;
};

struct regulator_dev *
regulator_register(const struct regulator_desc *regulator_desc,
		   const struct regulator_config *config);
struct regulator_dev *
devm_regulator_register(struct device *dev,
			const struct regulator_desc *regulator_desc,
			const struct regulator_config *config);
void regulator_unregister(struct regulator_dev *rdev);
void devm_regulator_unregister(struct device *dev, struct regulator_dev *rdev);

int regulator_notifier_call_chain(struct regulator_dev *rdev,
				  unsigned long event, void *data);

void *rdev_get_drvdata(struct regulator_dev *rdev);
struct device *rdev_get_dev(struct regulator_dev *rdev);
int rdev_get_id(struct regulator_dev *rdev);

int regulator_mode_to_status(unsigned int);

int regulator_list_voltage_linear(struct regulator_dev *rdev,
				  unsigned int selector);
int regulator_list_voltage_linear_range(struct regulator_dev *rdev,
					unsigned int selector);
int regulator_list_voltage_table(struct regulator_dev *rdev,
				  unsigned int selector);
int regulator_map_voltage_linear(struct regulator_dev *rdev,
				  int min_uV, int max_uV);
int regulator_map_voltage_linear_range(struct regulator_dev *rdev,
				       int min_uV, int max_uV);
int regulator_map_voltage_iterate(struct regulator_dev *rdev,
				  int min_uV, int max_uV);
int regulator_map_voltage_ascend(struct regulator_dev *rdev,
				  int min_uV, int max_uV);
int regulator_get_voltage_sel_regmap(struct regulator_dev *rdev);
int regulator_set_voltage_sel_regmap(struct regulator_dev *rdev, unsigned sel);
int regulator_is_enabled_regmap(struct regulator_dev *rdev);
int regulator_enable_regmap(struct regulator_dev *rdev);
int regulator_disable_regmap(struct regulator_dev *rdev);
int regulator_set_voltage_time_sel(struct regulator_dev *rdev,
				   unsigned int old_selector,
				   unsigned int new_selector);
int regulator_set_bypass_regmap(struct regulator_dev *rdev, bool enable);
int regulator_get_bypass_regmap(struct regulator_dev *rdev, bool *enable);

int regulator_set_active_discharge_regmap(struct regulator_dev *rdev,
					  bool enable);
void *regulator_get_init_drvdata(struct regulator_init_data *reg_init_data);

#endif
