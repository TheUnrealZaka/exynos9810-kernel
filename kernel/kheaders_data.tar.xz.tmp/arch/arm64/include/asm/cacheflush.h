
#ifndef __ASM_CACHEFLUSH_H
#define __ASM_CACHEFLUSH_H

#include <linux/mm.h>


#define PG_dcache_clean PG_arch_1


extern void flush_cache_louis(void);
extern void flush_cache_all(void);
extern void flush_cache_range(struct vm_area_struct *vma, unsigned long start, unsigned long end);
extern void flush_icache_range(unsigned long start, unsigned long end);
extern void __flush_dcache_area(void *addr, size_t len);
extern void __clean_dcache_area_poc(void *addr, size_t len);
extern void __clean_dcache_area_pou(void *addr, size_t len);
extern long __flush_cache_user_range(unsigned long start, unsigned long end);

static inline void flush_cache_mm(struct mm_struct *mm)
{
}

static inline void flush_cache_page(struct vm_area_struct *vma,
				    unsigned long user_addr, unsigned long pfn)
{
}


extern void __dma_map_area(const void *, size_t, int);
extern void __dma_unmap_area(const void *, size_t, int);
extern void __dma_flush_area(const void *, size_t);


extern void copy_to_user_page(struct vm_area_struct *, struct page *,
	unsigned long, void *, const void *, unsigned long);
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	do {							\
		memcpy(dst, src, len);				\
	} while (0)

#define flush_cache_dup_mm(mm) flush_cache_mm(mm)


#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
extern void flush_dcache_page(struct page *);

static inline void __flush_icache_all(void)
{
	asm("ic	ialluis");
	dsb(ish);
	isb();
}

#define flush_dcache_mmap_lock(mapping) \
	spin_lock_irq(&(mapping)->tree_lock)
#define flush_dcache_mmap_unlock(mapping) \
	spin_unlock_irq(&(mapping)->tree_lock)


#define flush_icache_page(vma,page)	do { } while (0)


static inline void flush_cache_vmap(unsigned long start, unsigned long end)
{
}

static inline void flush_cache_vunmap(unsigned long start, unsigned long end)
{
}

int set_memory_ro(unsigned long addr, int numpages);
int set_memory_rw(unsigned long addr, int numpages);
int set_memory_x(unsigned long addr, int numpages);
int set_memory_nx(unsigned long addr, int numpages);

#endif
