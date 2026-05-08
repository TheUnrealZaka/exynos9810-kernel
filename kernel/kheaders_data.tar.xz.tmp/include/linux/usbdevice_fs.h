




#ifndef _LINUX_USBDEVICE_FS_H
#define _LINUX_USBDEVICE_FS_H

#include <uapi/linux/usbdevice_fs.h>

#ifdef CONFIG_COMPAT
#include <linux/compat.h>

struct usbdevfs_ctrltransfer32 {
        u8 bRequestType;
        u8 bRequest;
        u16 wValue;
        u16 wIndex;
        u16 wLength;
        u32 timeout;  
        compat_caddr_t data;
};

struct usbdevfs_bulktransfer32 {
        compat_uint_t ep;
        compat_uint_t len;
        compat_uint_t timeout; 
        compat_caddr_t data;
};

struct usbdevfs_disconnectsignal32 {
        compat_int_t signr;
        compat_caddr_t context;
};

struct usbdevfs_urb32 {
	unsigned char type;
	unsigned char endpoint;
	compat_int_t status;
	compat_uint_t flags;
	compat_caddr_t buffer;
	compat_int_t buffer_length;
	compat_int_t actual_length;
	compat_int_t start_frame;
	compat_int_t number_of_packets;
	compat_int_t error_count;
	compat_uint_t signr;
	compat_caddr_t usercontext; 
	struct usbdevfs_iso_packet_desc iso_frame_desc[0];
};

struct usbdevfs_ioctl32 {
	s32 ifno;
	s32 ioctl_code;
	compat_caddr_t data;
};
#endif
#endif 
