#ifndef __LINUX_CMA_H
#define __LINUX_CMA_H





#ifdef __KERNEL__

#include <linux/device.h>

struct cma;
struct page;

#ifdef CONFIG_DMA_CMA

extern struct cma *dma_contiguous_default_area;

static inline struct cma *dev_get_cma_area(struct device *dev)
{
	if (dev && dev->cma_area)
		return dev->cma_area;
	return dma_contiguous_default_area;
}

static inline void dev_set_cma_area(struct device *dev, struct cma *cma)
{
	if (dev)
		dev->cma_area = cma;
}

static inline void dma_contiguous_set_default(struct cma *cma)
{
	dma_contiguous_default_area = cma;
}

void dma_contiguous_reserve(phys_addr_t addr_limit);

int __init dma_contiguous_reserve_area(phys_addr_t size, phys_addr_t base,
				       phys_addr_t limit, struct cma **res_cma,
				       bool fixed);



static inline int dma_declare_contiguous(struct device *dev, phys_addr_t size,
					 phys_addr_t base, phys_addr_t limit)
{
	struct cma *cma;
	int ret;
	ret = dma_contiguous_reserve_area(size, base, limit, &cma, true);
	if (ret == 0)
		dev_set_cma_area(dev, cma);

	return ret;
}

struct page *dma_alloc_from_contiguous(struct device *dev, size_t count,
				       unsigned int order);
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count);

#else

static inline struct cma *dev_get_cma_area(struct device *dev)
{
	return NULL;
}

static inline void dev_set_cma_area(struct device *dev, struct cma *cma) { }

static inline void dma_contiguous_set_default(struct cma *cma) { }

static inline void dma_contiguous_reserve(phys_addr_t limit) { }

static inline int dma_contiguous_reserve_area(phys_addr_t size, phys_addr_t base,
				       phys_addr_t limit, struct cma **res_cma,
				       bool fixed)
{
	return -ENOSYS;
}

static inline
int dma_declare_contiguous(struct device *dev, phys_addr_t size,
			   phys_addr_t base, phys_addr_t limit)
{
	return -ENOSYS;
}

static inline
struct page *dma_alloc_from_contiguous(struct device *dev, size_t count,
				       unsigned int order)
{
	return NULL;
}

static inline
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count)
{
	return false;
}

#endif

#endif

#endif
