
#ifndef __ASM_CACHE_H
#define __ASM_CACHE_H

#include <asm/cachetype.h>

#define L1_CACHE_SHIFT		7
#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)


#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#ifndef __ASSEMBLY__

#define __read_mostly __attribute__((__section__(".data..read_mostly")))

static inline int cache_line_size(void)
{
	u32 cwg = cache_type_cwg();
	return cwg ? 4 << cwg : L1_CACHE_BYTES;
}

#endif	

#endif
