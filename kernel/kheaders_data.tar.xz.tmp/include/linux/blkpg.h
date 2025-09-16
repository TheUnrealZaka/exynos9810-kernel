#ifndef _LINUX_BLKPG_H
#define _LINUX_BLKPG_H



#include <linux/compat.h>
#include <uapi/linux/blkpg.h>

#ifdef CONFIG_COMPAT

struct blkpg_compat_ioctl_arg {
	compat_int_t op;
	compat_int_t flags;
	compat_int_t datalen;
	compat_uptr_t data;
};
#endif

#endif 
