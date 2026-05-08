
#ifndef __ASM_DEVICE_H
#define __ASM_DEVICE_H

struct dev_archdata {
	struct dma_map_ops *dma_ops;
#ifdef CONFIG_IOMMU_API
	void *iommu;			
#endif
	bool dma_coherent;
};

struct pdev_archdata {
};

#endif
