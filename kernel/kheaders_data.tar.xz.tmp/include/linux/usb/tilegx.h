

#ifndef _LINUX_USB_TILEGX_H
#define _LINUX_USB_TILEGX_H

#include <gxio/usb_host.h>

struct tilegx_usb_platform_data {
	
	int dev_index;

	
	gxio_usb_host_context_t usb_ctx;

	
	unsigned int irq;
};

#endif 
