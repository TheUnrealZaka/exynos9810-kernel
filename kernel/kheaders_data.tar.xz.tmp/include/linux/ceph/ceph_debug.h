#ifndef _FS_CEPH_DEBUG_H
#define _FS_CEPH_DEBUG_H

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/string.h>

#ifdef CONFIG_CEPH_LIB_PRETTYDEBUG



# if defined(DEBUG) || defined(CONFIG_DYNAMIC_DEBUG)
#  define dout(fmt, ...)						\
	pr_debug("%.*s %12.12s:%-4d : " fmt,				\
		 8 - (int)sizeof(KBUILD_MODNAME), "    ",		\
		 kbasename(__FILE__), __LINE__, ##__VA_ARGS__)
# else

#  define dout(fmt, ...)	do {				\
		if (0)						\
			printk(KERN_DEBUG fmt, ##__VA_ARGS__);	\
	} while (0)
# endif

#else


# define dout(fmt, ...)	pr_debug(" " fmt, ##__VA_ARGS__)

#endif

#endif
