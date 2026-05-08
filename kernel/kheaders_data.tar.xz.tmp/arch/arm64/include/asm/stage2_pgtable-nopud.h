

#ifndef __ARM64_S2_PGTABLE_NOPUD_H_
#define __ARM64_S2_PGTABLE_NOPUD_H_

#define __S2_PGTABLE_PUD_FOLDED

#define S2_PUD_SHIFT		S2_PGDIR_SHIFT
#define S2_PTRS_PER_PUD		1
#define S2_PUD_SIZE		(_AC(1, UL) << S2_PUD_SHIFT)
#define S2_PUD_MASK		(~(S2_PUD_SIZE-1))

#define stage2_pgd_none(pgd)			(0)
#define stage2_pgd_present(pgd)			(1)
#define stage2_pgd_clear(pgd)			do { } while (0)
#define stage2_pgd_populate(pgd, pud)	do { } while (0)

#define stage2_pud_offset(pgd, address)		((pud_t *)(pgd))

#define stage2_pud_free(x)			do { } while (0)

#define stage2_pud_addr_end(addr, end)		(end)
#define stage2_pud_table_empty(pmdp)		(0)

#endif
