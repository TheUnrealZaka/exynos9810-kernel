

#ifndef _CHARGER_MANAGER_H
#define _CHARGER_MANAGER_H

#include <linux/power_supply.h>
#include <linux/extcon.h>
#include <linux/alarmtimer.h>

enum data_source {
	CM_BATTERY_PRESENT,
	CM_NO_BATTERY,
	CM_FUEL_GAUGE,
	CM_CHARGER_STAT,
};

enum polling_modes {
	CM_POLL_DISABLE = 0,
	CM_POLL_ALWAYS,
	CM_POLL_EXTERNAL_POWER_ONLY,
	CM_POLL_CHARGING_ONLY,
};

enum cm_event_types {
	CM_EVENT_UNKNOWN = 0,
	CM_EVENT_BATT_FULL,
	CM_EVENT_BATT_IN,
	CM_EVENT_BATT_OUT,
	CM_EVENT_BATT_OVERHEAT,
	CM_EVENT_BATT_COLD,
	CM_EVENT_EXT_PWR_IN_OUT,
	CM_EVENT_CHG_START_STOP,
	CM_EVENT_OTHERS,
};


struct charger_cable {
	const char *extcon_name;
	const char *name;

	
	struct extcon_specific_cable_nb extcon_dev;
	struct work_struct wq;
	struct notifier_block nb;

	
	bool attached;

	struct charger_regulator *charger;

	
	int min_uA;
	int max_uA;

	struct charger_manager *cm;
};


struct charger_regulator {
	
	const char *regulator_name;
	struct regulator *consumer;

	
	int externally_control;

	
	struct charger_cable *cables;
	int num_cables;

	struct attribute_group attr_g;
	struct device_attribute attr_name;
	struct device_attribute attr_state;
	struct device_attribute attr_externally_control;
	struct attribute *attrs[4];

	struct charger_manager *cm;
};


struct charger_desc {
	const char *psy_name;

	enum polling_modes polling_mode;
	unsigned int polling_interval_ms;

	unsigned int fullbatt_vchkdrop_ms;
	unsigned int fullbatt_vchkdrop_uV;
	unsigned int fullbatt_uV;
	unsigned int fullbatt_soc;
	unsigned int fullbatt_full_capacity;

	enum data_source battery_present;

	const char **psy_charger_stat;

	int num_charger_regulators;
	struct charger_regulator *charger_regulators;

	const char *psy_fuel_gauge;

	const char *thermal_zone;

	int temp_min;
	int temp_max;
	int temp_diff;

	bool measure_battery_temp;

	u32 charging_max_duration_ms;
	u32 discharging_max_duration_ms;
};

#define PSY_NAME_MAX	30


struct charger_manager {
	struct list_head entry;
	struct device *dev;
	struct charger_desc *desc;

#ifdef CONFIG_THERMAL
	struct thermal_zone_device *tzd_batt;
#endif
	bool charger_enabled;

	unsigned long fullbatt_vchk_jiffies_at;
	struct delayed_work fullbatt_vchk_work;

	int emergency_stop;

	char psy_name_buf[PSY_NAME_MAX + 1];
	struct power_supply_desc charger_psy_desc;
	struct power_supply *charger_psy;

	u64 charging_start_time;
	u64 charging_end_time;
};

#ifdef CONFIG_CHARGER_MANAGER
extern void cm_notify_event(struct power_supply *psy,
				enum cm_event_types type, char *msg);
#else
static inline void cm_notify_event(struct power_supply *psy,
				enum cm_event_types type, char *msg) { }
#endif
#endif 
