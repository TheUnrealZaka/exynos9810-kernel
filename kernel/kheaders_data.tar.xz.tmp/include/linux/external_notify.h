

  

#ifndef __EXTERNAL_NOTIFY_H__
#define __EXTERNAL_NOTIFY_H__

#include <linux/notifier.h>


enum external_notify_cmd {
	EXTERNAL_NOTIFY_3S_NODEVICE = 1,
	EXTERNAL_NOTIFY_DEVICE_CONNECT,
	EXTERNAL_NOTIFY_HOSTBLOCK_PRE,
	EXTERNAL_NOTIFY_HOSTBLOCK_POST,
	EXTERNAL_NOTIFY_MDMBLOCK_PRE,
	EXTERNAL_NOTIFY_MDMBLOCK_POST,
	EXTERNAL_NOTIFY_POWERROLE,
	EXTERNAL_NOTIFY_DEVICEADD,
};


enum external_notify_device {
	EXTERNAL_NOTIFY_DEV_MUIC,
	EXTERNAL_NOTIFY_DEV_CHARGER,
	EXTERNAL_NOTIFY_DEV_PDIC,
	EXTERNAL_NOTIFY_DEV_MANAGER,
};

enum external_notify_condev {
	EXTERNAL_NOTIFY_NONE = 0,
	EXTERNAL_NOTIFY_GPAD,
	EXTERNAL_NOTIFY_LANHUB,
};

#ifdef CONFIG_USB_EXTERNAL_NOTIFY
extern int send_external_notify(unsigned long cmd, int data);
extern int usb_external_notify_register(struct notifier_block *nb,
		notifier_fn_t notifier, int listener);
extern int usb_external_notify_unregister(struct notifier_block *nb);
#else
static inline int send_external_notify(unsigned long cmd,
			int data) {return 0; }
static inline int usb_external_notify_register(struct notifier_block *nb,
			notifier_fn_t notifier, int listener) {return 0; }
static inline int usb_external_notify_unregister(struct notifier_block *nb)
			{return 0; }
#endif

#endif 
