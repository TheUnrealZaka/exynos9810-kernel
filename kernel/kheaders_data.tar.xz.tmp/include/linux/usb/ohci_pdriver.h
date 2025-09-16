

#ifndef __USB_CORE_OHCI_PDRIVER_H
#define __USB_CORE_OHCI_PDRIVER_H


struct usb_ohci_pdata {
	unsigned	big_endian_desc:1;
	unsigned	big_endian_mmio:1;
	unsigned	no_big_frame_no:1;
	unsigned int	num_ports;

	
	int (*power_on)(struct platform_device *pdev);
	
	void (*power_off)(struct platform_device *pdev);
	
	void (*power_suspend)(struct platform_device *pdev);
};

#endif 
