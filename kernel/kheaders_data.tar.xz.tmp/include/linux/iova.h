

#ifndef _IOVA_H_
#define _IOVA_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/dma-mapping.h>


struct iova {
	struct rb_node	node;
	unsigned long	pfn_hi; 
	unsigned long	pfn_lo; 
};

struct iova_magazine;
struct iova_cpu_rcache;

#define IOVA_RANGE_CACHE_MAX_SIZE 6	
#define MAX_GLOBAL_MAGS 32	

struct iova_rcache {
	spinlock_t lock;
	unsigned long depot_size;
	struct iova_magazine *depot[MAX_GLOBAL_MAGS];
	struct iova_cpu_rcache __percpu *cpu_rcaches;
};


struct iova_domain {
	spinlock_t	iova_rbtree_lock; 
	struct rb_root	rbroot;		
	struct rb_node	*cached32_node; 
	unsigned long	granule;	
	unsigned long	start_pfn;	
	unsigned long	dma_32bit_pfn;
	struct iova_rcache rcaches[IOVA_RANGE_CACHE_MAX_SIZE];	
};

static inline unsigned long iova_size(struct iova *iova)
{
	return iova->pfn_hi - iova->pfn_lo + 1;
}

static inline unsigned long iova_shift(struct iova_domain *iovad)
{
	return __ffs(iovad->granule);
}

static inline unsigned long iova_mask(struct iova_domain *iovad)
{
	return iovad->granule - 1;
}

static inline size_t iova_offset(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova & iova_mask(iovad);
}

static inline size_t iova_align(struct iova_domain *iovad, size_t size)
{
	return ALIGN(size, iovad->granule);
}

static inline dma_addr_t iova_dma_addr(struct iova_domain *iovad, struct iova *iova)
{
	return (dma_addr_t)iova->pfn_lo << iova_shift(iovad);
}

static inline unsigned long iova_pfn(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova >> iova_shift(iovad);
}

int iova_cache_get(void);
void iova_cache_put(void);

struct iova *alloc_iova_mem(void);
void free_iova_mem(struct iova *iova);
void free_iova(struct iova_domain *iovad, unsigned long pfn);
void __free_iova(struct iova_domain *iovad, struct iova *iova);
struct iova *alloc_iova(struct iova_domain *iovad, unsigned long size,
	unsigned long limit_pfn,
	bool size_aligned);
void free_iova_fast(struct iova_domain *iovad, unsigned long pfn,
		    unsigned long size);
unsigned long alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
			      unsigned long limit_pfn);
struct iova *reserve_iova(struct iova_domain *iovad, unsigned long pfn_lo,
	unsigned long pfn_hi);
void copy_reserved_iova(struct iova_domain *from, struct iova_domain *to);
void init_iova_domain(struct iova_domain *iovad, unsigned long granule,
	unsigned long start_pfn, unsigned long pfn_32bit);
struct iova *find_iova(struct iova_domain *iovad, unsigned long pfn);
void put_iova_domain(struct iova_domain *iovad);
struct iova *split_and_remove_iova(struct iova_domain *iovad,
	struct iova *iova, unsigned long pfn_lo, unsigned long pfn_hi);
void free_cpu_cached_iovas(unsigned int cpu, struct iova_domain *iovad);

#endif
