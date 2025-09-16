/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _VERSION_COMPAT_DEFS_H_
#define _VERSION_COMPAT_DEFS_H_

#include <linux/version.h>

#if KERNEL_VERSION(4, 16, 0) >= LINUX_VERSION_CODE
typedef unsigned int __poll_t;
#endif

#endif 
