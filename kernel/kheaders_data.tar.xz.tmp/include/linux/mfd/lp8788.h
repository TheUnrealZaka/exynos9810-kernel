

#ifndef __MFD_LP8788_H__
#define __MFD_LP8788_H__

#include <linux/gpio.h>
#include <linux/irqdomain.h>
#include <linux/pwm.h>
#include <linux/regmap.h>

#define LP8788_DEV_BUCK		"lp8788-buck"
#define LP8788_DEV_DLDO		"lp8788-dldo"
#define LP8788_DEV_ALDO		"lp8788-aldo"
#define LP8788_DEV_CHARGER	"lp8788-charger"
#define LP8788_DEV_RTC		"lp8788-rtc"
#define LP8788_DEV_BACKLIGHT	"lp8788-backlight"
#define LP8788_DEV_VIBRATOR	"lp8788-vibrator"
#define LP8788_DEV_KEYLED	"lp8788-keyled"
#define LP8788_DEV_ADC		"lp8788-adc"

#define LP8788_NUM_BUCKS	4
#define LP8788_NUM_DLDOS	12
#define LP8788_NUM_ALDOS	10
#define LP8788_NUM_BUCK2_DVS	2

#define LP8788_CHG_IRQ		"CHG_IRQ"
#define LP8788_PRSW_IRQ		"PRSW_IRQ"
#define LP8788_BATT_IRQ		"BATT_IRQ"
#define LP8788_ALM_IRQ		"ALARM_IRQ"

enum lp8788_int_id {
	
	LP8788_INT_TSDL,
	LP8788_INT_TSDH,
	LP8788_INT_UVLO,
	LP8788_INT_FLAGMON,
	LP8788_INT_PWRON_TIME,
	LP8788_INT_PWRON,
	LP8788_INT_COMP1,
	LP8788_INT_COMP2,

	
	LP8788_INT_CHG_INPUT_STATE,
	LP8788_INT_CHG_STATE,
	LP8788_INT_EOC,
	LP8788_INT_CHG_RESTART,
	LP8788_INT_RESTART_TIMEOUT,
	LP8788_INT_FULLCHG_TIMEOUT,
	LP8788_INT_PRECHG_TIMEOUT,

	
	LP8788_INT_RTC_ALARM1 = 17,
	LP8788_INT_RTC_ALARM2,
	LP8788_INT_ENTER_SYS_SUPPORT,
	LP8788_INT_EXIT_SYS_SUPPORT,
	LP8788_INT_BATT_LOW,
	LP8788_INT_NO_BATT,

	LP8788_INT_MAX = 24,
};

enum lp8788_dvs_sel {
	DVS_SEL_V0,
	DVS_SEL_V1,
	DVS_SEL_V2,
	DVS_SEL_V3,
};

enum lp8788_ext_ldo_en_id {
	EN_ALDO1,
	EN_ALDO234,
	EN_ALDO5,
	EN_ALDO7,
	EN_DLDO7,
	EN_DLDO911,
	EN_LDOS_MAX,
};

enum lp8788_charger_event {
	NO_CHARGER,
	CHARGER_DETECTED,
};

enum lp8788_bl_ctrl_mode {
	LP8788_BL_REGISTER_ONLY,
	LP8788_BL_COMB_PWM_BASED,	
	LP8788_BL_COMB_REGISTER_BASED,	
};

enum lp8788_bl_dim_mode {
	LP8788_DIM_EXPONENTIAL,
	LP8788_DIM_LINEAR,
};

enum lp8788_bl_full_scale_current {
	LP8788_FULLSCALE_5000uA,
	LP8788_FULLSCALE_8500uA,
	LP8788_FULLSCALE_1200uA,
	LP8788_FULLSCALE_1550uA,
	LP8788_FULLSCALE_1900uA,
	LP8788_FULLSCALE_2250uA,
	LP8788_FULLSCALE_2600uA,
	LP8788_FULLSCALE_2950uA,
};

enum lp8788_bl_ramp_step {
	LP8788_RAMP_8us,
	LP8788_RAMP_1024us,
	LP8788_RAMP_2048us,
	LP8788_RAMP_4096us,
	LP8788_RAMP_8192us,
	LP8788_RAMP_16384us,
	LP8788_RAMP_32768us,
	LP8788_RAMP_65538us,
};

