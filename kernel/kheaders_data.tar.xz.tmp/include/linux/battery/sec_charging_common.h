

#ifndef __SEC_CHARGING_COMMON_H
#define __SEC_CHARGING_COMMON_H __FILE__

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/power_supply.h>
#include <linux/slab.h>
#include <linux/device.h>


#define	SEC_SIZEOF_POWER_SUPPLY_TYPE	POWER_SUPPLY_TYPE_MAX

enum sec_battery_voltage_mode {
	
	SEC_BATTERY_VOLTAGE_AVERAGE = 0,
	
	SEC_BATTERY_VOLTAGE_OCV,
};

enum sec_battery_current_mode {
	
	SEC_BATTERY_CURRENT_UA = 0,
	
	SEC_BATTERY_CURRENT_MA,
};

enum sec_battery_capacity_mode {
	
	SEC_BATTERY_CAPACITY_DESIGNED = 0,
	
	SEC_BATTERY_CAPACITY_ABSOLUTE,
	
	SEC_BATTERY_CAPACITY_TEMPERARY,
	
	SEC_BATTERY_CAPACITY_CURRENT,
	
	SEC_BATTERY_CAPACITY_AGEDCELL,
	
	SEC_BATTERY_CAPACITY_CYCLE,
};

enum sec_wireless_info_mode {
	SEC_WIRELESS_OTP_FIRM_RESULT = 0,
	SEC_WIRELESS_IC_GRADE,
	SEC_WIRELESS_IC_REVISION,
	SEC_WIRELESS_OTP_FIRM_VER_BIN,
	SEC_WIRELESS_OTP_FIRM_VER,
	SEC_WIRELESS_TX_FIRM_RESULT,
	SEC_WIRELESS_TX_FIRM_VER,
	SEC_TX_FIRMWARE,
	SEC_WIRELESS_OTP_FIRM_VERIFY,
};

enum sec_wireless_firm_update_mode {
	SEC_WIRELESS_RX_SDCARD_MODE = 0,
	SEC_WIRELESS_RX_BUILT_IN_MODE,
	SEC_WIRELESS_TX_ON_MODE,
	SEC_WIRELESS_TX_OFF_MODE,
	SEC_WIRELESS_RX_INIT,
};

enum sec_tx_firmware_mode {
	SEC_TX_OFF = 0,
	SEC_TX_STANDBY,
	SEC_TX_POWER_TRANSFER,
	SEC_TX_ERROR,
};

enum sec_wireless_control_mode {
	WIRELESS_VOUT_OFF = 0,
	WIRELESS_VOUT_NORMAL_VOLTAGE,	
	WIRELESS_VOUT_RESERVED,			
	WIRELESS_VOUT_HIGH_VOLTAGE,		
	WIRELESS_VOUT_CC_CV_VOUT,
	WIRELESS_VOUT_CV_CALL,
	WIRELESS_VOUT_CC_CALL,
	WIRELESS_VOUT_5V,
	WIRELESS_VOUT_9V,
	WIRELESS_VOUT_9V_OTG,
	WIRELESS_PAD_FAN_OFF,
	WIRELESS_PAD_FAN_ON,
	WIRELESS_PAD_LED_OFF,
	WIRELESS_PAD_LED_ON,
	WIRELESS_VRECT_ADJ_ON,
	WIRELESS_VRECT_ADJ_OFF,
	WIRELESS_VRECT_ADJ_ROOM_0,
	WIRELESS_VRECT_ADJ_ROOM_1,
	WIRELESS_VRECT_ADJ_ROOM_2,
	WIRELESS_VRECT_ADJ_ROOM_3,
	WIRELESS_VRECT_ADJ_ROOM_4,
	WIRELESS_VRECT_ADJ_ROOM_5,
	WIRELESS_CLAMP_ENABLE,
};

enum sec_siop_event_mode {
	SIOP_EVENT_IDLE = 0,
	SIOP_EVENT_WPC_CALL_START,		
	SIOP_EVENT_WPC_CALL_END,		
	SIOP_EVENT_MAX,					
};

