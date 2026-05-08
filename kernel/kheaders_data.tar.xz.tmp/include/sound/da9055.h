

#ifndef __SOUND_DA9055_H__
#define __SOUND_DA9055_H__

enum da9055_micbias_voltage {
	DA9055_MICBIAS_1_6V = 0,
	DA9055_MICBIAS_1_8V = 1,
	DA9055_MICBIAS_2_1V = 2,
	DA9055_MICBIAS_2_2V = 3,
};

struct da9055_platform_data {
	
	bool micbias_source;
	
	enum da9055_micbias_voltage micbias;
};

#endif
