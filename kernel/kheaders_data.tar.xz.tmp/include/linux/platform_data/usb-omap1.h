
#ifndef __LINUX_USB_OMAP1_H
#define __LINUX_USB_OMAP1_H

#include <linux/platform_device.h>

struct omap_usb_config {
	
	unsigned	register_host:1;
	unsigned	register_dev:1;
	u8		otg;	

	const char	*extcon;	

	u8		hmc_mode;

	
	u8		rwc;

	
	u8		pins[3];

	struct platform_device *udc_device;
	struct platform_device *ohci_device;
	struct platform_device *otg_device;

	u32 (*usb0_init)(unsigned nwires, unsigned is_device);
	u32 (*usb1_init)(unsigned nwires);
	u32 (*usb2_init)(unsigned nwires, unsigned alt_pingroup);

	int (*ocpi_enable)(void);
};

#endif 