enum sec_wireless_pad_mode {
	SEC_WIRELESS_PAD_NONE = 0,
	SEC_WIRELESS_PAD_WPC,
	SEC_WIRELESS_PAD_WPC_HV,
	SEC_WIRELESS_PAD_WPC_PACK,
	SEC_WIRELESS_PAD_WPC_PACK_TA,
	SEC_WIRELESS_PAD_WPC_STAND,
	SEC_WIRELESS_PAD_WPC_STAND_HV,
	SEC_WIRELESS_PAD_PMA,
};


enum sec_battery_adc_type {
	
	SEC_BATTERY_ADC_TYPE_NONE = 0,
	
	SEC_BATTERY_ADC_TYPE_AP,
	
	SEC_BATTERY_ADC_TYPE_IC,
	SEC_BATTERY_ADC_TYPE_NUM
};

enum sec_battery_adc_channel {
	SEC_BAT_ADC_CHANNEL_CABLE_CHECK = 0,
	SEC_BAT_ADC_CHANNEL_BAT_CHECK,
	SEC_BAT_ADC_CHANNEL_TEMP,
	SEC_BAT_ADC_CHANNEL_TEMP_AMBIENT,
	SEC_BAT_ADC_CHANNEL_FULL_CHECK,
	SEC_BAT_ADC_CHANNEL_VOLTAGE_NOW,
	SEC_BAT_ADC_CHANNEL_CHG_TEMP,
	SEC_BAT_ADC_CHANNEL_INBAT_VOLTAGE,
	SEC_BAT_ADC_CHANNEL_DISCHARGING_CHECK,
	SEC_BAT_ADC_CHANNEL_DISCHARGING_NTC,
	SEC_BAT_ADC_CHANNEL_WPC_TEMP,
	SEC_BAT_ADC_CHANNEL_SLAVE_CHG_TEMP,
	SEC_BAT_ADC_CHANNEL_NUM,
};

enum sec_battery_charge_mode {
	SEC_BAT_CHG_MODE_CHARGING = 0,
	SEC_BAT_CHG_MODE_CHARGING_OFF,
	SEC_BAT_CHG_MODE_BUCK_OFF,
};


enum sec_battery_charging_mode {
	
	SEC_BATTERY_CHARGING_NONE = 0,
	
	SEC_BATTERY_CHARGING_1ST,
	
	SEC_BATTERY_CHARGING_2ND,
	
	SEC_BATTERY_CHARGING_RECHARGING,
};


enum sec_battery_chg_temp_state {
	SEC_BATTERY_CHG_TEMP_NONE = 0,
	SEC_BATTERY_CHG_TEMP_HIGH_1ST,
	SEC_BATTERY_CHG_TEMP_HIGH_2ND,
};


enum sec_battery_wpc_pad_state {
	SEC_BATTERY_WPC_TEMP_NONE = 0,
	SEC_BATTERY_WPC_TEMP_HIGH,
};


enum sec_battery_bat_temp_state {
	SEC_BATTERY_MIX_TEMP_NONE = 0,
	SEC_BATTERY_MIX_TEMP_HIGH,
};


enum sec_battery_wc_heat_state {
	SEC_BATTERY_WC_HEAT_NONE = 0, 
	SEC_BATTERY_WC_HEAT_HIGH, 
};
struct sec_bat_adc_api {
	bool (*init)(struct platform_device *);
	bool (*exit)(void);
	int (*read)(unsigned int);
};
#define sec_bat_adc_api_t struct sec_bat_adc_api


enum sec_battery_polling_time_type {
	
	SEC_BATTERY_POLLING_TIME_BASIC = 0,
	SEC_BATTERY_POLLING_TIME_CHARGING,
	SEC_BATTERY_POLLING_TIME_DISCHARGING,
	SEC_BATTERY_POLLING_TIME_NOT_CHARGING,
	SEC_BATTERY_POLLING_TIME_SLEEP,
};

