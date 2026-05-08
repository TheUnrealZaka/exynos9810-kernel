
#ifndef __LV5207LP_H__
#define __LV5207LP_H__

struct device;

struct lv5207lp_platform_data {
	struct device *fbdev;
	unsigned int max_value;
	unsigned int def_value;
};

#endif