enum lp8788_isink_scale {
	LP8788_ISINK_SCALE_100mA,
	LP8788_ISINK_SCALE_120mA,
};

enum lp8788_isink_number {
	LP8788_ISINK_1,
	LP8788_ISINK_2,
	LP8788_ISINK_3,
};

enum lp8788_alarm_sel {
	LP8788_ALARM_1,
	LP8788_ALARM_2,
	LP8788_ALARM_MAX,
};

enum lp8788_adc_id {
	LPADC_VBATT_5P5,
	LPADC_VIN_CHG,
	LPADC_IBATT,
	LPADC_IC_TEMP,
	LPADC_VBATT_6P0,
	LPADC_VBATT_5P0,
	LPADC_ADC1,
	LPADC_ADC2,
	LPADC_VDD,
	LPADC_VCOIN,
	LPADC_VDD_LDO,
	LPADC_ADC3,
	LPADC_ADC4,
	LPADC_MAX,
};

struct lp8788;


struct lp8788_buck1_dvs {
	int gpio;
	enum lp8788_dvs_sel vsel;
};


struct lp8788_buck2_dvs {
	int gpio[LP8788_NUM_BUCK2_DVS];
	enum lp8788_dvs_sel vsel;
};


struct lp8788_ldo_enable_pin {
	int gpio;
	int init_state;
};


struct lp8788_chg_param {
	u8 addr;
	u8 val;
};


struct lp8788_charger_platform_data {
	const char *adc_vbatt;
	const char *adc_batt_temp;
	unsigned int max_vbatt_mv;
	struct lp8788_chg_param *chg_params;
	int num_chg_params;
	void (*charger_event) (struct lp8788 *lp,
				enum lp8788_charger_event event);
};


struct lp8788_backlight_platform_data {
	char *name;
	int initial_brightness;
	enum lp8788_bl_ctrl_mode bl_mode;
	enum lp8788_bl_dim_mode dim_mode;
	enum lp8788_bl_full_scale_current full_scale;
	enum lp8788_bl_ramp_step rise_time;
	enum lp8788_bl_ramp_step fall_time;
	enum pwm_polarity pwm_pol;
	unsigned int period_ns;
};


struct lp8788_led_platform_data {
	char *name;
	enum lp8788_isink_scale scale;
	enum lp8788_isink_number num;
	int iout_code;
};


struct lp8788_vib_platform_data {
	char *name;
	enum lp8788_isink_scale scale;
	enum lp8788_isink_number num;
	int iout_code;
	int pwm_code;
};


struct lp8788_platform_data {
	
	int (*init_func) (struct lp8788 *lp);

	
	struct regulator_init_data *buck_data[LP8788_NUM_BUCKS];
	struct regulator_init_data *dldo_data[LP8788_NUM_DLDOS];
	struct regulator_init_data *aldo_data[LP8788_NUM_ALDOS];
	struct lp8788_buck1_dvs *buck1_dvs;
	struct lp8788_buck2_dvs *buck2_dvs;
	struct lp8788_ldo_enable_pin *ldo_pin[EN_LDOS_MAX];

	
	struct lp8788_charger_platform_data *chg_pdata;

	
	enum lp8788_alarm_sel alarm_sel;

	
	struct lp8788_backlight_platform_data *bl_pdata;

	
	struct lp8788_led_platform_data *led_pdata;
	struct lp8788_vib_platform_data *vib_pdata;

	
	struct iio_map *adc_pdata;
};


struct lp8788 {
	struct device *dev;
	struct regmap *regmap;
	struct irq_domain *irqdm;
	int irq;
	struct lp8788_platform_data *pdata;
};

int lp8788_irq_init(struct lp8788 *lp, int chip_irq);
void lp8788_irq_exit(struct lp8788 *lp);
int lp8788_read_byte(struct lp8788 *lp, u8 reg, u8 *data);
int lp8788_read_multi_bytes(struct lp8788 *lp, u8 reg, u8 *data, size_t count);
int lp8788_write_byte(struct lp8788 *lp, u8 reg, u8 data);
int lp8788_update_bits(struct lp8788 *lp, u8 reg, u8 mask, u8 data);
#endif
