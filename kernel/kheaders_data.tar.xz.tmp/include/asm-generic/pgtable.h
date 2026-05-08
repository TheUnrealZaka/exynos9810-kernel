#ifndef _ASM_GENERIC_PGTABLE_H
#define _ASM_GENERIC_PGTABLE_H

#include <linux/pfn.h>

#ifndef __ASSEMBLY__
#ifdef CONFIG_MMU

#include <linux/mm_types.h>
#include <linux/bug.h>
#include <linux/errno.h>

#if 4 - defined(__PAGETABLE_PUD_FOLDED) - defined(__PAGETABLE_PMD_FOLDED) != \
	CONFIG_PGTABLE_LEVELS
#error CONFIG_PGTABLE_LEVELS is not consistent with __PAGETABLE_{PUD,PMD}_FOLDED
#endif


#ifndef USER_PGTABLES_CEILING
#define USER_PGTABLES_CEILING	0UL
#endif

#ifndef __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
extern int ptep_set_access_flags(struct vm_area_struct *vma,
				 unsigned long address, pte_t *ptep,
				 pte_t entry, int dirty);
#endif

#ifndef __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
extern int pmdp_set_access_flags(struct vm_area_struct *vma,
				 unsigned long address, pmd_t *pmdp,
				 pmd_t entry, int dirty);
#else
static inline int pmdp_set_access_flags(struct vm_area_struct *vma,
					unsigned long address, pmd_t *pmdp,
					pmd_t entry, int dirty)
{
	BUILD_BUG();
	return 0;
}
#endif 
#endif

#ifndef __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
static inline int ptep_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long address,
					    pte_t *ptep)
{
	pte_t pte = *ptep;
	int r = 1;
	if (!pte_young(pte))
		r = 0;
	else
		set_pte_at(vma->vm_mm, address, ptep, pte_mkold(pte));
	return r;
}
#endif

#ifndef __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline int pmdp_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long address,
					    pmd_t *pmdp)
{
	pmd_t pmd = *pmdp;
	int r = 1;
	if (!pmd_young(pmd))
		r = 0;
	else
		set_pmd_at(vma->vm_mm, address, pmdp, pmd_mkold(pmd));
	return r;
}
#else
static inline int pmdp_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long address,
					    pmd_t *pmdp)
{
	BUILD_BUG();
	return 0;
}
#endif 
#endif

#ifndef __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
int ptep_clear_flush_young(struct vm_area_struct *vma,
			   unsigned long address, pte_t *ptep);
#endif

#ifndef __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
extern int pmdp_clear_flush_young(struct vm_area_struct *vma,
				  unsigned long address, pmd_t *pmdp);
#else

static inline int pmdp_clear_flush_young(struct vm_area_struct *vma,
					 unsigned long address, pmd_t *pmdp)
{
	BUILD_BUG();
	return 0;
}
#endif 
#endif

#ifndef __HAVE_ARCH_PTEP_GET_AND_CLEAR
static inline pte_t ptep_get_and_clear(struct mm_struct *mm,
				       unsigned long address,
				       pte_t *ptep)
{
	pte_t pte = *ptep;
	pte_clear(mm, address, ptep);
	return pte;
}
#endif

#ifndef __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline pmd_t pmdp_huge_get_and_clear(struct mm_struct *mm,
					    unsigned long address,
					    pmd_t *pmdp)
{
	pmd_t pmd = *pmdp;
	pmd_clear(pmdp);
	return pmd;
}
#endif 
#endif

#ifndef __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR_FULL
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline pmd_t pmdp_huge_get_and_clear_full(struct mm_struct *mm,
					    unsigned long address, pmd_t *pmdp,
					    int full)
{
	return pmdp_huge_get_and_clear(mm, address, pmdp);
}
#endif 
#endif

#ifndef __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL
static inline pte_t ptep_get_and_clear_full(struct mm_struct *mm,
					    unsigned long address, pte_t *ptep,
					    int full)
{
	pte_t pte;
	pte = ptep_get_and_clear(mm, address, ptep);
	return pte;
}
#endif


