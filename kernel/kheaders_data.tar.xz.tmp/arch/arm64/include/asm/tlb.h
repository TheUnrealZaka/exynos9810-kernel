
#ifndef __ASM_TLB_H
#define __ASM_TLB_H

#include <linux/pagemap.h>
#include <linux/swap.h>

#ifdef CONFIG_HAVE_RCU_TABLE_FREE

#define tlb_remove_entry(tlb, entry)	tlb_remove_table(tlb, entry)
static inline void __tlb_remove_table(void *_table)
{
	free_page_and_swap_cache((struct page *)_table);
}
#else
#define tlb_remove_entry(tlb, entry)	tlb_remove_page(tlb, entry)
#endif 

#include <asm-generic/tlb.h>

static inline void tlb_flush(struct mmu_gather *tlb)
{
	struct vm_area_struct vma = { .vm_mm = tlb->mm, };

	
	if (tlb->fullmm)
		return;

	
	__flush_tlb_range(&vma, tlb->start, tlb->end, true);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t pte,
				  unsigned long addr)
{
	__flush_tlb_pgtable(tlb->mm, addr);
	pgtable_page_dtor(pte);
	tlb_remove_entry(tlb, pte);
}

#if CONFIG_PGTABLE_LEVELS > 2
static inline void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmdp,
				  unsigned long addr)
{
#ifdef CONFIG_UH_RKP
	if (is_rkp_ro_page((unsigned long)pmdp)) {
		__flush_tlb_pgtable(tlb->mm, addr);
		rkp_ro_free((void *)pmdp);
	} else {
		__flush_tlb_pgtable(tlb->mm, addr);
		tlb_remove_entry(tlb, virt_to_page(pmdp));
	}
#else
	__flush_tlb_pgtable(tlb->mm, addr);
	tlb_remove_entry(tlb, virt_to_page(pmdp));
#endif

}
#endif

#if CONFIG_PGTABLE_LEVELS > 3
static inline void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pudp,
				  unsigned long addr)
{
#ifdef CONFIG_UH_RKP
	if (is_rkp_ro_page((unsigned long)pudp)) {
		__flush_tlb_pgtable(tlb->mm, addr);
		rkp_ro_free((void *)pudp);
	} else {
		__flush_tlb_pgtable(tlb->mm, addr);
		tlb_remove_entry(tlb, virt_to_page(pudp));
	}
#else
	__flush_tlb_pgtable(tlb->mm, addr);
	tlb_remove_entry(tlb, virt_to_page(pudp));
#endif
}
#endif

#endif
