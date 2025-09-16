

#ifndef __LINUX_POWER_SUPPLY_H__
#define __LINUX_POWER_SUPPLY_H__

#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/leds.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/types.h>





enum {
	POWER_SUPPLY_STATUS_UNKNOWN = 0,
	POWER_SUPPLY_STATUS_CHARGING,
	POWER_SUPPLY_STATUS_DISCHARGING,
	POWER_SUPPLY_STATUS_NOT_CHARGING,
	POWER_SUPPLY_STATUS_FULL,
};

enum {
	POWER_SUPPLY_CHARGE_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_CHARGE_TYPE_NONE,
	POWER_SUPPLY_CHARGE_TYPE_TRICKLE,
	POWER_SUPPLY_CHARGE_TYPE_FAST,
	POWER_SUPPLY_CHARGE_TYPE_SLOW,
};

enum {
	POWER_SUPPLY_HEALTH_UNKNOWN = 0,
	POWER_SUPPLY_HEALTH_GOOD,
	POWER_SUPPLY_HEALTH_OVERHEAT,
	POWER_SUPPLY_HEALTH_WARM,
	POWER_SUPPLY_HEALTH_DEAD,
	POWER_SUPPLY_HEALTH_OVERVOLTAGE,
	POWER_SUPPLY_HEALTH_UNSPEC_FAILURE,
	POWER_SUPPLY_HEALTH_COLD,
	POWER_SUPPLY_HEALTH_COOL,
	POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE,
	POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE,
	POWER_SUPPLY_HEALTH_UNDERVOLTAGE,
	POWER_SUPPLY_HEALTH_OVERHEATLIMIT,
	POWER_SUPPLY_HEALTH_MAX,
};

enum {
	POWER_SUPPLY_TECHNOLOGY_UNKNOWN = 0,
	POWER_SUPPLY_TECHNOLOGY_NiMH,
	POWER_SUPPLY_TECHNOLOGY_LION,
	POWER_SUPPLY_TECHNOLOGY_LIPO,
	POWER_SUPPLY_TECHNOLOGY_LiFe,
	POWER_SUPPLY_TECHNOLOGY_NiCd,
	POWER_SUPPLY_TECHNOLOGY_LiMn,
};

enum {
	POWER_SUPPLY_CAPACITY_LEVEL_UNKNOWN = 0,
	POWER_SUPPLY_CAPACITY_LEVEL_CRITICAL,
	POWER_SUPPLY_CAPACITY_LEVEL_LOW,
	POWER_SUPPLY_CAPACITY_LEVEL_NORMAL,
	POWER_SUPPLY_CAPACITY_LEVEL_HIGH,
	POWER_SUPPLY_CAPACITY_LEVEL_FULL,
};

enum {
	POWER_SUPPLY_SCOPE_UNKNOWN = 0,
	POWER_SUPPLY_SCOPE_SYSTEM,
	POWER_SUPPLY_SCOPE_DEVICE,
};

enum {
	POWER_SUPPLY_DP_DM_UNKNOWN = 0,
	POWER_SUPPLY_DP_DM_PREPARE = 1,
	POWER_SUPPLY_DP_DM_UNPREPARE = 2,
	POWER_SUPPLY_DP_DM_CONFIRMED_HVDCP3 = 3,
	POWER_SUPPLY_DP_DM_DP_PULSE = 4,
	POWER_SUPPLY_DP_DM_DM_PULSE = 5,
	POWER_SUPPLY_DP_DM_DP0P6_DMF = 6,
	POWER_SUPPLY_DP_DM_DP0P6_DM3P3 = 7,
	POWER_SUPPLY_DP_DM_DPF_DMF = 8,
	POWER_SUPPLY_DP_DM_DPR_DMR = 9,
};

enum power_supply_property {
	
