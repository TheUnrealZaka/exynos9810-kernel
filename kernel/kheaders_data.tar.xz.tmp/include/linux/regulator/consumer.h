

#ifndef __LINUX_REGULATOR_CONSUMER_H_
#define __LINUX_REGULATOR_CONSUMER_H_

#include <linux/err.h>

struct device;
struct notifier_block;
struct regmap;



#define REGULATOR_MODE_FAST			0x1
#define REGULATOR_MODE_NORMAL			0x2
#define REGULATOR_MODE_IDLE			0x4
#define REGULATOR_MODE_STANDBY			0x8



#define REGULATOR_EVENT_UNDER_VOLTAGE		0x01
#define REGULATOR_EVENT_OVER_CURRENT		0x02
#define REGULATOR_EVENT_REGULATION_OUT		0x04
#define REGULATOR_EVENT_FAIL			0x08
#define REGULATOR_EVENT_OVER_TEMP		0x10
#define REGULATOR_EVENT_FORCE_DISABLE		0x20
#define REGULATOR_EVENT_VOLTAGE_CHANGE		0x40
#define REGULATOR_EVENT_DISABLE			0x80
#define REGULATOR_EVENT_PRE_VOLTAGE_CHANGE	0x100
#define REGULATOR_EVENT_ABORT_VOLTAGE_CHANGE	0x200
#define REGULATOR_EVENT_PRE_DISABLE		0x400
#define REGULATOR_EVENT_ABORT_DISABLE		0x800


struct pre_voltage_change_data {
	unsigned long old_uV;
	unsigned long min_uV;
	unsigned long max_uV;
};

struct regulator;


struct regulator_bulk_data {
	const char *supply;
	struct regulator *consumer;

	
	int ret;
};

#if defined(CONFIG_REGULATOR)


struct regulator *__must_check regulator_get(struct device *dev,
					     const char *id);
struct regulator *__must_check devm_regulator_get(struct device *dev,
					     const char *id);
struct regulator *__must_check regulator_get_exclusive(struct device *dev,
						       const char *id);
struct regulator *__must_check devm_regulator_get_exclusive(struct device *dev,
							const char *id);
struct regulator *__must_check regulator_get_optional(struct device *dev,
						      const char *id);
struct regulator *__must_check devm_regulator_get_optional(struct device *dev,
							   const char *id);
void regulator_put(struct regulator *regulator);
void devm_regulator_put(struct regulator *regulator);

int regulator_register_supply_alias(struct device *dev, const char *id,
				    struct device *alias_dev,
				    const char *alias_id);
void regulator_unregister_supply_alias(struct device *dev, const char *id);

int regulator_bulk_register_supply_alias(struct device *dev,
					 const char *const *id,
					 struct device *alias_dev,
					 const char *const *alias_id,
					 int num_id);
void regulator_bulk_unregister_supply_alias(struct device *dev,
					    const char * const *id, int num_id);

int devm_regulator_register_supply_alias(struct device *dev, const char *id,
					 struct device *alias_dev,
					 const char *alias_id);
void devm_regulator_unregister_supply_alias(struct device *dev,
					    const char *id);

int devm_regulator_bulk_register_supply_alias(struct device *dev,
					      const char *const *id,
					      struct device *alias_dev,
					      const char *const *alias_id,
					      int num_id);
void devm_regulator_bulk_unregister_supply_alias(struct device *dev,
						 const char *const *id,
						 int num_id);


int __must_check regulator_enable(struct regulator *regulator);
int regulator_disable(struct regulator *regulator);
int regulator_force_disable(struct regulator *regulator);
int regulator_is_enabled(struct regulator *regulator);
int regulator_disable_deferred(struct regulator *regulator, int ms);

int __must_check regulator_bulk_get(struct device *dev, int num_consumers,
				    struct regulator_bulk_data *consumers);
int __must_check devm_regulator_bulk_get(struct device *dev, int num_consumers,
					 struct regulator_bulk_data *consumers);
int __must_check regulator_bulk_enable(int num_consumers,
				       struct regulator_bulk_data *consumers);
int regulator_bulk_disable(int num_consumers,
			   struct regulator_bulk_data *consumers);
int regulator_bulk_force_disable(int num_consumers,
			   struct regulator_bulk_data *consumers);
void regulator_bulk_free(int num_consumers,
			 struct regulator_bulk_data *consumers);

