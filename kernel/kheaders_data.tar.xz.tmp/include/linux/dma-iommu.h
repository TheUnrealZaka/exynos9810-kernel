
#ifndef __DMA_IOMMU_H
#define __DMA_IOMMU_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/errno.h>

#ifdef CONFIG_IOMMU_DMA
#include <linux/iommu.h>
#include <linux/msi.h>

int iommu_dma_init(void);


int iommu_get_dma_cookie(struct iommu_domain *domain);
void iommu_put_dma_cookie(struct iommu_domain *domain);


int iommu_dma_init_domain(struct iommu_domain *domain, dma_addr_t base,
		u64 size, struct device *dev);


int dma_direction_to_prot(enum dma_data_direction dir, bool coherent);


struct page **iommu_dma_alloc(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t *handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t));
void iommu_dma_free(struct device *dev, struct page **pages, size_t size,
		dma_addr_t *handle);

int iommu_dma_mmap(struct page **pages, size_t size, struct vm_area_struct *vma);

dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
		unsigned long offset, size_t size, int prot);
int iommu_dma_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, int prot);


void iommu_dma_unmap_page(struct device *dev, dma_addr_t handle, size_t size,
		enum dma_data_direction dir, unsigned long attrs);
void iommu_dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nents,
		enum dma_data_direction dir, unsigned long attrs);
int iommu_dma_supported(struct device *dev, u64 mask);
int iommu_dma_mapping_error(struct device *dev, dma_addr_t dma_addr);


void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg);

#else

struct iommu_domain;
struct msi_msg;

static inline int iommu_dma_init(void)
{
	return 0;
}

static inline int iommu_get_dma_cookie(struct iommu_domain *domain)
{
	return -ENODEV;
}

static inline void iommu_put_dma_cookie(struct iommu_domain *domain)
{
}

static inline void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg)
{
}

#endif	
#endif	
#endif	