#ifndef __HAVE_ARCH_PTE_CLEAR_NOT_PRESENT_FULL
static inline void pte_clear_not_present_full(struct mm_struct *mm,
					      unsigned long address,
					      pte_t *ptep,
					      int full)
{
	pte_clear(mm, address, ptep);
}
#endif

#ifndef __HAVE_ARCH_PTEP_CLEAR_FLUSH
extern pte_t ptep_clear_flush(struct vm_area_struct *vma,
			      unsigned long address,
			      pte_t *ptep);
#endif

#ifndef __HAVE_ARCH_PMDP_HUGE_CLEAR_FLUSH
extern pmd_t pmdp_huge_clear_flush(struct vm_area_struct *vma,
			      unsigned long address,
			      pmd_t *pmdp);
#endif

#ifndef __HAVE_ARCH_PTEP_SET_WRPROTECT
struct mm_struct;
static inline void ptep_set_wrprotect(struct mm_struct *mm, unsigned long address, pte_t *ptep)
{
	pte_t old_pte = *ptep;
	set_pte_at(mm, address, ptep, pte_wrprotect(old_pte));
}
#endif

#ifdef CONFIG_TIMA_LKMAUTH
#ifdef CONFIG_TIMA_LKMAUTH_CODE_PROT
static inline void ptep_set_nxprotect(struct mm_struct *mm, unsigned long address, pte_t *ptep)
{
	pte_t old_pte = *ptep;
	set_pte_at(mm, address, ptep, pte_mknexec(old_pte));
	return;
}
#endif
#endif

#ifndef __HAVE_ARCH_PMDP_SET_WRPROTECT
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline void pmdp_set_wrprotect(struct mm_struct *mm,
				      unsigned long address, pmd_t *pmdp)
{
	pmd_t old_pmd = *pmdp;
	set_pmd_at(mm, address, pmdp, pmd_wrprotect(old_pmd));
}
#else
static inline void pmdp_set_wrprotect(struct mm_struct *mm,
				      unsigned long address, pmd_t *pmdp)
{
	BUILD_BUG();
}
#endif 
#endif

#ifndef pmdp_collapse_flush
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
extern pmd_t pmdp_collapse_flush(struct vm_area_struct *vma,
				 unsigned long address, pmd_t *pmdp);
#else
static inline pmd_t pmdp_collapse_flush(struct vm_area_struct *vma,
					unsigned long address,
					pmd_t *pmdp)
{
	BUILD_BUG();
	return *pmdp;
}
#define pmdp_collapse_flush pmdp_collapse_flush
#endif 
#endif

#ifndef __HAVE_ARCH_PGTABLE_DEPOSIT
extern void pgtable_trans_huge_deposit(struct mm_struct *mm, pmd_t *pmdp,
				       pgtable_t pgtable);
#endif

#ifndef __HAVE_ARCH_PGTABLE_WITHDRAW
extern pgtable_t pgtable_trans_huge_withdraw(struct mm_struct *mm, pmd_t *pmdp);
#endif

#ifdef CONFIG_TRANSPARENT_HUGEPAGE

static inline pmd_t generic_pmdp_establish(struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmdp, pmd_t pmd)
{
	pmd_t old_pmd = *pmdp;
	set_pmd_at(vma->vm_mm, address, pmdp, pmd);
	return old_pmd;
}
#endif

#ifndef __HAVE_ARCH_PMDP_INVALIDATE
extern void pmdp_invalidate(struct vm_area_struct *vma, unsigned long address,
			    pmd_t *pmdp);
#endif

#ifndef __HAVE_ARCH_PMDP_HUGE_SPLIT_PREPARE
static inline void pmdp_huge_split_prepare(struct vm_area_struct *vma,
					   unsigned long address, pmd_t *pmdp)
{

}
#endif

#ifndef __HAVE_ARCH_PTE_SAME
static inline int pte_same(pte_t pte_a, pte_t pte_b)
{
	return pte_val(pte_a) == pte_val(pte_b);
}
#endif

#ifndef __HAVE_ARCH_PTE_UNUSED

