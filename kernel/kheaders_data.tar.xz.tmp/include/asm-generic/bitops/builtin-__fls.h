#ifndef _ASM_GENERIC_BITOPS_BUILTIN___FLS_H_
#define _ASM_GENERIC_BITOPS_BUILTIN___FLS_H_


static __always_inline unsigned long __fls(unsigned long word)
{
	return (sizeof(word) * 8) - 1 - __builtin_clzl(word);
}

#endif