enum sec_battery_monitor_polling {
	
	SEC_BATTERY_MONITOR_WORKQUEUE,
	
	SEC_BATTERY_MONITOR_ALARM,
	
	SEC_BATTERY_MONITOR_TIMER,
};
#define sec_battery_monitor_polling_t \
	enum sec_battery_monitor_polling


enum sec_battery_full_charged {
	SEC_BATTERY_FULLCHARGED_NONE = 0,
	
	SEC_BATTERY_FULLCHARGED_ADC,
	
	SEC_BATTERY_FULLCHARGED_FG_CURRENT,
	
	SEC_BATTERY_FULLCHARGED_TIME,
	
	SEC_BATTERY_FULLCHARGED_SOC,
	
	SEC_BATTERY_FULLCHARGED_CHGGPIO,
	
	SEC_BATTERY_FULLCHARGED_CHGINT,
	
	SEC_BATTERY_FULLCHARGED_CHGPSY,
};


enum sec_battery_discharger_type {
	
	SEC_BAT_SELF_DISCHARGING_BY_ADC = 0,
	
	SEC_BAT_SELF_DISCHARGING_BY_FG,
};

#define sec_battery_full_charged_t \
	enum sec_battery_full_charged


#define sec_battery_full_condition_t unsigned int

#define SEC_BATTERY_FULL_CONDITION_NOTIMEFULL	1

#define SEC_BATTERY_FULL_CONDITION_NOSLEEPINFULL	2

#define SEC_BATTERY_FULL_CONDITION_SOC		4

#define SEC_BATTERY_FULL_CONDITION_VCELL	8

#define SEC_BATTERY_FULL_CONDITION_AVGVCELL	16

#define SEC_BATTERY_FULL_CONDITION_OCV		32


#define sec_battery_recharge_condition_t unsigned int

#define SEC_BATTERY_RECHARGE_CONDITION_SOC		1

#define SEC_BATTERY_RECHARGE_CONDITION_AVGVCELL		2

#define SEC_BATTERY_RECHARGE_CONDITION_VCELL		4


enum sec_battery_check {
	
	SEC_BATTERY_CHECK_NONE,
	
	SEC_BATTERY_CHECK_ADC,
	
	SEC_BATTERY_CHECK_CALLBACK,
	
	SEC_BATTERY_CHECK_PMIC,
	
	SEC_BATTERY_CHECK_FUELGAUGE,
	
	SEC_BATTERY_CHECK_CHARGER,
	
	SEC_BATTERY_CHECK_INT,
};
#define sec_battery_check_t \
	enum sec_battery_check


enum sec_battery_ovp_uvlo {
	
	SEC_BATTERY_OVP_UVLO_CALLBACK,
	
	SEC_BATTERY_OVP_UVLO_PMICPOLLING,
	
	SEC_BATTERY_OVP_UVLO_PMICINT,
	
	SEC_BATTERY_OVP_UVLO_CHGPOLLING,
	
	SEC_BATTERY_OVP_UVLO_CHGINT,
};
#define sec_battery_ovp_uvlo_t \
	enum sec_battery_ovp_uvlo


enum sec_battery_thermal_source {
	
	SEC_BATTERY_THERMAL_SOURCE_FG,
	
	SEC_BATTERY_THERMAL_SOURCE_CALLBACK,
	
	SEC_BATTERY_THERMAL_SOURCE_ADC,
};
#define sec_battery_thermal_source_t \
	enum sec_battery_thermal_source


enum sec_battery_temp_check {
	SEC_BATTERY_TEMP_CHECK_NONE = 0,	
	SEC_BATTERY_TEMP_CHECK_ADC,	
	SEC_BATTERY_TEMP_CHECK_TEMP,	
};
#define sec_battery_temp_check_t \
	enum sec_battery_temp_check


#define sec_battery_cable_check_t unsigned int

