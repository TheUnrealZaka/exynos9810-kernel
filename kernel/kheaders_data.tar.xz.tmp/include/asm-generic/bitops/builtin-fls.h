#ifndef _ASM_GENERIC_BITOPS_BUILTIN_FLS_H_
#define _ASM_GENERIC_BITOPS_BUILTIN_FLS_H_


static __always_inline int fls(int x)
{
	return x ? sizeof(x) * 8 - __builtin_clz(x) : 0;
}

#endif
