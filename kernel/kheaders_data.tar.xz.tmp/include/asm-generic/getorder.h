#ifndef __ASM_GENERIC_GETORDER_H
#define __ASM_GENERIC_GETORDER_H

#ifndef __ASSEMBLY__

#include <linux/compiler.h>
#include <linux/log2.h>


static inline __attribute_const__ int get_order(unsigned long size)
{
	if (__builtin_constant_p(size)) {
		if (!size)
			return BITS_PER_LONG - PAGE_SHIFT;

		if (size < (1UL << PAGE_SHIFT))
			return 0;

		return ilog2((size) - 1) - PAGE_SHIFT + 1;
	}

	size--;
	size >>= PAGE_SHIFT;
#if BITS_PER_LONG == 32
	return fls(size);
#else
	return fls64(size);
#endif
}

#endif	

#endif	
