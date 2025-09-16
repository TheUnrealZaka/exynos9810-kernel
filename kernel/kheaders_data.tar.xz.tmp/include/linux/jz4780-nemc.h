

#ifndef __LINUX_JZ4780_NEMC_H__
#define __LINUX_JZ4780_NEMC_H__

#include <linux/types.h>

struct device;


#define JZ4780_NEMC_NUM_BANKS	7


enum jz4780_nemc_bank_type {
	JZ4780_NEMC_BANK_SRAM,
	JZ4780_NEMC_BANK_NAND,
};

extern unsigned int jz4780_nemc_num_banks(struct device *dev);

extern void jz4780_nemc_set_type(struct device *dev, unsigned int bank,
				 enum jz4780_nemc_bank_type type);
extern void jz4780_nemc_assert(struct device *dev, unsigned int bank,
			       bool assert);

#endif 
