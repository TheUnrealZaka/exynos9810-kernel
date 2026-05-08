#ifndef __ASM_ARCH_PXA3XX_NAND_H
#define __ASM_ARCH_PXA3XX_NAND_H

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>




#define NUM_CHIP_SELECT		(2)
struct pxa3xx_nand_platform_data {

	
	int	enable_arbiter;

	
	int	keep_config;

	
	int	num_cs;

	
	bool	flash_bbt;

	
	int ecc_strength, ecc_step_size;

	const struct mtd_partition		*parts[NUM_CHIP_SELECT];
	unsigned int				nr_parts[NUM_CHIP_SELECT];
};

extern void pxa3xx_set_nand_info(struct pxa3xx_nand_platform_data *info);
#endif 
