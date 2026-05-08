#ifndef _UAPI_LINUX_KDEV_T_H
#define _UAPI_LINUX_KDEV_T_H
#ifndef __KERNEL__


#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))
#endif 
#endif 
