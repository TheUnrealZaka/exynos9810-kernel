

#ifndef __LINUX_LM3630A_H
#define __LINUX_LM3630A_H

#define LM3630A_NAME "lm3630a_bl"

enum lm3630a_pwm_ctrl {
	LM3630A_PWM_DISABLE = 0x00,
	LM3630A_PWM_BANK_A,
	LM3630A_PWM_BANK_B,
	LM3630A_PWM_BANK_ALL,
	LM3630A_PWM_BANK_A_ACT_LOW = 0x05,
	LM3630A_PWM_BANK_B_ACT_LOW,
	LM3630A_PWM_BANK_ALL_ACT_LOW,
};

enum lm3630a_leda_ctrl {
	LM3630A_LEDA_DISABLE = 0x00,
	LM3630A_LEDA_ENABLE = 0x04,
	LM3630A_LEDA_ENABLE_LINEAR = 0x14,
};

enum lm3630a_ledb_ctrl {
	LM3630A_LEDB_DISABLE = 0x00,
	LM3630A_LEDB_ON_A = 0x01,
	LM3630A_LEDB_ENABLE = 0x02,
	LM3630A_LEDB_ENABLE_LINEAR = 0x0A,
};

#define LM3630A_MAX_BRIGHTNESS 255

struct lm3630a_platform_data {

	
	int leda_init_brt;
	int leda_max_brt;
	enum lm3630a_leda_ctrl leda_ctrl;
	
	int ledb_init_brt;
	int ledb_max_brt;
	enum lm3630a_ledb_ctrl ledb_ctrl;
	
	unsigned int pwm_period;
	enum lm3630a_pwm_ctrl pwm_ctrl;
};

#endif 
