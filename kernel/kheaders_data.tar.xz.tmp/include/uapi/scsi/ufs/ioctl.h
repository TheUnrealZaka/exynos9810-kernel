#ifndef UAPI_UFS_IOCTL_H_
#define UAPI_UFS_IOCTL_H_

#include <linux/types.h>


#define UFS_IOCTL_QUERY			0x5388


struct ufs_ioctl_query_data {
	
	__u32 opcode;
	
	__u8 idn;
	
	__u16 buf_size;
	
	__u8 buffer[0];
};

#endif 
