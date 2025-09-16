

#ifndef __USB_CORE_EHCI_PDRIVER_H
#define __USB_CORE_EHCI_PDRIVER_H

struct platform_device;
struct usb_hcd;


struct usb_ehci_pdata {
	int		caps_offset;
	unsigned	has_tt:1;
	unsigned	has_synopsys_hc_bug:1;
	unsigned	big_endian_desc:1;
	unsigned	big_endian_mmio:1;
	unsigned	no_io_watchdog:1;
	unsigned	reset_on_resume:1;
	unsigned	dma_mask_64:1;

	
	int (*power_on)(struct platform_device *pdev);
	
	void (*power_off)(struct platform_device *pdev);
	
	void (*power_suspend)(struct platform_device *pdev);
	int (*pre_setup)(struct usb_hcd *hcd);
};

#endif 
