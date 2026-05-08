#ifndef _ASM_GENERIC_BITOPS_BUILTIN_FFS_H_
#define _ASM_GENERIC_BITOPS_BUILTIN_FFS_H_


static __always_inline int ffs(int x)
{
	return __builtin_ffs(x);
}

#endif
