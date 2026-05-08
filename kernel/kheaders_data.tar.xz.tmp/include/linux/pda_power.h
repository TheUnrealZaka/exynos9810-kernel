

#ifndef __PDA_POWER_H__
#define __PDA_POWER_H__

#define PDA_POWER_CHARGE_AC  (1 << 0)
#define PDA_POWER_CHARGE_USB (1 << 1)

struct device;

struct pda_power_pdata {
	int (*init)(struct device *dev);
	int (*is_ac_online)(void);
	int (*is_usb_online)(void);
	void (*set_charge)(int flags);
	void (*exit)(struct device *dev);
	int (*suspend)(pm_message_t state);
	int (*resume)(void);

	char **supplied_to;
	size_t num_supplicants;

	unsigned int wait_for_status; 
	unsigned int wait_for_charger; 
	unsigned int polling_interval; 

	unsigned long ac_max_uA; 

	bool use_otg_notifier;
};

#endif 
