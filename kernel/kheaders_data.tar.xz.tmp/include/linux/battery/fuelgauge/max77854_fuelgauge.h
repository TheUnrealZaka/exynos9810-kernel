

#ifndef __MAX77854_FUELGAUGE_H
#define __MAX77854_FUELGAUGE_H __FILE__

#include <linux/mfd/core.h>
#include <linux/mfd/max77854.h>
#include <linux/mfd/max77854-private.h>
#include <linux/regulator/machine.h>



#define PRINT_COUNT	10

#define ALERT_EN 0x04
#define CAPACITY_SCALE_DEFAULT_CURRENT 1000
#define CAPACITY_SCALE_HV_CURRENT 600

enum max77854_valrt_mode {
	NORMAL_MODE = 0,
	VEMPTY_MODE,
	VEMPTY_RECOVERY_MODE,
};

struct sec_fg_info {
	
	int pr_cnt;
	
	struct delayed_work	full_comp_work;
	u32 previous_fullcap;
	u32 previous_vffullcap;

	
	u32 soc;

	
	unsigned long fullcap_check_interval;
	int full_check_flag;
	bool is_first_check;
};

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
	FG_FULLCAPNOM,
	FG_FULLCAPREP,
	FG_MIXCAP,
	FG_AVCAP,
	FG_REPCAP,
	FG_CYCLE,
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
	u32 V_empty;
	u32 V_empty_origin;
	u32 QResidual20;
	u32 QResidual30;
	u32 TempCo;
	u32 Capacity;
	u8	*type_str;
	u32 ichgterm;
	u32 misccfg;
	u32 fullsocthr;
	u32 ichgterm_2nd;
	u32 misccfg_2nd;
	u32 fullsocthr_2nd;
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

struct cv_slope{
	int fg_current;
	int soc;
	int time;
};

struct max77854_fuelgauge_data {
	struct device           *dev;
	struct i2c_client       *i2c;
	struct i2c_client       *pmic;
	struct mutex            fuelgauge_mutex;
	struct max77854_platform_data *max77854_pdata;
	sec_fuelgauge_platform_data_t *pdata;
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

	int raw_capacity;
	int current_now;
	int current_avg;
	struct cv_slope *cv_data;
	int cv_data_lenth;

	bool using_temp_compensation;
	bool low_temp_compensation_en;
	bool using_hw_vempty;
	bool hw_v_empty;
	int sw_v_empty;

	unsigned int low_temp_limit;
	unsigned int low_temp_recovery;

	bool auto_discharge_en;
	u32 discharge_temp_threshold;
	u32 discharge_volt_threshold;

	u32 fg_resistor;
};

#endif 
