

#ifndef __DMA_MMP_TDMA_H
#define __DMA_MMP_TDMA_H

#include <linux/genalloc.h>


#define SRAM_GRANULARITY	512

enum sram_type {
	MMP_SRAM_UNDEFINED = 0,
	MMP_ASRAM,
	MMP_ISRAM,
};

struct sram_platdata {
	char *pool_name;
	int granularity;
};

#ifdef CONFIG_MMP_SRAM
extern struct gen_pool *sram_get_gpool(char *pool_name);
#else
static inline struct gen_pool *sram_get_gpool(char *pool_name)
{
	return NULL;
}
#endif

#endif 
