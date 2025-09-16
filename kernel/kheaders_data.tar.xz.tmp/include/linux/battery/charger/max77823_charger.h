

#ifndef __MAX77823_CHARGER_H
#define __MAX77823_CHARGER_H __FILE__

#include <linux/mfd/core.h>
#include <linux/mfd/max77823.h>
#include <linux/mfd/max77823-private.h>
#include <linux/regulator/machine.h>

#define MAX77823_SAFEOUT2                0x80


#define MAX77823_BYP_I                  (1 << 0)
#define MAX77823_BATP_I			(1 << 2)
#define MAX77823_BAT_I                  (1 << 3)
#define MAX77823_CHG_I                  (1 << 4)
#define MAX77823_WCIN_I			(1 << 5)
#define MAX77823_CHGIN_I                (1 << 6)


#define MAX77823_BYP_IM                 (1 << 0)
#define MAX77823_THM_IM                 (1 << 2)
#define MAX77823_BAT_IM                 (1 << 3)
#define MAX77823_CHG_IM                 (1 << 4)
#define MAX77823_WCIN_IM		(1 << 5)
#define MAX77823_CHGIN_IM               (1 << 6)


#define MAX77823_BYP_OK                 0x01
#define MAX77823_BYP_OK_SHIFT           0
#define MAX77823_BATP_OK		0x04
#define MAX77823_BATP_OK_SHIFT		2
#define MAX77823_BAT_OK                 0x08
#define MAX77823_BAT_OK_SHIFT           3
#define MAX77823_CHG_OK                 0x10
#define MAX77823_CHG_OK_SHIFT           4
#define MAX77823_WCIN_OK		0x20
#define MAX77823_WCIN_OK_SHIFT		5
#define MAX77823_CHGIN_OK               0x40
#define MAX77823_CHGIN_OK_SHIFT         6
#define MAX77823_DETBAT                 0x04
#define MAX77823_DETBAT_SHIFT           2


#define MAX77823_BATP_DTLS		0x01
#define MAX77823_BATP_DTLS_SHIFT	0
#define MAX77823_WCIN_DTLS		0x18
#define MAX77823_WCIN_DTLS_SHIFT	3
#define MAX77823_CHGIN_DTLS             0x60
#define MAX77823_CHGIN_DTLS_SHIFT       5


#define MAX77823_CHG_DTLS               0x0F
#define MAX77823_CHG_DTLS_SHIFT         0
#define MAX77823_BAT_DTLS               0x70
#define MAX77823_BAT_DTLS_SHIFT         4


#define MAX77823_BYP_DTLS               0x0F
#define MAX77823_BYP_DTLS_SHIFT         0
#define MAX77823_BYP_DTLS0      0x1
#define MAX77823_BYP_DTLS1      0x2
#define MAX77823_BYP_DTLS2      0x4
#define MAX77823_BYP_DTLS3      0x8


#define CHG_CNFG_00_MODE_SHIFT		        0
#define CHG_CNFG_00_CHG_SHIFT		        0
#define CHG_CNFG_00_OTG_SHIFT		        1
#define CHG_CNFG_00_BUCK_SHIFT		        2
#define CHG_CNFG_00_BOOST_SHIFT		        3
#define CHG_CNFG_00_DIS_MUIC_CTRL_SHIFT	        5
#define CHG_CNFG_00_MODE_MASK		        (0xf << CHG_CNFG_00_MODE_SHIFT)
#define CHG_CNFG_00_CHG_MASK		        (1 << CHG_CNFG_00_CHG_SHIFT)
#define CHG_CNFG_00_OTG_MASK		        (1 << CHG_CNFG_00_OTG_SHIFT)
#define CHG_CNFG_00_BUCK_MASK		        (1 << CHG_CNFG_00_BUCK_SHIFT)
#define CHG_CNFG_00_BOOST_MASK		        (1 << CHG_CNFG_00_BOOST_SHIFT)
#define CHG_CNFG_00_DIS_MUIC_CTRL_MASK	        (1 << CHG_CNFG_00_DIS_MUIC_CTRL_SHIFT)
#define MAX77823_MODE_DEFAULT                   0x04
#define MAX77823_MODE_CHGR                      0x01
#define MAX77823_MODE_OTG                       0x02
#define MAX77823_MODE_BUCK                      0x04
#define MAX77823_MODE_BOOST		        0x08


