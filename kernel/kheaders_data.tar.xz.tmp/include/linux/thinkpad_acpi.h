#ifndef __THINKPAD_ACPI_H__
#define __THINKPAD_ACPI_H__



enum {
	TPACPI_LED_MUTE,
	TPACPI_LED_MICMUTE,
	TPACPI_LED_MAX,
};

int tpacpi_led_set(int whichled, bool on);

#endif
