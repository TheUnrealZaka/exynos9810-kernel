
#ifndef _UAPI__LINUX_UIO_H
#define _UAPI__LINUX_UIO_H

#include <linux/compiler.h>
#include <linux/types.h>


struct iovec
{
	void __user *iov_base;	
	__kernel_size_t iov_len; 
};


 
#define UIO_FASTIOV	8
#define UIO_MAXIOV	1024


#endif 
