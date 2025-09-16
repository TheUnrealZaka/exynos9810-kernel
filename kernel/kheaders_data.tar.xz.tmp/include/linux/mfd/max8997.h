

#ifndef __LINUX_MFD_MAX8998_H
#define __LINUX_MFD_MAX8998_H

#include <linux/regulator/consumer.h>


enum max8998_regulators {
	MAX8997_LDO1 = 0,
	MAX8997_LDO2,
	MAX8997_LDO3,
	MAX8997_LDO4,
	MAX8997_LDO5,
	MAX8997_LDO6,
	MAX8997_LDO7,
	MAX8997_LDO8,
	MAX8997_LDO9,
	MAX8997_LDO10,
	MAX8997_LDO11,
	MAX8997_LDO12,
	MAX8997_LDO13,
	MAX8997_LDO14,
	MAX8997_LDO15,
	MAX8997_LDO16,
	MAX8997_LDO17,
	MAX8997_LDO18,
	MAX8997_LDO21,
	MAX8997_BUCK1,
	MAX8997_BUCK2,
	MAX8997_BUCK3,
	MAX8997_BUCK4,
	MAX8997_BUCK5,
	MAX8997_BUCK6,
	MAX8997_BUCK7,
	MAX8997_EN32KHZ_AP,
	MAX8997_EN32KHZ_CP,
	MAX8997_ENVICHG,
	MAX8997_ESAFEOUT1,
	MAX8997_ESAFEOUT2,
	MAX8997_CHARGER_CV, 
	MAX8997_CHARGER, 
	MAX8997_CHARGER_TOPOFF, 

	MAX8997_REG_MAX,
};

struct max8997_regulator_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *reg_node;
};

struct max8997_muic_reg_data {
	u8 addr;
	u8 data;
};


struct max8997_muic_platform_data {
	struct max8997_muic_reg_data *init_data;
	int num_init_data;

	
	int detcable_delay_ms;

	
	int path_usb;
	int path_uart;
};

enum max8997_haptic_motor_type {
	MAX8997_HAPTIC_ERM,
	MAX8997_HAPTIC_LRA,
};

enum max8997_haptic_pulse_mode {
	MAX8997_EXTERNAL_MODE,
	MAX8997_INTERNAL_MODE,
};

enum max8997_haptic_pwm_divisor {
	MAX8997_PWM_DIVISOR_32,
	MAX8997_PWM_DIVISOR_64,
	MAX8997_PWM_DIVISOR_128,
	MAX8997_PWM_DIVISOR_256,
};


struct max8997_haptic_platform_data {
	unsigned int pwm_channel_id;
	unsigned int pwm_period;

	enum max8997_haptic_motor_type type;
	enum max8997_haptic_pulse_mode mode;
	enum max8997_haptic_pwm_divisor pwm_divisor;

	unsigned int internal_mode_pattern;
	unsigned int pattern_cycle;
	unsigned int pattern_signal_period;
};

enum max8997_led_mode {
	MAX8997_NONE,
	MAX8997_FLASH_MODE,
	MAX8997_MOVIE_MODE,
	MAX8997_FLASH_PIN_CONTROL_MODE,
	MAX8997_MOVIE_PIN_CONTROL_MODE,
};


struct max8997_led_platform_data {
	enum max8997_led_mode mode[2];
	u8 brightness[2];
};

struct max8997_platform_data {
	
	int ono;

	
	struct max8997_regulator_data *regulators;
	int num_regulators;

	
	bool ignore_gpiodvs_side_effect;
	int buck125_gpios[3]; 
	int buck125_default_idx; 
	unsigned int buck1_voltage[8]; 
	bool buck1_gpiodvs;
	unsigned int buck2_voltage[8];
	bool buck2_gpiodvs;
	unsigned int buck5_voltage[8];
	bool buck5_gpiodvs;

	
	
	int eoc_mA; 
	
	int timeout; 

	
	struct max8997_muic_platform_data *muic_pdata;

	
	struct max8997_haptic_platform_data *haptic_pdata;

	
	
	struct max8997_led_platform_data *led_pdata;
};

#endif 
