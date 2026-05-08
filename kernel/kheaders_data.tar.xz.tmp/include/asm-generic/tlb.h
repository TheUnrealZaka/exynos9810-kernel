
#ifndef _ASM_GENERIC__TLB_H
#define _ASM_GENERIC__TLB_H

#include <linux/swap.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>

#ifdef CONFIG_HAVE_RCU_TABLE_FREE

struct mmu_table_batch {
	struct rcu_head		rcu;
	unsigned int		nr;
	void			*tables[0];
};

#define MAX_TABLE_BATCH		\
	((PAGE_SIZE - sizeof(struct mmu_table_batch)) / sizeof(void *))

extern void tlb_table_flush(struct mmu_gather *tlb);
extern void tlb_remove_table(struct mmu_gather *tlb, void *table);

void tlb_remove_table_sync_one(void);

#else

static inline void tlb_remove_table_sync_one(void) { }

#endif


#define MMU_GATHER_BUNDLE	8

struct mmu_gather_batch {
	struct mmu_gather_batch	*next;
	unsigned int		nr;
	unsigned int		max;
	struct page		*pages[0];
};

#define MAX_GATHER_BATCH	\
	((PAGE_SIZE - sizeof(struct mmu_gather_batch)) / sizeof(void *))


#define MAX_GATHER_BATCH_COUNT	(10000UL/MAX_GATHER_BATCH)


struct mmu_gather {
	struct mm_struct	*mm;
#ifdef CONFIG_HAVE_RCU_TABLE_FREE
	struct mmu_table_batch	*batch;
#endif
	unsigned long		start;
	unsigned long		end;
	
	unsigned int		fullmm : 1,
	
				need_flush_all : 1;

	struct mmu_gather_batch *active;
	struct mmu_gather_batch	local;
	struct page		*__pages[MMU_GATHER_BUNDLE];
	unsigned int		batch_count;
	
	unsigned long addr;
	int page_size;
};

#define HAVE_GENERIC_MMU_GATHER

void tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm, unsigned long start, unsigned long end);
void tlb_flush_mmu(struct mmu_gather *tlb);
void tlb_finish_mmu(struct mmu_gather *tlb, unsigned long start,
							unsigned long end);
extern bool __tlb_remove_page_size(struct mmu_gather *tlb, struct page *page,
				   int page_size);
void tlb_flush_pmd_range(struct mmu_gather *tlb, unsigned long address,
			 unsigned long size);

static inline void __tlb_adjust_range(struct mmu_gather *tlb,
				      unsigned long address)
{
	tlb->start = min(tlb->start, address);
	tlb->end = max(tlb->end, address + PAGE_SIZE);
	
	tlb->addr = address;
}

static inline void __tlb_reset_range(struct mmu_gather *tlb)
{
	if (tlb->fullmm) {
		tlb->start = tlb->end = ~0;
	} else {
		tlb->start = TASK_SIZE;
		tlb->end = 0;
	}
}

static inline void tlb_remove_page_size(struct mmu_gather *tlb,
					struct page *page, int page_size)
{
	if (__tlb_remove_page_size(tlb, page, page_size)) {
		tlb_flush_mmu(tlb);
		tlb->page_size = page_size;
		__tlb_adjust_range(tlb, tlb->addr);
		__tlb_remove_page_size(tlb, page, page_size);
	}
}

static bool __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return __tlb_remove_page_size(tlb, page, PAGE_SIZE);
}


static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return tlb_remove_page_size(tlb, page, PAGE_SIZE);
}

static inline bool __tlb_remove_pte_page(struct mmu_gather *tlb, struct page *page)
{
	
	VM_BUG_ON_PAGE(tlb->active->nr, page);
	tlb->page_size = PAGE_SIZE;
	__tlb_adjust_range(tlb, tlb->addr);
	return __tlb_remove_page(tlb, page);
}


#ifndef tlb_start_vma
#define tlb_start_vma(tlb, vma) do { } while (0)
#endif

#define __tlb_end_vma(tlb, vma)					\
	do {							\
		if (!tlb->fullmm && tlb->end) {			\
			tlb_flush(tlb);				\
			__tlb_reset_range(tlb);			\
		}						\
	} while (0)

#ifndef tlb_end_vma
#define tlb_end_vma	__tlb_end_vma
#endif

#ifndef __tlb_remove_tlb_entry
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)
#endif


#define tlb_remove_tlb_entry(tlb, ptep, address)		\
	do {							\
		__tlb_adjust_range(tlb, address);		\
		__tlb_remove_tlb_entry(tlb, ptep, address);	\
	} while (0)


#ifndef __tlb_remove_pmd_tlb_entry
#define __tlb_remove_pmd_tlb_entry(tlb, pmdp, address) do {} while (0)
#endif

#define tlb_remove_pmd_tlb_entry(tlb, pmdp, address)		\
	do {							\
		__tlb_adjust_range(tlb, address);		\
		__tlb_remove_pmd_tlb_entry(tlb, pmdp, address);	\
	} while (0)

#define pte_free_tlb(tlb, ptep, address)			\
	do {							\
		__tlb_adjust_range(tlb, address);		\
		__pte_free_tlb(tlb, ptep, address);		\
	} while (0)

#ifndef __ARCH_HAS_4LEVEL_HACK
#define pud_free_tlb(tlb, pudp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address);		\
		__pud_free_tlb(tlb, pudp, address);		\
	} while (0)
#endif

#define pmd_free_tlb(tlb, pmdp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address);		\
		__pmd_free_tlb(tlb, pmdp, address);		\
	} while (0)

#define tlb_migrate_finish(mm) do {} while (0)

#endif 
