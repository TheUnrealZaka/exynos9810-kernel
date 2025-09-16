

#ifndef __ASM_ARCH_NAND_H
#define __ASM_ARCH_NAND_H

#include <linux/mtd/partitions.h>

struct mxc_nand_platform_data {
	unsigned int width;	
	unsigned int hw_ecc:1;	
	unsigned int flash_bbt:1; 
	struct mtd_partition *parts;	
	int nr_parts;			
};
#endif 
