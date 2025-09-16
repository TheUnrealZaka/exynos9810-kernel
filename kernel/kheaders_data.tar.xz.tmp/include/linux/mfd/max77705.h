

#ifndef __MAX77705_H__
#define __MAX77705_H__
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define MFD_DEV_NAME "max77705"
#define M2SH(m) ((m) & 0x0F ? ((m) & 0x03 ? ((m) & 0x01 ? 0 : 1) : ((m) & 0x04 ? 2 : 3)) : \
		((m) & 0x30 ? ((m) & 0x10 ? 4 : 5) : ((m) & 0x40 ? 6 : 7)))

#if defined(CONFIG_MOTOR_DRV_MAX77705)
struct max77705_haptic_pdata {
	u16 max_timeout;
	u16 duty;
	u16 period;
	u16 reg2;
	bool overdrive_state;
	int gpio;
	char *regulator_name;
	unsigned int pwm_id;
	const char *vib_type;

	
	int multi_frequency;
	int freq_num;
	u32 *multi_freq_duty;
	u32 *multi_freq_period;
	int normal_ratio;
	int overdrive_ratio;
};
#endif

#if defined(CONFIG_MAX77705_VIBRATOR)
struct max77705_vibrator_pdata {
	int gpio;
	char *regulator_name;
	struct pwm_device *pwm;
	unsigned int pwm_id;
	const char *motor_type;

	int freq;
	
	int freq_nums;
	u32 *freq_array;
	u32 *ratio_array; 
	int normal_ratio;
	int overdrive_ratio;
	int high_temp_ratio;
	int high_temp_ref;
	int fold_open_ratio;
	int fold_close_ratio;
#if defined(CONFIG_SEC_VIBRATOR)
	bool calibration;
	int steps;
	int *intensities;
	int *haptic_intensities;
#endif
};
#endif

struct max77705_regulator_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *reg_node;
};

struct max77705_platform_data {
	
	int irq_base;
	int irq_gpio;
	bool wakeup;
	int wpc_en;
	struct muic_platform_data *muic_pdata;

	int num_regulators;
	struct max77705_regulator_data *regulators;
#if defined(CONFIG_MOTOR_DRV_MAX77705)
	struct max77705_haptic_pdata *haptic_data;
#endif
#if defined(CONFIG_MAX77705_VIBRATOR)
	struct max77705_vibrator_pdata *vibrator_data;
#endif
	struct mfd_cell *sub_devices;
	int num_subdevs;
	char *wireless_charger_name;
};

struct max77705 {
	struct regmap *regmap;
};

#endif 

