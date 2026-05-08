
#ifndef TTM_PAGE_ALLOC
#define TTM_PAGE_ALLOC

#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_memory.h>

struct device;


int ttm_page_alloc_init(struct ttm_mem_global *glob, unsigned max_pages);

void ttm_page_alloc_fini(void);


extern int ttm_pool_populate(struct ttm_tt *ttm);


extern void ttm_pool_unpopulate(struct ttm_tt *ttm);


extern int ttm_page_alloc_debugfs(struct seq_file *m, void *data);


#if defined(CONFIG_SWIOTLB) || defined(CONFIG_INTEL_IOMMU)

int ttm_dma_page_alloc_init(struct ttm_mem_global *glob, unsigned max_pages);


void ttm_dma_page_alloc_fini(void);


extern int ttm_dma_page_alloc_debugfs(struct seq_file *m, void *data);

extern int ttm_dma_populate(struct ttm_dma_tt *ttm_dma, struct device *dev);
extern void ttm_dma_unpopulate(struct ttm_dma_tt *ttm_dma, struct device *dev);

#else
static inline int ttm_dma_page_alloc_init(struct ttm_mem_global *glob,
					  unsigned max_pages)
{
	return -ENODEV;
}

static inline void ttm_dma_page_alloc_fini(void) { return; }

static inline int ttm_dma_page_alloc_debugfs(struct seq_file *m, void *data)
{
	return 0;
}
static inline int ttm_dma_populate(struct ttm_dma_tt *ttm_dma,
				   struct device *dev)
{
	return -ENOMEM;
}
static inline void ttm_dma_unpopulate(struct ttm_dma_tt *ttm_dma,
				      struct device *dev)
{
}
#endif

#endif
