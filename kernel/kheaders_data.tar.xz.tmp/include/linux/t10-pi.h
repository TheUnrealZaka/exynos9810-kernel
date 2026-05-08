#ifndef _LINUX_T10_PI_H
#define _LINUX_T10_PI_H

#include <linux/types.h>
#include <linux/blkdev.h>


enum t10_dif_type {
	T10_PI_TYPE0_PROTECTION = 0x0,
	T10_PI_TYPE1_PROTECTION = 0x1,
	T10_PI_TYPE2_PROTECTION = 0x2,
	T10_PI_TYPE3_PROTECTION = 0x3,
};


struct t10_pi_tuple {
	__be16 guard_tag;	
	__be16 app_tag;		
	__be32 ref_tag;		
};


extern struct blk_integrity_profile t10_pi_type1_crc;
extern struct blk_integrity_profile t10_pi_type1_ip;
extern struct blk_integrity_profile t10_pi_type3_crc;
extern struct blk_integrity_profile t10_pi_type3_ip;

#endif
