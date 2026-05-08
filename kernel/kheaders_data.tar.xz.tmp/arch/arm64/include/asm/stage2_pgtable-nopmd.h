

#ifndef __ARM64_S2_PGTABLE_NOPMD_H_
#define __ARM64_S2_PGTABLE_NOPMD_H_

#include <asm/stage2_pgtable-nopud.h>

#define __S2_PGTABLE_PMD_FOLDED

#define S2_PMD_SHIFT		S2_PUD_SHIFT
#define S2_PTRS_PER_PMD		1
#define S2_PMD_SIZE		(1UL << S2_PMD_SHIFT)
#define S2_PMD_MASK		(~(S2_PMD_SIZE-1))

#define stage2_pud_none(pud)			(0)
#define stage2_pud_present(pud)			(1)
#define stage2_pud_clear(pud)			do { } while (0)
#define stage2_pud_populate(pud, pmd)		do { } while (0)
#define stage2_pmd_offset(pud, address)		((pmd_t *)(pud))

#define stage2_pmd_free(pmd)			do { } while (0)

#define stage2_pmd_addr_end(addr, end)		(end)

#define stage2_pud_huge(pud)			(0)
#define stage2_pmd_table_empty(pmdp)		(0)

#endif