int regulator_count_voltages(struct regulator *regulator);
int regulator_list_voltage(struct regulator *regulator, unsigned selector);
int regulator_is_supported_voltage(struct regulator *regulator,
				   int min_uV, int max_uV);
unsigned int regulator_get_linear_step(struct regulator *regulator);
int regulator_set_voltage(struct regulator *regulator, int min_uV, int max_uV);
int regulator_set_voltage_time(struct regulator *regulator,
			       int old_uV, int new_uV);
int regulator_get_voltage(struct regulator *regulator);
int regulator_sync_voltage(struct regulator *regulator);
#ifdef CONFIG_SEC_PM
int regulator_set_short_detection(struct regulator *regulator,
				  bool enable, int lv_uA);
#endif
int regulator_set_current_limit(struct regulator *regulator,
			       int min_uA, int max_uA);
int regulator_get_current_limit(struct regulator *regulator);

int regulator_set_mode(struct regulator *regulator, unsigned int mode);
unsigned int regulator_get_mode(struct regulator *regulator);
int regulator_set_load(struct regulator *regulator, int load_uA);

int regulator_allow_bypass(struct regulator *regulator, bool allow);

struct regmap *regulator_get_regmap(struct regulator *regulator);
int regulator_get_hardware_vsel_register(struct regulator *regulator,
					 unsigned *vsel_reg,
					 unsigned *vsel_mask);
int regulator_list_hardware_vsel(struct regulator *regulator,
				 unsigned selector);


int regulator_register_notifier(struct regulator *regulator,
			      struct notifier_block *nb);
int devm_regulator_register_notifier(struct regulator *regulator,
				     struct notifier_block *nb);
int regulator_unregister_notifier(struct regulator *regulator,
				struct notifier_block *nb);
void devm_regulator_unregister_notifier(struct regulator *regulator,
					struct notifier_block *nb);


void *regulator_get_drvdata(struct regulator *regulator);
void regulator_set_drvdata(struct regulator *regulator, void *data);

#else


static inline struct regulator *__must_check regulator_get(struct device *dev,
	const char *id)
{
	
	return NULL;
}

static inline struct regulator *__must_check
devm_regulator_get(struct device *dev, const char *id)
{
	return NULL;
}

static inline struct regulator *__must_check
regulator_get_exclusive(struct device *dev, const char *id)
{
	return ERR_PTR(-ENODEV);
}

static inline struct regulator *__must_check
regulator_get_optional(struct device *dev, const char *id)
{
	return ERR_PTR(-ENODEV);
}


static inline struct regulator *__must_check
devm_regulator_get_optional(struct device *dev, const char *id)
{
	return ERR_PTR(-ENODEV);
}

static inline void regulator_put(struct regulator *regulator)
{
}

static inline void devm_regulator_put(struct regulator *regulator)
{
}

static inline int regulator_register_supply_alias(struct device *dev,
						  const char *id,
						  struct device *alias_dev,
						  const char *alias_id)
{
	return 0;
}

static inline void regulator_unregister_supply_alias(struct device *dev,
						    const char *id)
{
}

static inline int regulator_bulk_register_supply_alias(struct device *dev,
						const char *const *id,
						struct device *alias_dev,
						const char * const *alias_id,
						int num_id)
{
	return 0;
}

static inline void regulator_bulk_unregister_supply_alias(struct device *dev,
						const char * const *id,
						int num_id)
{
}

static inline int devm_regulator_register_supply_alias(struct device *dev,
						       const char *id,
						       struct device *alias_dev,
						       const char *alias_id)
{
	return 0;
}

static inline void devm_regulator_unregister_supply_alias(struct device *dev,
							  const char *id)
{
}

static inline int devm_regulator_bulk_register_supply_alias(struct device *dev,
						const char *const *id,
						struct device *alias_dev,
						const char *const *alias_id,
						int num_id)
{
	return 0;
}

static inline void devm_regulator_bulk_unregister_supply_alias(
	struct device *dev, const char *const *id, int num_id)
{
}

static inline int regulator_enable(struct regulator *regulator)
{
	return 0;
}

static inline int regulator_disable(struct regulator *regulator)
{
	return 0;
}

static inline int regulator_force_disable(struct regulator *regulator)
{
	return 0;
}

static inline int regulator_disable_deferred(struct regulator *regulator,
					     int ms)
{
	return 0;
}

static inline int regulator_is_enabled(struct regulator *regulator)
{
	return 1;
}