static inline int pte_unused(pte_t pte)
{
	return 0;
}
#endif

#ifndef __HAVE_ARCH_PMD_SAME
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline int pmd_same(pmd_t pmd_a, pmd_t pmd_b)
{
	return pmd_val(pmd_a) == pmd_val(pmd_b);
}
#else 
static inline int pmd_same(pmd_t pmd_a, pmd_t pmd_b)
{
	BUILD_BUG();
	return 0;
}
#endif 
#endif

#ifndef __HAVE_ARCH_PGD_OFFSET_GATE
#define pgd_offset_gate(mm, addr)	pgd_offset(mm, addr)
#endif

#ifndef __HAVE_ARCH_MOVE_PTE
#define move_pte(pte, prot, old_addr, new_addr)	(pte)
#endif

#ifndef pte_accessible
# define pte_accessible(mm, pte)	((void)(pte), 1)
#endif

#ifndef flush_tlb_fix_spurious_fault
#define flush_tlb_fix_spurious_fault(vma, address) flush_tlb_page(vma, address)
#endif

#ifndef pgprot_noncached
#define pgprot_noncached(prot)	(prot)
#endif

#ifndef pgprot_writecombine
#define pgprot_writecombine pgprot_noncached
#endif

#ifndef pgprot_writethrough
#define pgprot_writethrough pgprot_noncached
#endif

#ifndef pgprot_device
#define pgprot_device pgprot_noncached
#endif

#ifndef pgprot_modify
#define pgprot_modify pgprot_modify
static inline pgprot_t pgprot_modify(pgprot_t oldprot, pgprot_t newprot)
{
	if (pgprot_val(oldprot) == pgprot_val(pgprot_noncached(oldprot)))
		newprot = pgprot_noncached(newprot);
	if (pgprot_val(oldprot) == pgprot_val(pgprot_writecombine(oldprot)))
		newprot = pgprot_writecombine(newprot);
	if (pgprot_val(oldprot) == pgprot_val(pgprot_device(oldprot)))
		newprot = pgprot_device(newprot);
	return newprot;
}
#endif



#define pgd_addr_end(addr, end)						\
({	unsigned long __boundary = ((addr) + PGDIR_SIZE) & PGDIR_MASK;	\
	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
})

#ifndef pud_addr_end
#define pud_addr_end(addr, end)						\
({	unsigned long __boundary = ((addr) + PUD_SIZE) & PUD_MASK;	\
	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
})
#endif

#ifndef pmd_addr_end
#define pmd_addr_end(addr, end)						\
({	unsigned long __boundary = ((addr) + PMD_SIZE) & PMD_MASK;	\
	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
})
#endif


void pgd_clear_bad(pgd_t *);
void pud_clear_bad(pud_t *);
void pmd_clear_bad(pmd_t *);

static inline int pgd_none_or_clear_bad(pgd_t *pgd)
{
	if (pgd_none(*pgd))
		return 1;
	if (unlikely(pgd_bad(*pgd))) {
		pgd_clear_bad(pgd);
		return 1;
	}
	return 0;
}

static inline int pud_none_or_clear_bad(pud_t *pud)
{
	if (pud_none(*pud))
		return 1;
	if (unlikely(pud_bad(*pud))) {
		pud_clear_bad(pud);
		return 1;
	}
	return 0;
}

static inline int pmd_none_or_clear_bad(pmd_t *pmd)
{
	if (pmd_none(*pmd))
		return 1;
	if (unlikely(pmd_bad(*pmd))) {
		pmd_clear_bad(pmd);
		return 1;
	}
	return 0;
}

static inline pte_t __ptep_modify_prot_start(struct mm_struct *mm,
					     unsigned long addr,
					     pte_t *ptep)
{
	
	return ptep_get_and_clear(mm, addr, ptep);
}

static inline void __ptep_modify_prot_commit(struct mm_struct *mm,
					     unsigned long addr,
					     pte_t *ptep, pte_t pte)
{
	
	set_pte_at(mm, addr, ptep, pte);
}

#ifndef __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION

static inline pte_t ptep_modify_prot_start(struct mm_struct *mm,
					   unsigned long addr,
					   pte_t *ptep)
{
	return __ptep_modify_prot_start(mm, addr, ptep);
}


static inline void ptep_modify_prot_commit(struct mm_struct *mm,
					   unsigned long addr,
					   pte_t *ptep, pte_t pte)
{
	__ptep_modify_prot_commit(mm, addr, ptep, pte);
}
#endif 
#endif 


#ifndef __HAVE_ARCH_ENTER_LAZY_MMU_MODE
#define arch_enter_lazy_mmu_mode()	do {} while (0)
#define arch_leave_lazy_mmu_mode()	do {} while (0)
#define arch_flush_lazy_mmu_mode()	do {} while (0)
#endif


#ifndef __HAVE_ARCH_START_CONTEXT_SWITCH
#define arch_start_context_switch(prev)	do {} while (0)
#endif

#ifndef CONFIG_HAVE_ARCH_SOFT_DIRTY
static inline int pte_soft_dirty(pte_t pte)
{
	return 0;
}

static inline int pmd_soft_dirty(pmd_t pmd)
{
	return 0;
}

static inline pte_t pte_mksoft_dirty(pte_t pte)
{
	return pte;
}

static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
{
	return pmd;
}

static inline pte_t pte_clear_soft_dirty(pte_t pte)
{
	return pte;
}

static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
{
	return pmd;
}

static inline pte_t pte_swp_mksoft_dirty(pte_t pte)
{
	return pte;
}

static inline int pte_swp_soft_dirty(pte_t pte)
{
	return 0;
}

static inline pte_t pte_swp_clear_soft_dirty(pte_t pte)
{
	return pte;
}
#endif

#ifndef __HAVE_PFNMAP_TRACKING



static inline int track_pfn_remap(struct vm_area_struct *vma, pgprot_t *prot,
				  unsigned long pfn, unsigned long addr,
				  unsigned long size)
{
	return 0;
}


static inline int track_pfn_insert(struct vm_area_struct *vma, pgprot_t *prot,
				   pfn_t pfn)
{
	return 0;
}


static inline int track_pfn_copy(struct vm_area_struct *vma)
{
	return 0;
}


static inline void untrack_pfn(struct vm_area_struct *vma,
			       unsigned long pfn, unsigned long size)
{
}


static inline void untrack_pfn_moved(struct vm_area_struct *vma)
{
}
#else
extern int track_pfn_remap(struct vm_area_struct *vma, pgprot_t *prot,
			   unsigned long pfn, unsigned long addr,
			   unsigned long size);
extern int track_pfn_insert(struct vm_area_struct *vma, pgprot_t *prot,
			    pfn_t pfn);
extern int track_pfn_copy(struct vm_area_struct *vma);
extern void untrack_pfn(struct vm_area_struct *vma, unsigned long pfn,
			unsigned long size);
extern void untrack_pfn_moved(struct vm_area_struct *vma);
#endif

#ifdef __HAVE_COLOR_ZERO_PAGE
static inline int is_zero_pfn(unsigned long pfn)
{
	extern unsigned long zero_pfn;
	unsigned long offset_from_zero_pfn = pfn - zero_pfn;
	return offset_from_zero_pfn <= (zero_page_mask >> PAGE_SHIFT);
}

#define my_zero_pfn(addr)	page_to_pfn(ZERO_PAGE(addr))

#else
static inline int is_zero_pfn(unsigned long pfn)
{
	extern unsigned long zero_pfn;
	return pfn == zero_pfn;
}

static inline unsigned long my_zero_pfn(unsigned long addr)
{
	extern unsigned long zero_pfn;
	return zero_pfn;
}
#endif

#ifdef CONFIG_MMU

#ifndef CONFIG_TRANSPARENT_HUGEPAGE
static inline int pmd_trans_huge(pmd_t pmd)
{
	return 0;
}
#ifndef __HAVE_ARCH_PMD_WRITE
static inline int pmd_write(pmd_t pmd)
{
	BUG();
	return 0;
}
#endif 
#endif 

