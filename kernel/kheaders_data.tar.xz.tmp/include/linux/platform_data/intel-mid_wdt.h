

#ifndef __INTEL_MID_WDT_H__
#define __INTEL_MID_WDT_H__

#include <linux/platform_device.h>

struct intel_mid_wdt_pdata {
	int irq;
	int (*probe)(struct platform_device *pdev);
};

#endif 
