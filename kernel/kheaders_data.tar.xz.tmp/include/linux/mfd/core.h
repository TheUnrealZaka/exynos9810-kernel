

#ifndef MFD_CORE_H
#define MFD_CORE_H

#include <linux/platform_device.h>

struct irq_domain;
struct property_entry;


struct mfd_cell_acpi_match {
	const char			*pnpid;
	const unsigned long long	adr;
};


struct mfd_cell {
	const char		*name;
	int			id;

	
	atomic_t		*usage_count;
	int			(*enable)(struct platform_device *dev);
	int			(*disable)(struct platform_device *dev);

	int			(*suspend)(struct platform_device *dev);
	int			(*resume)(struct platform_device *dev);

	
	void			*platform_data;
	size_t			pdata_size;

	
	struct property_entry *properties;

	
	const char		*of_compatible;

	
	const struct mfd_cell_acpi_match	*acpi_match;

	
	int			num_resources;
	const struct resource	*resources;

	
	bool			ignore_resource_conflicts;

	
	bool			pm_runtime_no_callbacks;

	
	const char * const	*parent_supplies;
	int			num_parent_supplies;
};


extern int mfd_cell_enable(struct platform_device *pdev);
extern int mfd_cell_disable(struct platform_device *pdev);


extern int mfd_clone_cell(const char *cell, const char **clones,
		size_t n_clones);


static inline const struct mfd_cell *mfd_get_cell(struct platform_device *pdev)
{
	return pdev->mfd_cell;
}

extern int mfd_add_devices(struct device *parent, int id,
			   const struct mfd_cell *cells, int n_devs,
			   struct resource *mem_base,
			   int irq_base, struct irq_domain *irq_domain);

static inline int mfd_add_hotplug_devices(struct device *parent,
		const struct mfd_cell *cells, int n_devs)
{
	return mfd_add_devices(parent, PLATFORM_DEVID_AUTO, cells, n_devs,
			NULL, 0, NULL);
}

extern void mfd_remove_devices(struct device *parent);

extern int devm_mfd_add_devices(struct device *dev, int id,
				const struct mfd_cell *cells, int n_devs,
				struct resource *mem_base,
				int irq_base, struct irq_domain *irq_domain);
#endif
