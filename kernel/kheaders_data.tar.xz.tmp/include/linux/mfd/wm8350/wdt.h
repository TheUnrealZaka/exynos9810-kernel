

#ifndef __LINUX_MFD_WM8350_WDT_H_
#define __LINUX_MFD_WM8350_WDT_H_

#include <linux/platform_device.h>

#define WM8350_WDOG_HIB_MODE                    0x0080
#define WM8350_WDOG_DEBUG                       0x0040
#define WM8350_WDOG_MODE_MASK                   0x0030
#define WM8350_WDOG_TO_MASK                     0x0007

#define WM8350_IRQ_SYS_WDOG_TO			24

struct wm8350_wdt {
	struct platform_device *pdev;
};

#endif