static inline int regulator_bulk_get(struct device *dev,
				     int num_consumers,
				     struct regulator_bulk_data *consumers)
{
	return 0;
}

static inline int devm_regulator_bulk_get(struct device *dev, int num_consumers,
					  struct regulator_bulk_data *consumers)
{
	return 0;
}

static inline int regulator_bulk_enable(int num_consumers,
					struct regulator_bulk_data *consumers)
{
	return 0;
}

static inline int regulator_bulk_disable(int num_consumers,
					 struct regulator_bulk_data *consumers)
{
	return 0;
}

static inline int regulator_bulk_force_disable(int num_consumers,
					struct regulator_bulk_data *consumers)
{
	return 0;
}

static inline void regulator_bulk_free(int num_consumers,
				       struct regulator_bulk_data *consumers)
{
}

static inline int regulator_set_voltage(struct regulator *regulator,
					int min_uV, int max_uV)
{
	return 0;
}

static inline int regulator_set_voltage_time(struct regulator *regulator,
					     int old_uV, int new_uV)
{
	return 0;
}

static inline int regulator_get_voltage(struct regulator *regulator)
{
	return -EINVAL;
}

static inline int regulator_is_supported_voltage(struct regulator *regulator,
				   int min_uV, int max_uV)
{
	return 0;
}

#ifdef CONFIG_SEC_PM
static inline int regulator_set_short_protection(struct regulator *regulator,
						 bool enable, int lv_uA)
{
	return 0;
}
#endif

static inline int regulator_set_current_limit(struct regulator *regulator,
					     int min_uA, int max_uA)
{
	return 0;
}

static inline int regulator_get_current_limit(struct regulator *regulator)
{
	return 0;
}

static inline int regulator_set_mode(struct regulator *regulator,
	unsigned int mode)
{
	return 0;
}

static inline unsigned int regulator_get_mode(struct regulator *regulator)
{
	return REGULATOR_MODE_NORMAL;
}

static inline int regulator_set_load(struct regulator *regulator, int load_uA)
{
	return 0;
}

static inline int regulator_allow_bypass(struct regulator *regulator,
					 bool allow)
{
	return 0;
}

static inline struct regmap *regulator_get_regmap(struct regulator *regulator)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int regulator_get_hardware_vsel_register(struct regulator *regulator,
						       unsigned *vsel_reg,
						       unsigned *vsel_mask)
{
	return -EOPNOTSUPP;
}

static inline int regulator_list_hardware_vsel(struct regulator *regulator,
					       unsigned selector)
{
	return -EOPNOTSUPP;
}

static inline int regulator_register_notifier(struct regulator *regulator,
			      struct notifier_block *nb)
{
	return 0;
}

static inline int devm_regulator_register_notifier(struct regulator *regulator,
						   struct notifier_block *nb)
{
	return 0;
}

static inline int regulator_unregister_notifier(struct regulator *regulator,
				struct notifier_block *nb)
{
	return 0;
}

static inline int devm_regulator_unregister_notifier(struct regulator *regulator,
						     struct notifier_block *nb)
{
	return 0;
}

static inline void *regulator_get_drvdata(struct regulator *regulator)
{
	return NULL;
}

static inline void regulator_set_drvdata(struct regulator *regulator,
	void *data)
{
}

static inline int regulator_count_voltages(struct regulator *regulator)
{
	return 0;
}

static inline int regulator_list_voltage(struct regulator *regulator, unsigned selector)
{
	return -EINVAL;
}

#endif

static inline int regulator_set_voltage_triplet(struct regulator *regulator,
						int min_uV, int target_uV,
						int max_uV)
{
	if (regulator_set_voltage(regulator, target_uV, max_uV) == 0)
		return 0;

	return regulator_set_voltage(regulator, min_uV, max_uV);
}

static inline int regulator_set_voltage_tol(struct regulator *regulator,
					    int new_uV, int tol_uV)
{
	if (regulator_set_voltage(regulator, new_uV, new_uV + tol_uV) == 0)
		return 0;
	else
		return regulator_set_voltage(regulator,
					     new_uV - tol_uV, new_uV + tol_uV);
}

static inline int regulator_is_supported_voltage_tol(struct regulator *regulator,
						     int target_uV, int tol_uV)
{
	return regulator_is_supported_voltage(regulator,
					      target_uV - tol_uV,
					      target_uV + tol_uV);
}

#endif