#define MAX77823_CHG_CC                         0x3F


#define MAX77823_CHG_TO_ITH		        0x07


#define MAX77823_CHG_MINVSYS_MASK               0xE0
#define MAX77823_CHG_MINVSYS_SHIFT              5
#define MAX77823_CHG_PRM_MASK                   0x1F
#define MAX77823_CHG_PRM_SHIFT                  0

#define CHG_CNFG_04_CHG_CV_PRM_SHIFT            0
#define CHG_CNFG_04_CHG_CV_PRM_MASK             (0x3F << CHG_CNFG_04_CHG_CV_PRM_SHIFT)


#define MAX77823_CHG_CHGIN_LIM                  0x7F


#define MAX77823_CHG_WCINSEL		        0x40


#define MAX77823_BATP_DTLS		        0x01
#define MAX77823_BATP_DTLS_SHIFT	        0
#define MAX77823_WCIN_DTLS		        0x18
#define MAX77823_WCIN_DTLS_SHIFT	        3
#define MAX77823_CHGIN_DTLS                     0x60
#define MAX77823_CHGIN_DTLS_SHIFT               5


#define MAX77823_CHG_DTLS                       0x0F
#define MAX77823_CHG_DTLS_SHIFT                 0
#define MAX77823_BAT_DTLS                       0x70
#define MAX77823_BAT_DTLS_SHIFT                 4

#define SIOP_INPUT_LIMIT_CURRENT                1200
#define SIOP_CHARGING_LIMIT_CURRENT             1000
#define SLOW_CHARGING_CURRENT_STANDARD          400

struct max77823_charger_data {
	struct device           *dev;
	struct i2c_client       *i2c;
	struct i2c_client       *pmic_i2c;
	struct mutex            charger_mutex;

	struct max77823_platform_data *max77823_pdata;

	struct power_supply	psy_chg;

	struct workqueue_struct *wqueue;
	struct work_struct	chgin_work;
	struct delayed_work	isr_work;
	struct delayed_work	recovery_work;	
	struct delayed_work	wpc_work;	
	struct delayed_work	chgin_init_work;	


	struct mutex irq_lock;
	struct mutex ops_lock;

	
	struct wake_lock recovery_wake_lock;
	struct wake_lock wpc_wake_lock;
	struct wake_lock chgin_wake_lock;

	unsigned int	is_charging;
	unsigned int	charging_type;
	unsigned int	battery_state;
	unsigned int	battery_present;
	unsigned int	cable_type;
	unsigned int	charging_current_max;
	unsigned int	charging_current;
	unsigned int	input_current_limit;
	unsigned int	vbus_state;
	int		aicl_on;
	int		status;
	int		siop_level;
	int uvlo_attach_flag;
	int uvlo_attach_cable_type;

	int		irq_bypass;
	int		irq_batp;

	int		irq_battery;
	int		irq_chg;
	int		irq_wcin;
	int		irq_chgin;

	
	bool		soft_reg_state;
	int		soft_reg_current;

	
	bool		reg_loop_deted;

	
	int		wc_w_gpio;
	int		wc_w_irq;
	int		wc_w_state;
	int		wc_v_gpio;
	int		wc_v_irq;
	int		wc_v_state;
	bool		wc_pwr_det;
	int		soft_reg_recovery_cnt;

	int pmic_ver;
	int input_curr_limit_step;
	int wpc_input_curr_limit_step;
	int charging_curr_step;

	sec_battery_platform_data_t	*pdata;
};

extern void cp_usb_power_control(int enable);
#endif 