#define SEC_BATTERY_CABLE_CHECK_NOUSBCHARGE		1

#define SEC_BATTERY_CABLE_CHECK_NOINCOMPATIBLECHARGE	2

#define SEC_BATTERY_CABLE_CHECK_PSY			4

#define SEC_BATTERY_CABLE_CHECK_INT			8

#define SEC_BATTERY_CABLE_CHECK_CHGINT			16

#define SEC_BATTERY_CABLE_CHECK_POLLING			32


#define sec_battery_cable_source_t unsigned int

#define	SEC_BATTERY_CABLE_SOURCE_EXTERNAL	1

#define	SEC_BATTERY_CABLE_SOURCE_CALLBACK	2

#define	SEC_BATTERY_CABLE_SOURCE_ADC		4


#define sec_fuelgauge_capacity_type_t int

#define SEC_FUELGAUGE_CAPACITY_TYPE_RESET	(-1)

#define SEC_FUELGAUGE_CAPACITY_TYPE_RAW		1

#define SEC_FUELGAUGE_CAPACITY_TYPE_SCALE	2

#define SEC_FUELGAUGE_CAPACITY_TYPE_DYNAMIC_SCALE	4

#define SEC_FUELGAUGE_CAPACITY_TYPE_ATOMIC	8

#define SEC_FUELGAUGE_CAPACITY_TYPE_SKIP_ABNORMAL	16


#define sec_charger_functions_t unsigned int

#define SEC_CHARGER_NO_GRADUAL_CHARGING_CURRENT		1


#define SEC_CHARGER_MINIMUM_SIOP_CHARGING_CURRENT	2


struct sec_bat_adc_table_data {
	int adc;
	int data;
};
#define sec_bat_adc_table_data_t \
	struct sec_bat_adc_table_data

struct sec_bat_adc_region {
	int min;
	int max;
};
#define sec_bat_adc_region_t \
	struct sec_bat_adc_region

struct sec_charging_current {
#ifdef CONFIG_OF
	unsigned int input_current_limit;
	unsigned int fast_charging_current;
	unsigned int full_check_current_1st;
	unsigned int full_check_current_2nd;
#else
	int input_current_limit;
	int fast_charging_current;
	int full_check_current_1st;
	int full_check_current_2nd;
#endif
};

#define sec_charging_current_t \
	struct sec_charging_current

#if defined(CONFIG_BATTERY_AGE_FORECAST)
struct sec_age_data {
	unsigned int cycle;
	unsigned int float_voltage;
	unsigned int recharge_condition_vcell;
	unsigned int full_condition_vcell;
	unsigned int full_condition_soc;
};

#define sec_age_data_t \
	struct sec_age_data
#endif

struct sec_battery_platform_data {
	
	
	void (*initial_check)(void);
	void (*monitor_additional_check)(void);
	bool (*bat_gpio_init)(void);
	bool (*fg_gpio_init)(void);
	bool (*is_lpm)(void);
	bool (*check_jig_status)(void);
	bool (*is_interrupt_cable_check_possible)(int);
	int (*check_cable_callback)(void);
	int (*get_cable_from_extended_cable_type)(int);
	bool (*cable_switch_check)(void);
	bool (*cable_switch_normal)(void);
	bool (*check_cable_result_callback)(int);
	bool (*check_battery_callback)(void);
	bool (*check_battery_result_callback)(void);
	int (*ovp_uvlo_callback)(void);
	bool (*ovp_uvlo_result_callback)(int);
	bool (*fuelalert_process)(bool);
	bool (*get_temperature_callback)(
			enum power_supply_property,
			union power_supply_propval*);

	
	sec_bat_adc_api_t adc_api[SEC_BATTERY_ADC_TYPE_NUM];
	
	sec_bat_adc_region_t *cable_adc_value;
	
	sec_charging_current_t *charging_current;
#ifdef CONFIG_OF
	unsigned int *polling_time;
	char *chip_vendor;
	unsigned int temp_adc_type;
#else
	int *polling_time;
#endif
	
