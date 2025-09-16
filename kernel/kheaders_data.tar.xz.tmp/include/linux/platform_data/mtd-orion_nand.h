

#ifndef __MTD_ORION_NAND_H
#define __MTD_ORION_NAND_H


struct orion_nand_data {
	struct mtd_partition *parts;
	int (*dev_ready)(struct mtd_info *mtd);
	u32 nr_parts;
	u8 ale;		
	u8 cle;		
	u8 width;	
	u8 chip_delay;
};


#endif
