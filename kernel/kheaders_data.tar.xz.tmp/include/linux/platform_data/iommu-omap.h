

#include <linux/platform_device.h>

#define MMU_REG_SIZE		256


struct omap_iommu_arch_data {
	const char *name;
	struct omap_iommu *iommu_dev;
};

struct iommu_platform_data {
	const char *name;
	const char *reset_name;
	int nr_tlb_entries;

	int (*assert_reset)(struct platform_device *pdev, const char *name);
	int (*deassert_reset)(struct platform_device *pdev, const char *name);
};