	unsigned int pre_afc_input_current;
	unsigned int pre_wc_afc_input_current;
	unsigned int store_mode_afc_input_current;
	unsigned int store_mode_hv_wireless_input_current;

	char *pmic_name;

	
	char *vendor;
	int technology;
	int battery_type;
	void *battery_data;

	int bat_gpio_ta_nconnected;
	
	int bat_polarity_ta_nconnected;
	int bat_irq;
	int bat_irq_gpio;
	unsigned long bat_irq_attr;
	int jig_irq;
	unsigned long jig_irq_attr;
	sec_battery_cable_check_t cable_check_type;
	sec_battery_cable_source_t cable_source_type;

	bool use_LED;				

	
	bool swelling_mode_skip_in_high_temp;
	
	unsigned int event_waiting_time;

	
	int swelling_high_temp_block;
	int swelling_high_temp_recov;
	int swelling_low_temp_block;
	int swelling_low_temp_recov;
	unsigned int swelling_low_temp_current;
	unsigned int swelling_low_temp_topoff;
	unsigned int swelling_high_temp_current;
	unsigned int swelling_high_temp_topoff;
	unsigned int swelling_normal_float_voltage;
	unsigned int swelling_drop_float_voltage;
	unsigned int swelling_high_rechg_voltage;
	unsigned int swelling_low_rechg_voltage;

#if defined(CONFIG_CALC_TIME_TO_FULL)
	unsigned int ttf_hv_charge_current;
	unsigned int ttf_hv_wireless_charge_current;
#endif

#if defined(CONFIG_STEP_CHARGING)
	
	unsigned int *step_charging_condition;
	unsigned int *step_charging_current;
#endif

	
	bool self_discharging_en;
	unsigned int discharging_adc_max;
	unsigned int discharging_adc_min;
	unsigned int self_discharging_voltage_limit;
	unsigned int discharging_ntc_limit;
	int force_discharging_limit;
	int force_discharging_recov;
	int factory_discharging;
	unsigned int self_discharging_type;
#if defined(CONFIG_SW_SELF_DISCHARGING)
	
	int self_discharging_temp_block;
	int self_discharging_volt_block;
	int self_discharging_temp_recov;
	int self_discharging_temp_pollingtime;
#endif

	
	sec_battery_monitor_polling_t polling_type;
	
	unsigned int monitor_initial_count;

	
	sec_battery_check_t battery_check_type;
	
	unsigned int check_count;
	
	
	unsigned int check_adc_max;
	
	unsigned int check_adc_min;

	
	sec_battery_ovp_uvlo_t ovp_uvlo_check_type;

	sec_battery_thermal_source_t thermal_source;

	
#ifdef CONFIG_OF
	sec_bat_adc_table_data_t *temp_adc_table;
	sec_bat_adc_table_data_t *temp_amb_adc_table;
	sec_bat_adc_table_data_t *chg_temp_adc_table;
	sec_bat_adc_table_data_t *wpc_temp_adc_table;
	sec_bat_adc_table_data_t *slave_chg_temp_adc_table;
	sec_bat_adc_table_data_t *inbat_adc_table;
#else
	const sec_bat_adc_table_data_t *temp_adc_table;
	const sec_bat_adc_table_data_t *temp_amb_adc_table;
#endif
	unsigned int temp_adc_table_size;
	unsigned int temp_amb_adc_table_size;
	unsigned int chg_temp_adc_table_size;
	unsigned int wpc_temp_adc_table_size;
	unsigned int slave_chg_temp_adc_table_size;
	unsigned int inbat_adc_table_size;

