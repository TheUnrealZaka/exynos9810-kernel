

#ifndef __ARM64_S2_PGTABLE_H_
#define __ARM64_S2_PGTABLE_H_

#include <asm/pgtable.h>


#define STAGE2_PGTABLE_LEVELS		ARM64_HW_PGTABLE_LEVELS(KVM_PHYS_SHIFT - 4)


#if STAGE2_PGTABLE_LEVELS > CONFIG_PGTABLE_LEVELS
#error "Unsupported combination of guest IPA and host VA_BITS."
#endif


#define S2_PGDIR_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(4 - STAGE2_PGTABLE_LEVELS)
#define S2_PGDIR_SIZE			(_AC(1, UL) << S2_PGDIR_SHIFT)
#define S2_PGDIR_MASK			(~(S2_PGDIR_SIZE - 1))


#define PTRS_PER_S2_PGD			(1 << (KVM_PHYS_SHIFT - S2_PGDIR_SHIFT))


#define KVM_MMU_CACHE_MIN_PAGES		(STAGE2_PGTABLE_LEVELS - 1)


#if STAGE2_PGTABLE_LEVELS > 3

#define S2_PUD_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(1)
#define S2_PUD_SIZE			(_AC(1, UL) << S2_PUD_SHIFT)
#define S2_PUD_MASK			(~(S2_PUD_SIZE - 1))

#define stage2_pgd_none(pgd)				pgd_none(pgd)
#define stage2_pgd_clear(pgd)				pgd_clear(pgd)
#define stage2_pgd_present(pgd)				pgd_present(pgd)
#define stage2_pgd_populate(pgd, pud)			pgd_populate(NULL, pgd, pud)
#define stage2_pud_offset(pgd, address)			pud_offset(pgd, address)
#define stage2_pud_free(pud)				pud_free(NULL, pud)

#define stage2_pud_table_empty(pudp)			kvm_page_empty(pudp)

static inline phys_addr_t stage2_pud_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PUD_SIZE) & S2_PUD_MASK;

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif		


#if STAGE2_PGTABLE_LEVELS > 2

#define S2_PMD_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(2)
#define S2_PMD_SIZE			(_AC(1, UL) << S2_PMD_SHIFT)
#define S2_PMD_MASK			(~(S2_PMD_SIZE - 1))

#define stage2_pud_none(pud)				pud_none(pud)
#define stage2_pud_clear(pud)				pud_clear(pud)
#define stage2_pud_present(pud)				pud_present(pud)
#define stage2_pud_populate(pud, pmd)			pud_populate(NULL, pud, pmd)
#define stage2_pmd_offset(pud, address)			pmd_offset(pud, address)
#define stage2_pmd_free(pmd)				pmd_free(NULL, pmd)

#define stage2_pud_huge(pud)				pud_huge(pud)
#define stage2_pmd_table_empty(pmdp)			kvm_page_empty(pmdp)

static inline phys_addr_t stage2_pmd_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PMD_SIZE) & S2_PMD_MASK;

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif		

#define stage2_pte_table_empty(ptep)			kvm_page_empty(ptep)

#if STAGE2_PGTABLE_LEVELS == 2
#include <asm/stage2_pgtable-nopmd.h>
#elif STAGE2_PGTABLE_LEVELS == 3
#include <asm/stage2_pgtable-nopud.h>
#endif


#define stage2_pgd_index(addr)				(((addr) >> S2_PGDIR_SHIFT) & (PTRS_PER_S2_PGD - 1))

static inline phys_addr_t stage2_pgd_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PGDIR_SIZE) & S2_PGDIR_MASK;

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif	
