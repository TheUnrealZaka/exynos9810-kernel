

#ifndef _LINUX_PROCA_FCNTL_H
#define _LINUX_PROCA_FCNTL_H

#include <linux/file.h>

#if defined(CONFIG_FIVE_PA_FEATURE) || defined(CONFIG_PROCA)
int proca_fcntl_setxattr(struct file *file, void __user *lv_xattr);
#else
static inline int proca_fcntl_setxattr(struct file *file, void __user *lv_xattr)
{
	return 0;
}
#endif

#endif 
