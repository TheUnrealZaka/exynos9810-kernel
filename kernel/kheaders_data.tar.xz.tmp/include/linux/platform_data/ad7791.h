#ifndef __LINUX_PLATFORM_DATA_AD7791__
#define __LINUX_PLATFORM_DATA_AD7791__


struct ad7791_platform_data {
	bool buffered;
	bool burnout_current;
	bool unipolar;
};

#endif
