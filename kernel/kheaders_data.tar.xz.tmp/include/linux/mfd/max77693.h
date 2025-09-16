

#ifndef __LINUX_MFD_MAX77693_H
#define __LINUX_MFD_MAX77693_H


enum max77693_regulators {
	MAX77693_ESAFEOUT1 = 0,
	MAX77693_ESAFEOUT2,
	MAX77693_CHARGER,
	MAX77693_REG_MAX,
};

struct max77693_reg_data {
	u8 addr;
	u8 data;
};

struct max77693_muic_platform_data {
	struct max77693_reg_data *init_data;
	int num_init_data;

	int detcable_delay_ms;

	
	int path_usb;
	int path_uart;
};




enum max77693_led_trigger {
	MAX77693_LED_TRIG_OFF,
	MAX77693_LED_TRIG_FLASH,
	MAX77693_LED_TRIG_TORCH,
	MAX77693_LED_TRIG_EXT,
	MAX77693_LED_TRIG_SOFT,
};


enum max77693_led_trigger_type {
	MAX77693_LED_TRIG_TYPE_EDGE,
	MAX77693_LED_TRIG_TYPE_LEVEL,
};


enum max77693_led_boost_mode {
	MAX77693_LED_BOOST_NONE,
	MAX77693_LED_BOOST_ADAPTIVE,
	MAX77693_LED_BOOST_FIXED,
};



struct max77693_platform_data {
	
	struct max77693_muic_platform_data *muic_data;
	struct max77693_led_platform_data *led_data;
};
#endif	
