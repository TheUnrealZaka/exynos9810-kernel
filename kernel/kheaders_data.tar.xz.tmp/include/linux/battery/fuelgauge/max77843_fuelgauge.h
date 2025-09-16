

#ifndef __MAX17050_FUELGAUGE_H
#define __MAX17050_FUELGAUGE_H __FILE__

#if defined(ANDROID_ALARM_ACTIVATED)
#include <linux/android_alarm.h>
#endif

#include <linux/mfd/core.h>
#include <linux/mfd/max77843.h>
#include <linux/mfd/max77843-private.h>
#include <linux/regulator/machine.h>



#define PRINT_COUNT	10

#define LOW_BATT_COMP_RANGE_NUM	5
#define LOW_BATT_COMP_LEVEL_NUM	2
#define MAX_LOW_BATT_CHECK_CNT	10

#define ALERT_EN 0x04

enum {
	FG_LEVEL = 0,
	FG_TEMPERATURE,
	FG_VOLTAGE,
	FG_CURRENT,
	FG_CURRENT_AVG,
	FG_CHECK_STATUS,
	FG_RAW_SOC,
	FG_VF_SOC,
	FG_AV_SOC,
	FG_FULLCAP,
	FG_MIXCAP,
	FG_AVCAP,
	FG_REPCAP,
};

enum {
	POSITIVE = 0,
	NEGATIVE,
};

enum {
	RANGE = 0,
	SLOPE,
	OFFSET,
	TABLE_MAX
};

#define CURRENT_RANGE_MAX_NUM	5

struct battery_data_t {
	u32 QResidual20;
	u32 QResidual30;
	u32 Capacity;
	u32 low_battery_comp_voltage;
	s32 low_battery_table[CURRENT_RANGE_MAX_NUM][TABLE_MAX];
	u8	*type_str;
	u32 ichgterm;
	u32 misccfg;
	u32 fullsocthr;
	u32 ichgterm_2nd;
	u32 misccfg_2nd;
	u32 fullsocthr_2nd;
};

struct sec_fg_info {
	
	int pr_cnt;
	
	struct delayed_work	full_comp_work;
	u32 previous_fullcap;
	u32 previous_vffullcap;
	
	int low_batt_comp_cnt[LOW_BATT_COMP_RANGE_NUM][LOW_BATT_COMP_LEVEL_NUM];
	int low_batt_comp_flag;
	
	int low_batt_boot_flag;
	bool is_low_batt_alarm;

	
	u32 soc;

	
	unsigned long fullcap_check_interval;
	int full_check_flag;
	bool is_first_check;
};


#define VFFULLCAP_CHECK_INTERVAL	300 

#define VFSOC_FOR_FULLCAP_LEARNING	950
#define LOW_CURRENT_FOR_FULLCAP_LEARNING	20
#define HIGH_CURRENT_FOR_FULLCAP_LEARNING	120
#define LOW_AVGCURRENT_FOR_FULLCAP_LEARNING	20
#define HIGH_AVGCURRENT_FOR_FULLCAP_LEARNING	100



#define POWER_OFF_SOC_HIGH_MARGIN	20
#define POWER_OFF_VOLTAGE_HIGH_MARGIN	3500
#define POWER_OFF_VOLTAGE_LOW_MARGIN	3400



#define STABLE_LOW_BATTERY_DIFF	30
#define STABLE_LOW_BATTERY_DIFF_LOWBATT	10
#define LOW_BATTERY_SOC_REDUCE_UNIT	10

struct max77843_fuelgauge_data {
	struct device           *dev;
	struct i2c_client       *i2c;
	struct i2c_client       *pmic;
	struct mutex            fuelgauge_mutex;
	struct max77843_platform_data *max77843_pdata;
	sec_battery_platform_data_t *pdata;
	struct power_supply		psy_fg;
	struct delayed_work isr_work;

	int cable_type;
	bool is_charging;

	
	struct sec_fg_info	info;
	struct battery_data_t        *battery_data;

	bool is_fuel_alerted;
	struct wake_lock fuel_alert_wake_lock;

	unsigned int capacity_old;	
	unsigned int capacity_max;	
	unsigned int standard_capacity;

	bool initial_update_of_soc;
	struct mutex fg_lock;

	
	int reg_addr;
	u8 reg_data[2];

	unsigned int pre_soc;
	int fg_irq;
};

#endif 