	POWER_SUPPLY_PROP_STATUS = 0,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_AUTHENTIC,
	POWER_SUPPLY_PROP_CHARGING_ENABLED,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_CYCLE_COUNT,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_MIN,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_AVG,
	POWER_SUPPLY_PROP_VOLTAGE_OCV,
	POWER_SUPPLY_PROP_VOLTAGE_BOOT,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_REGULATION,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CURRENT_AVG,
	POWER_SUPPLY_PROP_CURRENT_FULL,
	POWER_SUPPLY_PROP_CURRENT_BOOT,
	POWER_SUPPLY_PROP_POWER_DESIGN,
	POWER_SUPPLY_PROP_POWER_NOW,
	POWER_SUPPLY_PROP_POWER_AVG,
	POWER_SUPPLY_PROP_FILTER_CFG,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_EMPTY_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_FULL,
	POWER_SUPPLY_PROP_CHARGE_EMPTY,
	POWER_SUPPLY_PROP_CHARGE_NOW,
	POWER_SUPPLY_PROP_CHARGE_AVG,
	POWER_SUPPLY_PROP_CHARGE_COUNTER,
	POWER_SUPPLY_PROP_CHARGE_OTG_CONTROL,
	POWER_SUPPLY_PROP_CHARGE_POWERED_OTG_CONTROL,
	POWER_SUPPLY_PROP_CHARGE_UNO_CONTROL,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT,
	POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
	POWER_SUPPLY_PROP_CHARGE_COUNTER_SHADOW,
	POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
	POWER_SUPPLY_PROP_ENERGY_EMPTY_DESIGN,
	POWER_SUPPLY_PROP_ENERGY_FULL,
	POWER_SUPPLY_PROP_ENERGY_EMPTY,
	POWER_SUPPLY_PROP_ENERGY_NOW,
	POWER_SUPPLY_PROP_ENERGY_AVG,
	POWER_SUPPLY_PROP_CAPACITY, 
	POWER_SUPPLY_PROP_CAPACITY_ALERT_MIN, 
	POWER_SUPPLY_PROP_CAPACITY_ALERT_MAX, 
	POWER_SUPPLY_PROP_CAPACITY_LEVEL,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TEMP_MAX,
	POWER_SUPPLY_PROP_TEMP_MIN,
	POWER_SUPPLY_PROP_TEMP_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_ALERT_MAX,
	POWER_SUPPLY_PROP_COOL_TEMP,
	POWER_SUPPLY_PROP_WARM_TEMP,
	POWER_SUPPLY_PROP_TEMP_AMBIENT,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
	POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
	POWER_SUPPLY_PROP_TIME_TO_FULL_AVG,
	POWER_SUPPLY_PROP_TYPE, 
	POWER_SUPPLY_PROP_SCOPE,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_CALIBRATE,
	POWER_SUPPLY_PROP_SYSTEM_TEMP_LEVEL,
	POWER_SUPPLY_PROP_RESISTANCE,
	
	POWER_SUPPLY_PROP_USB_OTG,
	POWER_SUPPLY_PROP_CHARGE_ENABLED,
	
	POWER_SUPPLY_PROP_CHARGE_COUNTER_EXT,
	
	POWER_SUPPLY_PROP_MANUFACTURER,
	POWER_SUPPLY_PROP_SERIAL_NUMBER,
	POWER_SUPPLY_PROP_AFC_CHARGER_MODE,
	POWER_SUPPLY_PROP_USB_HC,
	POWER_SUPPLY_PROP_MODEL_NAME,

	POWER_SUPPLY_PROP_MAX,
	POWER_SUPPLY_EXT_PROP_MAX = POWER_SUPPLY_PROP_MAX + 256,
};

