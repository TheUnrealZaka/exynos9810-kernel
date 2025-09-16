

#ifndef __ASM_PLAT_IOVMM_H
#define __ASM_PLAT_IOVMM_H

#include <linux/dma-direction.h>
#include <linux/iommu.h>

#include <asm/page.h>
#include <linux/mm_types.h>

#define IOMMU_PFNMAP	(1 << 5) 

struct scatterlist;
struct device;

typedef u32 exynos_iova_t;

#define SYSMMU_FAULT_BITS	4
#define SYSMMU_FAULT_SHIFT	16
#define SYSMMU_FAULT_MASK	((1 << SYSMMU_FAULT_BITS) - 1)
#define SYSMMU_FAULT_FLAG(id) (((id) & SYSMMU_FAULT_MASK) << SYSMMU_FAULT_SHIFT)
#define SYSMMU_FAULT_ID(fg)   (((fg) >> SYSMMU_FAULT_SHIFT) & SYSMMU_FAULT_MASK)

#define SYSMMU_FAULT_PTW_ACCESS   0
#define SYSMMU_FAULT_PAGE_FAULT   1
#define SYSMMU_FAULT_TLB_MULTIHIT 2
#define SYSMMU_FAULT_ACCESS       3
#define SYSMMU_FAULT_SECURITY     4
#define SYSMMU_FAULT_UNKNOWN      5

#define IOMMU_FAULT_EXYNOS_PTW_ACCESS SYSMMU_FAULT_FLAG(SYSMMU_FAULT_PTW_ACCESS)
#define IOMMU_FAULT_EXYNOS_PAGE_FAULT SYSMMU_FAULT_FLAG(SYSMMU_FAULT_PAGE_FAULT)
#define IOMMU_FAULT_EXYNOS_TLB_MULTIHIT \
				SYSMMU_FAULT_FLAG(SYSMMU_FAULT_TLB_MULTIHIT)
#define IOMMU_FAULT_EXYNOS_ACCESS     SYSMMU_FAULT_FLAG(SYSMMU_FAULT_ACCESS)
#define IOMMU_FAULT_EXYNOS_SECURITY   SYSMMU_FAULT_FLAG(SYSMMU_FAULT_SECURITY)
#define IOMMU_FAULT_EXYNOS_UNKNOWN    SYSMMU_FAULT_FLAG(SYSMMU_FAULT_UNKOWN)


#define SYSMMU_PBUFCFG_TLB_UPDATE      (1 << 16)
#define SYSMMU_PBUFCFG_ASCENDING       (1 << 12)
#define SYSMMU_PBUFCFG_DESCENDING      (0 << 12)
#define SYSMMU_PBUFCFG_PREFETCH                (1 << 8)
#define SYSMMU_PBUFCFG_WRITE           (1 << 4)
#define SYSMMU_PBUFCFG_READ            (0 << 4)

#define SYSMMU_PBUFCFG_DEFAULT_INPUT   (SYSMMU_PBUFCFG_TLB_UPDATE | \
					SYSMMU_PBUFCFG_ASCENDING |  \
					SYSMMU_PBUFCFG_PREFETCH |   \
					SYSMMU_PBUFCFG_READ)
#define SYSMMU_PBUFCFG_DEFAULT_OUTPUT  (SYSMMU_PBUFCFG_TLB_UPDATE | \
					SYSMMU_PBUFCFG_ASCENDING |  \
					SYSMMU_PBUFCFG_PREFETCH |   \
					SYSMMU_PBUFCFG_WRITE)

#define SYSMMU_PBUFCFG_ASCENDING_INPUT   (SYSMMU_PBUFCFG_TLB_UPDATE | \
					SYSMMU_PBUFCFG_ASCENDING |  \
					SYSMMU_PBUFCFG_PREFETCH |   \
					SYSMMU_PBUFCFG_READ)

#define SYSMMU_PBUFCFG_DESCENDING_INPUT   (SYSMMU_PBUFCFG_TLB_UPDATE | \
					SYSMMU_PBUFCFG_DESCENDING |  \
					SYSMMU_PBUFCFG_PREFETCH |   \
					SYSMMU_PBUFCFG_READ)

enum sysmmu_ppc_event {
	READ_TOTAL,
	READ_L1TLB_MISS,
	READ_L2TLB_MISS,
	READ_FLPD_MISS,
	READ_PB_LOOKUP,
	READ_PB_MISS,
	READ_BLOCK_NUM_BY_PREFETCH,
	READ_BLOCK_CYCLE_BRY_PREFETCH,
	READ_TLB_MISS,
	READ_FLPD_MISS_PREFETCH,
	WRITE_TOTAL = 0x10,
	WRITE_L1TLB_MISS,
	WRITE_L2TLB_MISS,
	WRITE_FLPD_MISS,
	WRITE_PB_LOOKUP,
	WRITE_PB_MISS,
	WRITE_BLOCK_NUM_BY_PREFETCH,
	WRITE_BLOCK_CYCLE_BY_PREFETCH,
	WRITE_TLB_MISS,
	WRITE_FLPD_MISS_PREFETCH,
	TOTAL_ID_NUM,
};

