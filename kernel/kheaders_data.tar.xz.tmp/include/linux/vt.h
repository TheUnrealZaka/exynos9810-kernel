#ifndef _LINUX_VT_H
#define _LINUX_VT_H

#include <uapi/linux/vt.h>



#define VT_ALLOCATE		0x0001 
#define VT_DEALLOCATE		0x0002 
#define VT_WRITE		0x0003 
#define VT_UPDATE		0x0004 
#define VT_PREWRITE		0x0005 

#ifdef CONFIG_VT_CONSOLE

extern int vt_kmsg_redirect(int new);

#else

static inline int vt_kmsg_redirect(int new)
{
	return 0;
}

#endif

#endif 
