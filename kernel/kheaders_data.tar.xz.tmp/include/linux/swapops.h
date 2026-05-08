#ifndef _LINUX_SWAPOPS_H
#define _LINUX_SWAPOPS_H

#include <linux/radix-tree.h>
#include <linux/bug.h>


#define SWP_TYPE_SHIFT(e)	((sizeof(e.val) * 8) - \
			(MAX_SWAPFILES_SHIFT + RADIX_TREE_EXCEPTIONAL_SHIFT))
#define SWP_OFFSET_MASK(e)	((1UL << SWP_TYPE_SHIFT(e)) - 1)


static inline swp_entry_t swp_entry(unsigned long type, pgoff_t offset)
{
	swp_entry_t ret;

	ret.val = (type << SWP_TYPE_SHIFT(ret)) |
			(offset & SWP_OFFSET_MASK(ret));
	return ret;
}


static inline unsigned swp_type(swp_entry_t entry)
{
	return (entry.val >> SWP_TYPE_SHIFT(entry));
}


static inline pgoff_t swp_offset(swp_entry_t entry)
{
	return entry.val & SWP_OFFSET_MASK(entry);
}

#ifdef CONFIG_MMU

static inline int is_swap_pte(pte_t pte)
{
	return !pte_none(pte) && !pte_present(pte);
}
#endif


static inline swp_entry_t pte_to_swp_entry(pte_t pte)
{
	swp_entry_t arch_entry;

	if (pte_swp_soft_dirty(pte))
		pte = pte_swp_clear_soft_dirty(pte);
	arch_entry = __pte_to_swp_entry(pte);
	return swp_entry(__swp_type(arch_entry), __swp_offset(arch_entry));
}


static inline pte_t swp_entry_to_pte(swp_entry_t entry)
{
	swp_entry_t arch_entry;

	arch_entry = __swp_entry(swp_type(entry), swp_offset(entry));
	return __swp_entry_to_pte(arch_entry);
}

static inline swp_entry_t radix_to_swp_entry(void *arg)
{
	swp_entry_t entry;

	entry.val = (unsigned long)arg >> RADIX_TREE_EXCEPTIONAL_SHIFT;
	return entry;
}

static inline void *swp_to_radix_entry(swp_entry_t entry)
{
	unsigned long value;

	value = entry.val << RADIX_TREE_EXCEPTIONAL_SHIFT;
	return (void *)(value | RADIX_TREE_EXCEPTIONAL_ENTRY);
}

#ifdef CONFIG_MIGRATION
static inline swp_entry_t make_migration_entry(struct page *page, int write)
{
	BUG_ON(!PageLocked(page));
	return swp_entry(write ? SWP_MIGRATION_WRITE : SWP_MIGRATION_READ,
			page_to_pfn(page));
}

static inline int is_migration_entry(swp_entry_t entry)
{
	return unlikely(swp_type(entry) == SWP_MIGRATION_READ ||
			swp_type(entry) == SWP_MIGRATION_WRITE);
}

static inline int is_write_migration_entry(swp_entry_t entry)
{
	return unlikely(swp_type(entry) == SWP_MIGRATION_WRITE);
}

static inline struct page *migration_entry_to_page(swp_entry_t entry)
{
	struct page *p = pfn_to_page(swp_offset(entry));
	
	BUG_ON(!PageLocked(p));
	return p;
}

static inline void make_migration_entry_read(swp_entry_t *entry)
{
	*entry = swp_entry(SWP_MIGRATION_READ, swp_offset(*entry));
}

extern void __migration_entry_wait(struct mm_struct *mm, pte_t *ptep,
					spinlock_t *ptl);
extern void migration_entry_wait(struct mm_struct *mm, pmd_t *pmd,
					unsigned long address);
extern void migration_entry_wait_huge(struct vm_area_struct *vma,
		struct mm_struct *mm, pte_t *pte);
#else

#define make_migration_entry(page, write) swp_entry(0, 0)
static inline int is_migration_entry(swp_entry_t swp)
{
	return 0;
}
#define migration_entry_to_page(swp) NULL
static inline void make_migration_entry_read(swp_entry_t *entryp) { }
static inline void __migration_entry_wait(struct mm_struct *mm, pte_t *ptep,
					spinlock_t *ptl) { }
static inline void migration_entry_wait(struct mm_struct *mm, pmd_t *pmd,
					 unsigned long address) { }
static inline void migration_entry_wait_huge(struct vm_area_struct *vma,
		struct mm_struct *mm, pte_t *pte) { }
static inline int is_write_migration_entry(swp_entry_t entry)
{
	return 0;
}

#endif

#ifdef CONFIG_MEMORY_FAILURE

extern atomic_long_t num_poisoned_pages __read_mostly;


static inline swp_entry_t make_hwpoison_entry(struct page *page)
{
	BUG_ON(!PageLocked(page));
	return swp_entry(SWP_HWPOISON, page_to_pfn(page));
}

static inline int is_hwpoison_entry(swp_entry_t entry)
{
	return swp_type(entry) == SWP_HWPOISON;
}

static inline bool test_set_page_hwpoison(struct page *page)
{
	return TestSetPageHWPoison(page);
}

static inline void num_poisoned_pages_inc(void)
{
	atomic_long_inc(&num_poisoned_pages);
}

static inline void num_poisoned_pages_dec(void)
{
	atomic_long_dec(&num_poisoned_pages);
}

#else

static inline swp_entry_t make_hwpoison_entry(struct page *page)
{
	return swp_entry(0, 0);
}

static inline int is_hwpoison_entry(swp_entry_t swp)
{
	return 0;
}

static inline bool test_set_page_hwpoison(struct page *page)
{
	return false;
}

static inline void num_poisoned_pages_inc(void)
{
}
#endif

#if defined(CONFIG_MEMORY_FAILURE) || defined(CONFIG_MIGRATION)
static inline int non_swap_entry(swp_entry_t entry)
{
	return swp_type(entry) >= MAX_SWAPFILES;
}
#else
static inline int non_swap_entry(swp_entry_t entry)
{
	return 0;
}
#endif

#endif 
