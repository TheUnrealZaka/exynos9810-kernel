

#ifndef _UX500_CHARGALG_H
#define _UX500_CHARGALG_H

#include <linux/power_supply.h>


#define psy_to_ux500_charger(x) power_supply_get_drvdata(x)


struct ux500_charger;

struct ux500_charger_ops {
	int (*enable) (struct ux500_charger *, int, int, int);
	int (*check_enable) (struct ux500_charger *, int, int);
	int (*kick_wd) (struct ux500_charger *);
	int (*update_curr) (struct ux500_charger *, int);
	int (*pp_enable) (struct ux500_charger *, bool);
	int (*pre_chg_enable) (struct ux500_charger *, bool);
};


struct ux500_charger {
	struct power_supply *psy;
	struct ux500_charger_ops ops;
	int max_out_volt;
	int max_out_curr;
	int wdt_refresh;
	bool enabled;
	bool external;
	bool power_path;
};

extern struct blocking_notifier_head charger_notifier_list;

#endif