enum power_supply_type {
	POWER_SUPPLY_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_TYPE_BATTERY,			
	POWER_SUPPLY_TYPE_UPS,				
	POWER_SUPPLY_TYPE_MAINS,			
	POWER_SUPPLY_TYPE_USB,				
	POWER_SUPPLY_TYPE_USB_DCP,			
	POWER_SUPPLY_TYPE_USB_CDP,			
	POWER_SUPPLY_TYPE_USB_ACA,			
	POWER_SUPPLY_TYPE_BMS,				
	POWER_SUPPLY_TYPE_MISC,				
	POWER_SUPPLY_TYPE_WIRELESS,			
	POWER_SUPPLY_TYPE_HV_WIRELESS,			
	POWER_SUPPLY_TYPE_PMA_WIRELESS,			
	POWER_SUPPLY_TYPE_CARDOCK,			
	POWER_SUPPLY_TYPE_UARTOFF,			
	POWER_SUPPLY_TYPE_OTG,				
	POWER_SUPPLY_TYPE_LAN_HUB,			
	POWER_SUPPLY_TYPE_MHL_500,			
	POWER_SUPPLY_TYPE_MHL_900,			
	POWER_SUPPLY_TYPE_MHL_1500,			
	POWER_SUPPLY_TYPE_MHL_USB,			
	POWER_SUPPLY_TYPE_SMART_OTG,			
	POWER_SUPPLY_TYPE_SMART_NOTG,			
	POWER_SUPPLY_TYPE_POWER_SHARING,		
	POWER_SUPPLY_TYPE_HV_MAINS,			
	POWER_SUPPLY_TYPE_HV_MAINS_12V,			
	POWER_SUPPLY_TYPE_HV_PREPARE_MAINS,		
	POWER_SUPPLY_TYPE_HV_ERR,			
	POWER_SUPPLY_TYPE_MHL_USB_100,			
	POWER_SUPPLY_TYPE_MHL_2000,			
	POWER_SUPPLY_TYPE_HV_UNKNOWN,			
	POWER_SUPPLY_TYPE_MDOCK_TA,			
	POWER_SUPPLY_TYPE_HMT_CONNECTED,		
	POWER_SUPPLY_TYPE_HMT_CHARGE,			
	POWER_SUPPLY_TYPE_WIRELESS_PACK,		
	POWER_SUPPLY_TYPE_WIRELESS_PACK_TA,		
	POWER_SUPPLY_TYPE_WIRELESS_STAND,       
	POWER_SUPPLY_TYPE_WIRELESS_HV_STAND,    
	POWER_SUPPLY_TYPE_PDIC,			
	POWER_SUPPLY_TYPE_HV_MAINS_CHG_LIMIT,	
	POWER_SUPPLY_TYPE_HV_QC20,			
	POWER_SUPPLY_TYPE_HV_QC30,			
	POWER_SUPPLY_TYPE_MAX,
};


#define POWER_SUPPLY_TYPE_HV_WIRELESS_ETX	100

enum power_supply_notifier_events {
	PSY_EVENT_PROP_CHANGED,
};

union power_supply_propval {
	int intval;
	const char *strval;
	int64_t int64val;
};

struct device_node;
struct power_supply;


struct power_supply_config {
	struct device_node *of_node;
	
	void *drv_data;

	char **supplied_to;
	size_t num_supplicants;
};


struct power_supply_desc {
	const char *name;
	enum power_supply_type type;
	enum power_supply_property *properties;
	size_t num_properties;

	
	int (*get_property)(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val);
	int (*set_property)(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val);
	
	int (*property_is_writeable)(struct power_supply *psy,
				     enum power_supply_property psp);
	void (*external_power_changed)(struct power_supply *psy);
	void (*set_charged)(struct power_supply *psy);

	
	bool no_thermal;
	
	int use_for_apm;
};

struct power_supply {
	const struct power_supply_desc *desc;

	char **supplied_to;
	size_t num_supplicants;

	char **supplied_from;
	size_t num_supplies;
	struct device_node *of_node;

	
	void *drv_data;

	
	struct device dev;
	struct work_struct changed_work;
	struct delayed_work deferred_register_work;
	spinlock_t changed_lock;
	bool changed;
	bool initialized;
	bool removing;
	atomic_t use_cnt;
#ifdef CONFIG_THERMAL
	struct thermal_zone_device *tzd;
	struct thermal_cooling_device *tcd;
#endif

#ifdef CONFIG_LEDS_TRIGGERS
	struct led_trigger *charging_full_trig;
	char *charging_full_trig_name;
	struct led_trigger *charging_trig;
	char *charging_trig_name;
	struct led_trigger *full_trig;
	char *full_trig_name;
	struct led_trigger *online_trig;
	char *online_trig_name;
	struct led_trigger *charging_blink_full_solid_trig;
	char *charging_blink_full_solid_trig_name;
#endif
};



