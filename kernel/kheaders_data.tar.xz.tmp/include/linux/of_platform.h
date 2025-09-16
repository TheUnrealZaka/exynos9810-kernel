#ifndef _LINUX_OF_PLATFORM_H
#define _LINUX_OF_PLATFORM_H


#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/pm.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>


struct of_dev_auxdata {
	char *compatible;
	resource_size_t phys_addr;
	char *name;
	void *platform_data;
};


#define OF_DEV_AUXDATA(_compat,_phys,_name,_pdata) \
	{ .compatible = _compat, .phys_addr = _phys, .name = _name, \
	  .platform_data = _pdata }

extern const struct of_device_id of_default_bus_match_table[];


extern struct platform_device *of_device_alloc(struct device_node *np,
					 const char *bus_id,
					 struct device *parent);
extern struct platform_device *of_find_device_by_node(struct device_node *np);


extern struct platform_device *of_platform_device_create(struct device_node *np,
						   const char *bus_id,
						   struct device *parent);

extern int of_platform_bus_probe(struct device_node *root,
				 const struct of_device_id *matches,
				 struct device *parent);
#ifdef CONFIG_OF_ADDRESS
extern int of_platform_populate(struct device_node *root,
				const struct of_device_id *matches,
				const struct of_dev_auxdata *lookup,
				struct device *parent);
extern int of_platform_default_populate(struct device_node *root,
					const struct of_dev_auxdata *lookup,
					struct device *parent);
extern void of_platform_depopulate(struct device *parent);
#else
static inline int of_platform_populate(struct device_node *root,
					const struct of_device_id *matches,
					const struct of_dev_auxdata *lookup,
					struct device *parent)
{
	return -ENODEV;
}
static inline int of_platform_default_populate(struct device_node *root,
					       const struct of_dev_auxdata *lookup,
					       struct device *parent)
{
	return -ENODEV;
}
static inline void of_platform_depopulate(struct device *parent) { }
#endif

#if defined(CONFIG_OF_DYNAMIC) && defined(CONFIG_OF_ADDRESS)
extern void of_platform_register_reconfig_notifier(void);
#else
static inline void of_platform_register_reconfig_notifier(void) { }
#endif

#endif	
