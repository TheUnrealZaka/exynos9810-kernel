

#ifndef __IIO_ADC_AD7266_H__
#define __IIO_ADC_AD7266_H__


enum ad7266_range {
	AD7266_RANGE_VREF,
	AD7266_RANGE_2VREF,
};


enum ad7266_mode {
	AD7266_MODE_DIFF,
	AD7266_MODE_PSEUDO_DIFF,
	AD7266_MODE_SINGLE_ENDED,
};


struct ad7266_platform_data {
	enum ad7266_range range;
	enum ad7266_mode mode;
	bool fixed_addr;
	unsigned int addr_gpios[3];
};

#endif