	sec_battery_temp_check_t temp_check_type;
	unsigned int temp_check_count;
	unsigned int chg_temp_check; 
	unsigned int chg_thermal_source; 
	unsigned int wpc_temp_check;
	unsigned int mix_temp_check;
	unsigned int wpc_thermal_source; 
	unsigned int slave_chg_temp_check;
	unsigned int slave_thermal_source; 
	unsigned int inbat_voltage;

	
	int temp_highlimit_threshold_normal;
	int temp_highlimit_recovery_normal;
	int temp_high_threshold_normal;
	int temp_high_recovery_normal;
	int temp_low_threshold_normal;
	int temp_low_recovery_normal;
	int temp_highlimit_threshold_lpm;
	int temp_highlimit_recovery_lpm;
	int temp_high_threshold_lpm;
	int temp_high_recovery_lpm;
	int temp_low_threshold_lpm;
	int temp_low_recovery_lpm;
	int chg_high_temp_1st;
	int chg_high_temp_2nd;
	int chg_high_temp_recovery;
	unsigned int chg_charging_limit_current;
	unsigned int chg_charging_limit_current_2nd;
	unsigned int chg_skip_check_time;
	unsigned int chg_skip_check_capacity;
	int wpc_high_temp;
	int wpc_high_temp_recovery;
	int wpc_heat_temp_recovery;
	int wpc_lcd_on_high_temp;
	int wpc_lcd_on_high_temp_rec;
	unsigned int wpc_hv_lcd_on_input_limit_current;
	unsigned int wpc_charging_limit_current;
	unsigned int sleep_mode_limit_current;
	unsigned int wc_full_input_limit_current;
	unsigned int wc_heating_input_limit_current;
	unsigned int wc_heating_time;
	unsigned int wc_cv_current;
	unsigned int wpc_skip_check_time;
	unsigned int wpc_skip_check_capacity;
	int mix_high_tbat;
	int mix_high_tchg;
	int mix_high_tbat_recov;
	unsigned int mix_input_limit_current;
	int mix_high_tbat_hv;
	int mix_high_tchg_hv;
	int mix_high_tbat_recov_hv;
	unsigned int mix_input_limit_current_hv;

	
	
	sec_battery_full_charged_t full_check_type;
	
	sec_battery_full_charged_t full_check_type_2nd;
	unsigned int full_check_count;
	int chg_gpio_full_check;
	
	int chg_polarity_full_check;
	sec_battery_full_condition_t full_condition_type;
	unsigned int full_condition_soc;
	unsigned int full_condition_vcell;
	unsigned int full_condition_avgvcell;
	unsigned int full_condition_ocv;

	unsigned int recharge_check_count;
	sec_battery_recharge_condition_t recharge_condition_type;
	unsigned int recharge_condition_soc;
	unsigned int recharge_condition_avgvcell;
	unsigned int recharge_condition_vcell;

	
	unsigned long charging_total_time;
	
	unsigned long recharging_total_time;
	
	unsigned long charging_reset_time;

	
	char *fuelgauge_name;
	int fg_irq;
	unsigned long fg_irq_attr;
	
	int fuel_alert_soc;
	
	bool repeated_fuelalert;
	sec_fuelgauge_capacity_type_t capacity_calculation_type;
	
	int capacity_max;
	int capacity_max_hv;

	int capacity_max_margin;
	int capacity_min;

	
	char *charger_name;
	char *fgsrc_switch_name;
	bool support_fgsrc_change;

	
	char *wireless_charger_name;
	int wireless_cc_cv;
	int wpc_det;
	int wpc_en;

	int chg_gpio_en;
	int chg_irq;
	unsigned long chg_irq_attr;
	
#ifdef CONFIG_OF
	unsigned int chg_float_voltage;
#else
	int chg_float_voltage;
#endif
#if defined(CONFIG_BATTERY_AGE_FORECAST)
	int num_age_step;
	int age_step;
	int age_data_length;
	sec_age_data_t* age_data;
#endif
	unsigned int siop_event_check_type;
	unsigned int siop_call_cc_current;
	unsigned int siop_call_cv_current;

