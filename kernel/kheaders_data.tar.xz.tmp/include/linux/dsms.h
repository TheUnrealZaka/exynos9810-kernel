

#ifndef _LINUX_DSMS_H
#define _LINUX_DSMS_H

#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/types.h>

#define DSMS_SUCCESS (0)
#define DSMS_DENY (-EPERM)
#define DSMS_NOT_IMPLEMENTED (-ENOSYS)

// DSMS Kernel Interface

#ifdef CONFIG_SECURITY_DSMS

extern int noinline dsms_send_message(const char *feature_code,
				      const char *detail, int64_t value);

#else

static inline int dsms_send_message(const char *feature_code,
				    const char *detail,
				    int64_t value)
{
	
	return DSMS_NOT_IMPLEMENTED;
}

#endif 

#endif 