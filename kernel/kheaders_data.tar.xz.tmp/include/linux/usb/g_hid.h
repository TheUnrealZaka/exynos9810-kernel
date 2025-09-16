

#ifndef __LINUX_USB_G_HID_H
#define __LINUX_USB_G_HID_H

struct hidg_func_descriptor {
	unsigned char		subclass;
	unsigned char		protocol;
	unsigned short		report_length;
	unsigned short		report_desc_length;
	unsigned char		report_desc[];
};

#endif 