	int siop_input_limit_current;
	int siop_charging_limit_current;
	int siop_hv_input_limit_current;
	int siop_hv_charging_limit_current;
	int siop_wireless_input_limit_current;
	int siop_wireless_charging_limit_current;
	int siop_hv_wireless_input_limit_current;
	int siop_hv_wireless_charging_limit_current;

	sec_charger_functions_t chg_functions_setting;

	bool fake_capacity;

	
	unsigned int adc_check_count;
	
	unsigned int adc_type[];
};

struct sec_charger_platform_data {
	bool (*chg_gpio_init)(void);

	
	sec_charging_current_t *charging_current;

	
	char *wireless_charger_name;
	int wireless_cc_cv;

	int vbus_ctrl_gpio;
	int chg_gpio_en;
	
	int chg_float_voltage;
	int irq_gpio;
	int chg_irq;
	unsigned long chg_irq_attr;

	
	int otg_en;

	
	sec_battery_ovp_uvlo_t ovp_uvlo_check_type;
	
	sec_battery_full_charged_t full_check_type;
	
	sec_battery_full_charged_t full_check_type_2nd;

	sec_charger_functions_t chg_functions_setting;
};

struct sec_fuelgauge_platform_data {
	bool (*fg_gpio_init)(void);
	bool (*check_jig_status)(void);
	int (*check_cable_callback)(void);
	bool (*fuelalert_process)(bool);

	
	sec_charging_current_t *charging_current;

	int jig_irq;
	int jig_gpio;
	unsigned long jig_irq_attr;

	sec_battery_thermal_source_t thermal_source;

	int fg_irq;
	unsigned long fg_irq_attr;
	
	int fuel_alert_soc;
	int fuel_alert_vol;
	
	bool repeated_fuelalert;
	sec_fuelgauge_capacity_type_t capacity_calculation_type;
	
	int capacity_max;
	int capacity_max_hv;
	int capacity_max_margin;
	int capacity_min;

#if defined(CONFIG_BATTERY_AGE_FORECAST)
	unsigned int full_condition_soc;
#endif
};

#define sec_battery_platform_data_t \
	struct sec_battery_platform_data

#define sec_charger_platform_data_t \
	struct sec_charger_platform_data

#define sec_fuelgauge_platform_data_t \
	struct sec_fuelgauge_platform_data

static inline struct power_supply *get_power_supply_by_name(char *name)
{
	if (!name)
		return (struct power_supply *)NULL;
	else
		return power_supply_get_by_name(name);
}

#define psy_do_property(name, function, property, value) \
{	\
	struct power_supply *psy;	\
	int ret;	\
	psy = get_power_supply_by_name((name));	\
	if (!psy) {	\
		pr_err("%s: Fail to "#function" psy (%s)\n",	\
			__func__, (name));	\
		value.intval = 0;	\
	} else {	\
		if (psy->desc->function##_property != NULL) { \
			ret = psy->desc->function##_property(psy, (property), &(value)); \
			if (ret < 0) {	\
				pr_err("%s: Fail to %s "#function" (%d=>%d)\n", \
						__func__, name, (property), ret);	\
				value.intval = 0;	\
			}	\
		}	\
	}	\
}

#ifndef CONFIG_OF
#define adc_init(pdev, pdata, channel)	\
	(((pdata)->adc_api)[((((pdata)->adc_type[(channel)]) <	\
	SEC_BATTERY_ADC_TYPE_NUM) ? ((pdata)->adc_type[(channel)]) :	\
	SEC_BATTERY_ADC_TYPE_NONE)].init((pdev)))

#define adc_exit(pdata, channel)	\
	(((pdata)->adc_api)[((pdata)->adc_type[(channel)])].exit())

#define adc_read(pdata, channel)	\
	(((pdata)->adc_api)[((pdata)->adc_type[(channel)])].read((channel)))
#endif

#define get_battery_data(driver)	\
	(((struct battery_data_t *)(driver)->pdata->battery_data)	\
	[(driver)->pdata->battery_type])

#endif 