struct power_supply_info {
	const char *name;
	int technology;
	int voltage_max_design;
	int voltage_min_design;
	int charge_full_design;
	int charge_empty_design;
	int energy_full_design;
	int energy_empty_design;
	int use_for_apm;
};

extern struct atomic_notifier_head power_supply_notifier;
extern int power_supply_reg_notifier(struct notifier_block *nb);
extern void power_supply_unreg_notifier(struct notifier_block *nb);
extern struct power_supply *power_supply_get_by_name(const char *name);
extern void power_supply_put(struct power_supply *psy);
#ifdef CONFIG_OF
extern struct power_supply *power_supply_get_by_phandle(struct device_node *np,
							const char *property);
extern struct power_supply *devm_power_supply_get_by_phandle(
				    struct device *dev, const char *property);
#else 
static inline struct power_supply *
power_supply_get_by_phandle(struct device_node *np, const char *property)
{ return NULL; }
static inline struct power_supply *
devm_power_supply_get_by_phandle(struct device *dev, const char *property)
{ return NULL; }
#endif 
extern void power_supply_changed(struct power_supply *psy);
extern int power_supply_am_i_supplied(struct power_supply *psy);
extern int power_supply_set_battery_charged(struct power_supply *psy);

#ifdef CONFIG_POWER_SUPPLY
extern int power_supply_is_system_supplied(void);
#else
static inline int power_supply_is_system_supplied(void) { return -ENOSYS; }
#endif

extern int power_supply_get_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val);
extern int power_supply_set_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val);
extern int power_supply_property_is_writeable(struct power_supply *psy,
					enum power_supply_property psp);
extern void power_supply_external_power_changed(struct power_supply *psy);

extern struct power_supply *__must_check
power_supply_register(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
power_supply_register_no_ws(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
devm_power_supply_register(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern struct power_supply *__must_check
devm_power_supply_register_no_ws(struct device *parent,
				 const struct power_supply_desc *desc,
				 const struct power_supply_config *cfg);
extern void power_supply_unregister(struct power_supply *psy);
extern int power_supply_powers(struct power_supply *psy, struct device *dev);

extern void *power_supply_get_drvdata(struct power_supply *psy);

extern struct class *power_supply_class;

static inline bool power_supply_is_amp_property(enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
	case POWER_SUPPLY_PROP_CHARGE_EMPTY_DESIGN:
	case POWER_SUPPLY_PROP_CHARGE_FULL:
	case POWER_SUPPLY_PROP_CHARGE_EMPTY:
	case POWER_SUPPLY_PROP_CHARGE_NOW:
	case POWER_SUPPLY_PROP_CHARGE_AVG:
	case POWER_SUPPLY_PROP_CHARGE_COUNTER:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
	case POWER_SUPPLY_PROP_CURRENT_MAX:
	case POWER_SUPPLY_PROP_CURRENT_NOW:
	case POWER_SUPPLY_PROP_CURRENT_AVG:
	case POWER_SUPPLY_PROP_CURRENT_BOOT:
		return 1;
	default:
		break;
	}

	return 0;
}

static inline bool power_supply_is_watt_property(enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY_DESIGN:
	case POWER_SUPPLY_PROP_ENERGY_FULL:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
	case POWER_SUPPLY_PROP_ENERGY_NOW:
	case POWER_SUPPLY_PROP_ENERGY_AVG:
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
	case POWER_SUPPLY_PROP_VOLTAGE_MIN:
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
	case POWER_SUPPLY_PROP_VOLTAGE_AVG:
	case POWER_SUPPLY_PROP_VOLTAGE_OCV:
	case POWER_SUPPLY_PROP_VOLTAGE_BOOT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:
	case POWER_SUPPLY_PROP_POWER_NOW:
		return 1;
	default:
		break;
	}

	return 0;
}

#endif 
