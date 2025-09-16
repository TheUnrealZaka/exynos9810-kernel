
#ifndef __BD6107_H__
#define __BD6107_H__

struct device;

struct bd6107_platform_data {
	struct device *fbdev;
	int reset;			
	unsigned int def_value;
};

#endif
