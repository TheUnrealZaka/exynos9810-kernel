#ifndef _ADFS_FS_H
#define _ADFS_FS_H

#include <uapi/linux/adfs_fs.h>


static inline int adfs_checkbblk(unsigned char *ptr)
{
	unsigned int result = 0;
	unsigned char *p = ptr + 511;

	do {
	        result = (result & 0xff) + (result >> 8);
        	result = result + *--p;
	} while (p != ptr);

	return (result & 0xff) != ptr[511];
}
#endif
