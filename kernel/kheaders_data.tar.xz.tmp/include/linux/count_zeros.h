

#ifndef _LINUX_BITOPS_COUNT_ZEROS_H_
#define _LINUX_BITOPS_COUNT_ZEROS_H_

#include <asm/bitops.h>


static inline int count_leading_zeros(unsigned long x)
{
	if (sizeof(x) == 4)
		return BITS_PER_LONG - fls(x);
	else
		return BITS_PER_LONG - fls64(x);
}

#define COUNT_LEADING_ZEROS_0 BITS_PER_LONG


static inline int count_trailing_zeros(unsigned long x)
{
#define COUNT_TRAILING_ZEROS_0 (-1)

	if (sizeof(x) == 4)
		return ffs(x);
	else
		return (x != 0) ? __ffs(x) : COUNT_TRAILING_ZEROS_0;
}

#endif 
