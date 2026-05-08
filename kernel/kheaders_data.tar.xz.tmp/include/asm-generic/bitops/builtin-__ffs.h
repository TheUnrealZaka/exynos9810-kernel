#ifndef _ASM_GENERIC_BITOPS_BUILTIN___FFS_H_
#define _ASM_GENERIC_BITOPS_BUILTIN___FFS_H_


static __always_inline unsigned long __ffs(unsigned long word)
{
	return __builtin_ctzl(word);
}

#endif
