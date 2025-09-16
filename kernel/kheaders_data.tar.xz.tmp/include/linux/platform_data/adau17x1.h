

#ifndef __LINUX_PLATFORM_DATA_ADAU17X1_H__
#define __LINUX_PLATFORM_DATA_ADAU17X1_H__


enum adau17x1_micbias_voltage {
	ADAU17X1_MICBIAS_0_90_AVDD = 0,
	ADAU17X1_MICBIAS_0_65_AVDD = 1,
};


enum adau1761_digmic_jackdet_pin_mode {
	ADAU1761_DIGMIC_JACKDET_PIN_MODE_NONE,
	ADAU1761_DIGMIC_JACKDET_PIN_MODE_DIGMIC,
	ADAU1761_DIGMIC_JACKDET_PIN_MODE_JACKDETECT,
};


enum adau1761_jackdetect_debounce_time {
	ADAU1761_JACKDETECT_DEBOUNCE_5MS = 0,
	ADAU1761_JACKDETECT_DEBOUNCE_10MS = 1,
	ADAU1761_JACKDETECT_DEBOUNCE_20MS = 2,
	ADAU1761_JACKDETECT_DEBOUNCE_40MS = 3,
};


enum adau1761_output_mode {
	ADAU1761_OUTPUT_MODE_HEADPHONE,
	ADAU1761_OUTPUT_MODE_HEADPHONE_CAPLESS,
	ADAU1761_OUTPUT_MODE_LINE,
};


struct adau1761_platform_data {
	bool input_differential;
	enum adau1761_output_mode lineout_mode;
	enum adau1761_output_mode headphone_mode;

	enum adau1761_digmic_jackdet_pin_mode digmic_jackdetect_pin_mode;

	enum adau1761_jackdetect_debounce_time jackdetect_debounce_time;
	bool jackdetect_active_low;

	enum adau17x1_micbias_voltage micbias_voltage;
};


struct adau1781_platform_data {
	bool left_input_differential;
	bool right_input_differential;

	bool use_dmic;

	enum adau17x1_micbias_voltage micbias_voltage;
};

#endif
