
#ifndef _XT_CHECKSUM_TARGET_H
#define _XT_CHECKSUM_TARGET_H

#include <linux/types.h>

#define XT_CHECKSUM_OP_FILL	0x01	

struct xt_CHECKSUM_info {
	__u8 operation;	
};

#endif 
