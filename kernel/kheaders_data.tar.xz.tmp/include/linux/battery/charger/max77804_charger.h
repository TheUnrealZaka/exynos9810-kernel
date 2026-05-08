
#ifndef __MAX77804_CHARGER_H
#define __MAX77804_CHARGER_H __FILE__
#include <linux/mfd/core.h>
#include <linux/mfd/max77804.h>
#include <linux/mfd/max77804-private.h>
#include <linux/regulator/machine.h>

#if defined(CONFIG_V1A) || defined(CONFIG_N1A)
#define max77888_charger
#else
#undef max77888_charger
#endif

#define ENABLE 1
#define DISABLE 0


#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define ABS(x)		((x) < 0 ? (-1 * (x)) : (x))

#define OFF_CURR	0	
#define KEEP_CURR	-1	


#define CHG_CNFG_04_CHG_CV_PRM_SHIFT            0
#define CHG_CNFG_04_CHG_CV_PRM_MASK             (0x1f << CHG_CNFG_04_CHG_CV_PRM_SHIFT)


#define CHG_CNFG_12_CHGINSEL_SHIFT              5
#define CHG_CNFG_12_CHGINSEL_MASK               (0x1 << CHG_CNFG_12_CHGINSEL_SHIFT)


#define MAX77804_BYP_I                  (1 << 0)
#define MAX77804_BATP_I			(1 << 2)
#define MAX77804_BAT_I                  (1 << 3)
#define MAX77804_CHG_I                  (1 << 4)
#define MAX77804_WCIN_I			(1 << 5)
#define MAX77804_CHGIN_I                (1 << 6)


#define MAX77804_BYP_IM                 (1 << 0)
#define MAX77804_THM_IM                 (1 << 2)
#define MAX77804_BAT_IM                 (1 << 3)
#define MAX77804_CHG_IM                 (1 << 4)
#define MAX77804_WCIN_IM		(1 << 5)
#define MAX77804_CHGIN_IM               (1 << 6)


#define MAX77804_BYP_OK                 0x01
#define MAX77804_BYP_OK_SHIFT           0
#define MAX77804_BATP_OK		0x04
#define MAX77804_BATP_OK_SHIFT		2
#define MAX77804_BAT_OK                 0x08
#define MAX77804_BAT_OK_SHIFT           3
#define MAX77804_CHG_OK                 0x10
#define MAX77804_CHG_OK_SHIFT           4
#define MAX77804_WCIN_OK		0x20
#define MAX77804_WCIN_OK_SHIFT		5
#define MAX77804_CHGIN_OK               0x40
#define MAX77804_CHGIN_OK_SHIFT         6
#define MAX77804_DETBAT                 0x04
#define MAX77804_DETBAT_SHIFT           2


#define MAX77804_BATP_DTLS		0x01
#define MAX77804_BATP_DTLS_SHIFT	0
#define MAX77804_WCIN_DTLS		0x18
#define MAX77804_WCIN_DTLS_SHIFT	3
#define MAX77804_CHGIN_DTLS             0x60
#define MAX77804_CHGIN_DTLS_SHIFT       5


#define MAX77804_CHG_DTLS               0x0F
#define MAX77804_CHG_DTLS_SHIFT         0
#define MAX77804_BAT_DTLS               0x70
#define MAX77804_BAT_DTLS_SHIFT         4


#define MAX77804_BYP_DTLS               0x0F
#define MAX77804_BYP_DTLS_SHIFT         0
#define MAX77804_BYP_DTLS0      0x1
#define MAX77804_BYP_DTLS1      0x2
#define MAX77804_BYP_DTLS2      0x4
#define MAX77804_BYP_DTLS3      0x8


#define MAX77804_MODE_DEFAULT   0x04
#define MAX77804_MODE_CHGR      0x01
#define MAX77804_MODE_OTG       0x02
#define MAX77804_MODE_BUCK      0x04
#define MAX77804_MODE_BOOST		0x08


#define MAX77804_CHG_CC         0x3F


#define MAX77804_CHG_TO_ITH		0x07


#define MAX77804_CHG_MINVSYS_MASK       0xE0
#define MAX77804_CHG_MINVSYS_SHIFT      5
#define MAX77804_CHG_PRM_MASK           0x1F
#define MAX77804_CHG_PRM_SHIFT          0


#define MAX77804_CHG_CHGIN_LIM  0x7F


#define MAX77804_CHG_WCINSEL		0x40


#define MAX77804_CHGTYPMAN              0x02
#define MAX77804_CHGTYPMAN_SHIFT        1


#define MAX77804_VBVOLT                 0x40
#define MAX77804_VBVOLT_SHIFT           6
#define MAX77804_CHGDETRUN              0x08
#define MAX77804_CHGDETRUN_SHIFT        3
#define MAX77804_CHGTYPE                0x07
#define MAX77804_CHGTYPE_SHIFT          0


#define IRQ_DEBOUNCE_TIME       20      


#define DET_ERR_RETRY   5
#define DET_ERR_DELAY   200


#define SOFT_CHG_START_CURR     100     
#define SOFT_CHG_START_DUR      100     
#define SOFT_CHG_CURR_STEP      100     
#define SOFT_CHG_STEP_DUR       20      

#define DEFAULT_AC_CURRENT	1600	
#define DEFAULT_USB_CURRENT	500	

enum {
	POWER_SUPPLY_VBUS_UNKNOWN = 0,
	POWER_SUPPLY_VBUS_UVLO,
	POWER_SUPPLY_VBUS_WEAK,
	POWER_SUPPLY_VBUS_OVLO,
	POWER_SUPPLY_VBUS_GOOD,
};

extern sec_battery_platform_data_t sec_battery_pdata;

#if defined(CONFIG_TARGET_LOCALE_KOR)
#define SEC_CHG_ATTR(_name)				\
{							\
	.attr = {.name = #_name, .mode = 0664},	\
	.show = sec_chg_show_attrs,			\
	.store = sec_chg_store_attrs,			\
}

enum {
	CHG_SIOP_INPUT_LIMIT = 0,
};
#endif

#endif
