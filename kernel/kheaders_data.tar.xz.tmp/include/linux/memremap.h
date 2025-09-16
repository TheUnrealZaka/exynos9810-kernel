#ifndef _LINUX_MEMREMAP_H_
#define _LINUX_MEMREMAP_H_
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/percpu-refcount.h>

struct resource;
struct device;


struct vmem_altmap {
	const unsigned long base_pfn;
	const unsigned long reserve;
	unsigned long free;
	unsigned long align;
	unsigned long alloc;
};

unsigned long vmem_altmap_offset(struct vmem_altmap *altmap);
void vmem_altmap_free(struct vmem_altmap *altmap, unsigned long nr_pfns);

#ifdef CONFIG_ZONE_DEVICE
struct vmem_altmap *to_vmem_altmap(unsigned long memmap_start);
#else
static inline struct vmem_altmap *to_vmem_altmap(unsigned long memmap_start)
{
	return NULL;
}
#endif


struct dev_pagemap {
	struct vmem_altmap *altmap;
	const struct resource *res;
	struct percpu_ref *ref;
	struct device *dev;
};

#ifdef CONFIG_ZONE_DEVICE
void *devm_memremap_pages(struct device *dev, struct resource *res,
		struct percpu_ref *ref, struct vmem_altmap *altmap);
struct dev_pagemap *find_dev_pagemap(resource_size_t phys);
#else
static inline void *devm_memremap_pages(struct device *dev,
		struct resource *res, struct percpu_ref *ref,
		struct vmem_altmap *altmap)
{
	
	WARN_ON_ONCE(1);
	return ERR_PTR(-ENXIO);
}

static inline struct dev_pagemap *find_dev_pagemap(resource_size_t phys)
{
	return NULL;
}
#endif


static inline struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap)
{
	const struct resource *res = pgmap ? pgmap->res : NULL;
	resource_size_t phys = PFN_PHYS(pfn);

	
	if (res && phys >= res->start && phys <= res->end) {
		percpu_ref_get(pgmap->ref);
		return pgmap;
	}

	
	rcu_read_lock();
	pgmap = find_dev_pagemap(phys);
	if (pgmap && !percpu_ref_tryget_live(pgmap->ref))
		pgmap = NULL;
	rcu_read_unlock();

	return pgmap;
}

static inline void put_dev_pagemap(struct dev_pagemap *pgmap)
{
	if (pgmap)
		percpu_ref_put(pgmap->ref);
}
#endif 
