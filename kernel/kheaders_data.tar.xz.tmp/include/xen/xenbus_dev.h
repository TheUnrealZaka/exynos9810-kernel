

#ifndef __LINUX_XEN_XENBUS_DEV_H__
#define __LINUX_XEN_XENBUS_DEV_H__

#include <linux/ioctl.h>

#define IOCTL_XENBUS_BACKEND_EVTCHN			\
	_IOC(_IOC_NONE, 'B', 0, 0)

#define IOCTL_XENBUS_BACKEND_SETUP			\
	_IOC(_IOC_NONE, 'B', 1, 0)

#endif 
