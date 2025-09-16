

#ifndef _LINUX_PLATFORM_DATA_WKUP_M3_H
#define _LINUX_PLATFORM_DATA_WKUP_M3_H

struct platform_device;

struct wkup_m3_platform_data {
	const char *reset_name;

	int (*assert_reset)(struct platform_device *pdev, const char *name);
	int (*deassert_reset)(struct platform_device *pdev, const char *name);
};

#endif 
