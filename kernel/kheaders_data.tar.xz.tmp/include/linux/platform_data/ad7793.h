
#ifndef __LINUX_PLATFORM_DATA_AD7793_H__
#define __LINUX_PLATFORM_DATA_AD7793_H__


enum ad7793_clock_source {
	AD7793_CLK_SRC_INT,
	AD7793_CLK_SRC_INT_CO,
	AD7793_CLK_SRC_EXT,
	AD7793_CLK_SRC_EXT_DIV2,
};


enum ad7793_bias_voltage {
	AD7793_BIAS_VOLTAGE_DISABLED,
	AD7793_BIAS_VOLTAGE_AIN1,
	AD7793_BIAS_VOLTAGE_AIN2,
	AD7793_BIAS_VOLTAGE_AIN3,
};


enum ad7793_refsel {
	AD7793_REFSEL_REFIN1 = 0,
	AD7793_REFSEL_REFIN2 = 1,
	AD7793_REFSEL_INTERNAL = 2,
};


enum ad7793_current_source_direction {
	AD7793_IEXEC1_IOUT1_IEXEC2_IOUT2 = 0,
	AD7793_IEXEC1_IOUT2_IEXEC2_IOUT1 = 1,
	AD7793_IEXEC1_IEXEC2_IOUT1 = 2,
	AD7793_IEXEC1_IEXEC2_IOUT2 = 3,
};


enum ad7793_excitation_current {
	AD7793_IX_DISABLED = 0,
	AD7793_IX_10uA = 1,
	AD7793_IX_210uA = 2,
	AD7793_IX_1mA = 3,
};


struct ad7793_platform_data {
	enum ad7793_clock_source clock_src;
	bool burnout_current;
	bool boost_enable;
	bool buffered;
	bool unipolar;

	enum ad7793_refsel refsel;
	enum ad7793_bias_voltage bias_voltage;
	enum ad7793_excitation_current exitation_current;
	enum ad7793_current_source_direction current_source_direction;
};

#endif 