struct sysmmu_prefbuf {
	unsigned long base;
	unsigned long size;
	unsigned long config;
};

#if defined(CONFIG_EXYNOS_IOVMM)
int iovmm_activate(struct device *dev);
void iovmm_deactivate(struct device *dev);
struct iommu_domain *get_domain_from_dev(struct device *dev);


dma_addr_t iovmm_map(struct device *dev, struct scatterlist *sg, off_t offset,
		size_t size, enum dma_data_direction direction, int prot);


void iovmm_unmap(struct device *dev, dma_addr_t iova);


#else
#define iovmm_activate(dev)		(-ENOSYS)
#define iovmm_deactivate(dev)		do { } while (0)
#define iovmm_map(dev, sg, offset, size, direction, prot) (-ENOSYS)
#define iovmm_unmap(dev, iova)		do { } while (0)
#define get_domain_from_dev(dev)	NULL
static inline dma_addr_t exynos_iovmm_map_userptr(struct device *dev,
			unsigned long vaddr, size_t size, int prot)
{
}
#define exynos_iovmm_unmap_userptr(dev, iova) do { } while (0)

#endif 

#if defined(CONFIG_EXYNOS_IOMMU)

int exynos_sysmmu_map_user_pages(struct device *dev,
					struct mm_struct *mm,
					unsigned long vaddr,
					exynos_iova_t iova,
					size_t size, bool write,
					bool shareable);


int exynos_sysmmu_unmap_user_pages(struct device *dev,
					struct mm_struct *mm,
					unsigned long vaddr,
					exynos_iova_t iova,
					size_t size);

void exynos_iommu_sync_for_device(struct device *dev, dma_addr_t iova,
				  size_t len, enum dma_data_direction dir);


void exynos_iommu_sync_for_cpu(struct device *dev, dma_addr_t iova, size_t len,
				enum dma_data_direction dir);


dma_addr_t exynos_iovmm_map_userptr(struct device *dev, unsigned long vaddr,
				    size_t size, int prot);

void exynos_iovmm_unmap_userptr(struct device *dev, dma_addr_t iova);


extern int handle_pte_fault(struct mm_struct *mm,
			    struct vm_area_struct *vma, unsigned long address,
			    pte_t *pte, pmd_t *pmd, unsigned int flags);


void sysmmu_set_prefetch_buffer_by_region(struct device *dev,
			struct sysmmu_prefbuf pb_reg[], unsigned int num_reg);

int sysmmu_set_prefetch_buffer_property(struct device *dev,
			unsigned int inplanes, unsigned int onplanes,
			unsigned int ipoption[], unsigned int opoption[]);
void exynos_sysmmu_show_status(struct device *dev);
void exynos_sysmmu_dump_pgtable(struct device *dev);


int exynos_sysmmu_set_ppc_event(struct device *dev, int event);
void exynos_sysmmu_clear_ppc_event(struct device *dev);
void exynos_sysmmu_show_ppc_event(struct device *dev);


void iovmm_set_fault_handler(struct device *dev,
			     iommu_fault_handler_t handler, void *token);

#else
#define sysmmu_set_prefetch_buffer_property(dev, inplanes, onplnes, ipoption, opoption) \
					(0)
#define sysmmu_set_prefetch_buffer_by_region(dev, pb_reg, num_reg) \
					do { } while (0)
#define exynos_sysmmu_map_user_pages(dev, mm, vaddr, iova, size, write, sharable) \
					(-ENOSYS)
#define exynos_sysmmu_unmap_user_pages(dev, mm, vaddr, iova, size) \
					do { } while (0)
#define exynos_sysmmu_show_status(dev) do { } while (0)
#define exynos_sysmmu_dump_pgtable(dev) do { } while (0)

#define exynos_sysmmu_clear_ppc_event(dev) do { } while (0)
#define exynos_sysmmu_show_ppc_event(dev) do { } while (0)
#define exynos_sysmmu_set_ppc_event(dev, event) do { } while (0)
#define iovmm_set_fault_handler(dev, handler, token) do { } while(0)

#define exynos_iommu_sync_for_device(dev, iova, len, dir) do { } while (0)
#define exynos_iommu_sync_for_cpu(dev, iova, len, dir) do { } while (0)

#endif
#endif 