#ifndef pmd_read_atomic
static inline pmd_t pmd_read_atomic(pmd_t *pmdp)
{
	
	return *pmdp;
}
#endif

#ifndef pmd_move_must_withdraw
static inline int pmd_move_must_withdraw(spinlock_t *new_pmd_ptl,
					 spinlock_t *old_pmd_ptl)
{
	
	return new_pmd_ptl != old_pmd_ptl;
}
#endif


static inline int pmd_none_or_trans_huge_or_clear_bad(pmd_t *pmd)
{
	pmd_t pmdval = pmd_read_atomic(pmd);
	
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	barrier();
#endif
	if (pmd_none(pmdval) || pmd_trans_huge(pmdval))
		return 1;
	if (unlikely(pmd_bad(pmdval))) {
		pmd_clear_bad(pmd);
		return 1;
	}
	return 0;
}


static inline int pmd_trans_unstable(pmd_t *pmd)
{
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	return pmd_none_or_trans_huge_or_clear_bad(pmd);
#else
	return 0;
#endif
}

#ifndef CONFIG_NUMA_BALANCING

static inline int pte_protnone(pte_t pte)
{
	return 0;
}

static inline int pmd_protnone(pmd_t pmd)
{
	return 0;
}
#endif 

#endif 

#ifdef CONFIG_HAVE_ARCH_HUGE_VMAP
int pud_set_huge(pud_t *pud, phys_addr_t addr, pgprot_t prot);
int pmd_set_huge(pmd_t *pmd, phys_addr_t addr, pgprot_t prot);
int pud_clear_huge(pud_t *pud);
int pmd_clear_huge(pmd_t *pmd);
int pud_free_pmd_page(pud_t *pud, unsigned long addr);
int pmd_free_pte_page(pmd_t *pmd, unsigned long addr);
#else	
static inline int pud_set_huge(pud_t *pud, phys_addr_t addr, pgprot_t prot)
{
	return 0;
}
static inline int pmd_set_huge(pmd_t *pmd, phys_addr_t addr, pgprot_t prot)
{
	return 0;
}
static inline int pud_clear_huge(pud_t *pud)
{
	return 0;
}
static inline int pmd_clear_huge(pmd_t *pmd)
{
	return 0;
}
static inline int pud_free_pmd_page(pud_t *pud, unsigned long addr)
{
	return 0;
}
static inline int pmd_free_pte_page(pmd_t *pmd, unsigned long addr)
{
	return 0;
}
#endif	

#ifndef __HAVE_ARCH_FLUSH_PMD_TLB_RANGE
#ifdef CONFIG_TRANSPARENT_HUGEPAGE

#define flush_pmd_tlb_range(vma, addr, end)	flush_tlb_range(vma, addr, end)
#else
#define flush_pmd_tlb_range(vma, addr, end)	BUILD_BUG()
#endif
#endif

struct file;
int phys_mem_access_prot_allowed(struct file *file, unsigned long pfn,
			unsigned long size, pgprot_t *vma_prot);

#ifndef __HAVE_ARCH_PFN_MODIFY_ALLOWED
static inline bool pfn_modify_allowed(unsigned long pfn, pgprot_t prot)
{
	return true;
}

static inline bool arch_has_pfn_modify_check(void)
{
	return false;
}
#endif 

#ifndef PAGE_KERNEL_EXEC
# define PAGE_KERNEL_EXEC PAGE_KERNEL
#endif

#endif 

#ifndef io_remap_pfn_range
#define io_remap_pfn_range remap_pfn_range
#endif

#if !defined(MAX_POSSIBLE_PHYSMEM_BITS) && !defined(CONFIG_64BIT)
#ifdef CONFIG_PHYS_ADDR_T_64BIT

#error Missing MAX_POSSIBLE_PHYSMEM_BITS definition
#else
#define MAX_POSSIBLE_PHYSMEM_BITS 32
#endif
#endif

#ifndef has_transparent_hugepage
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
#define has_transparent_hugepage() 1
#else
#define has_transparent_hugepage() 0
#endif
#endif

#endif 
