
#ifndef __ASM_CACHETYPE_H
#define __ASM_CACHETYPE_H

#include <asm/cputype.h>

#define CTR_L1IP_SHIFT		14
#define CTR_L1IP_MASK		3
#define CTR_CWG_SHIFT		24
#define CTR_CWG_MASK		15
#define CTR_DMINLINE_SHIFT	16
#define CTR_IMINLINE_SHIFT	0

#define CTR_CACHE_MINLINE_MASK	\
	((0xf << CTR_DMINLINE_SHIFT) | (0xf << CTR_IMINLINE_SHIFT))

#define ICACHE_POLICY_RESERVED	0
#define ICACHE_POLICY_AIVIVT	1
#define ICACHE_POLICY_VIPT	2
#define ICACHE_POLICY_PIPT	3

#ifndef __ASSEMBLY__

#include <linux/bitops.h>

#define CTR_L1IP(ctr)	(((ctr) >> CTR_L1IP_SHIFT) & CTR_L1IP_MASK)

#define ICACHEF_ALIASING	0
#define ICACHEF_AIVIVT		1

extern unsigned long __icache_flags;


#define CCSIDR_EL1_WRITE_THROUGH	BIT(31)
#define CCSIDR_EL1_WRITE_BACK		BIT(30)
#define CCSIDR_EL1_READ_ALLOCATE	BIT(29)
#define CCSIDR_EL1_WRITE_ALLOCATE	BIT(28)
#define CCSIDR_EL1_LINESIZE_MASK	0x7
#define CCSIDR_EL1_LINESIZE(x)		((x) & CCSIDR_EL1_LINESIZE_MASK)
#define CCSIDR_EL1_ASSOCIATIVITY_SHIFT	3
#define CCSIDR_EL1_ASSOCIATIVITY_MASK	0x3ff
#define CCSIDR_EL1_ASSOCIATIVITY(x)	\
	(((x) >> CCSIDR_EL1_ASSOCIATIVITY_SHIFT) & CCSIDR_EL1_ASSOCIATIVITY_MASK)
#define CCSIDR_EL1_NUMSETS_SHIFT	13
#define CCSIDR_EL1_NUMSETS_MASK		0x7fff
#define CCSIDR_EL1_NUMSETS(x) \
	(((x) >> CCSIDR_EL1_NUMSETS_SHIFT) & CCSIDR_EL1_NUMSETS_MASK)

#define CACHE_LINESIZE(x)	(16 << CCSIDR_EL1_LINESIZE(x))
#define CACHE_NUMSETS(x)	(CCSIDR_EL1_NUMSETS(x) + 1)
#define CACHE_ASSOCIATIVITY(x)	(CCSIDR_EL1_ASSOCIATIVITY(x) + 1)

extern u64 __attribute_const__ cache_get_ccsidr(u64 csselr);


static inline int icache_get_linesize(void)
{
	return CACHE_LINESIZE(cache_get_ccsidr(1L));
}

static inline int icache_get_numsets(void)
{
	return CACHE_NUMSETS(cache_get_ccsidr(1L));
}


static inline int icache_is_aliasing(void)
{
	return test_bit(ICACHEF_ALIASING, &__icache_flags);
}

static inline int icache_is_aivivt(void)
{
	return test_bit(ICACHEF_AIVIVT, &__icache_flags);
}

static inline u32 cache_type_cwg(void)
{
	return (read_cpuid_cachetype() >> CTR_CWG_SHIFT) & CTR_CWG_MASK;
}

#endif	

#endif	
