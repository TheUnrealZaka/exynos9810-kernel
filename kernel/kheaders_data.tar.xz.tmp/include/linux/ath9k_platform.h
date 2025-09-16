

#ifndef _LINUX_ATH9K_PLATFORM_H
#define _LINUX_ATH9K_PLATFORM_H

#define ATH9K_PLAT_EEP_MAX_WORDS	2048

struct ath9k_platform_data {
	const char *eeprom_name;

	u16 eeprom_data[ATH9K_PLAT_EEP_MAX_WORDS];
	u8 *macaddr;

	int led_pin;
	u32 gpio_mask;
	u32 gpio_val;

	u32 bt_active_pin;
	u32 bt_priority_pin;
	u32 wlan_active_pin;

	bool endian_check;
	bool is_clk_25mhz;
	bool tx_gain_buffalo;
	bool disable_2ghz;
	bool disable_5ghz;
	bool led_active_high;

	int (*get_mac_revision)(void);
	int (*external_reset)(void);

	bool use_eeprom;
};

#endif 
