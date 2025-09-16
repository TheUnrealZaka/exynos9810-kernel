

#ifndef _OMAP_IOMMU_H_
#define _OMAP_IOMMU_H_

extern void omap_iommu_save_ctx(struct device *dev);
extern void omap_iommu_restore_ctx(struct device *dev);

#endif
