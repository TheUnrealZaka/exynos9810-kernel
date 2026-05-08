#ifndef _ASM_GENERIC_IOCTL_H
#define _ASM_GENERIC_IOCTL_H

#include <uapi/asm-generic/ioctl.h>

#ifdef __CHECKER__
#define _IOC_TYPECHECK(t) (sizeof(t))
#else

extern unsigned int __invalid_size_argument_for_IOC;
#define _IOC_TYPECHECK(t) \
	((sizeof(t) == sizeof(t[1]) && \
	  sizeof(t) < (1 << _IOC_SIZEBITS)) ? \
	  sizeof(t) : __invalid_size_argument_for_IOC)
#endif

#endif 
