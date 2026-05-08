

#ifndef __LINUX_IOMMU_H
#define __LINUX_IOMMU_H

#include <linux/errno.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/scatterlist.h>
#include <trace/events/iommu.h>

#define IOMMU_READ	(1 << 0)
#define IOMMU_WRITE	(1 << 1)
#define IOMMU_CACHE	(1 << 2) 
#define IOMMU_NOEXEC	(1 << 3)
#define IOMMU_MMIO	(1 << 4) 

struct iommu_ops;
struct iommu_group;
struct bus_type;
struct device;
struct iommu_domain;
struct notifier_block;


#define IOMMU_FAULT_READ	0x0
#define IOMMU_FAULT_WRITE	0x1

typedef int (*iommu_fault_handler_t)(struct iommu_domain *,
			struct device *, unsigned long, int, void *);

struct iommu_domain_geometry {
	dma_addr_t aperture_start; 
	dma_addr_t aperture_end;   
	bool force_aperture;       
};


#define __IOMMU_DOMAIN_PAGING	(1U << 0)  
#define __IOMMU_DOMAIN_DMA_API	(1U << 1)  
#define __IOMMU_DOMAIN_PT	(1U << 2)  


#define IOMMU_DOMAIN_BLOCKED	(0U)
#define IOMMU_DOMAIN_IDENTITY	(__IOMMU_DOMAIN_PT)
#define IOMMU_DOMAIN_UNMANAGED	(__IOMMU_DOMAIN_PAGING)
#define IOMMU_DOMAIN_DMA	(__IOMMU_DOMAIN_PAGING |	\
				 __IOMMU_DOMAIN_DMA_API)

struct iommu_domain {
	unsigned type;
	const struct iommu_ops *ops;
	unsigned long pgsize_bitmap;	
	iommu_fault_handler_t handler;
	void *handler_token;
	struct iommu_domain_geometry geometry;
	void *iova_cookie;
};

enum iommu_cap {
	IOMMU_CAP_CACHE_COHERENCY,	
	IOMMU_CAP_INTR_REMAP,		
	IOMMU_CAP_NOEXEC,		
};



enum iommu_attr {
	DOMAIN_ATTR_GEOMETRY,
	DOMAIN_ATTR_PAGING,
	DOMAIN_ATTR_WINDOWS,
	DOMAIN_ATTR_FSL_PAMU_STASH,
	DOMAIN_ATTR_FSL_PAMU_ENABLE,
	DOMAIN_ATTR_FSL_PAMUV1,
	DOMAIN_ATTR_NESTING,	
	DOMAIN_ATTR_MAX,
};


struct iommu_dm_region {
	struct list_head	list;
	phys_addr_t		start;
	size_t			length;
	int			prot;
};

#ifdef CONFIG_IOMMU_API


struct iommu_ops {
	bool (*capable)(enum iommu_cap);

	
	struct iommu_domain *(*domain_alloc)(unsigned iommu_domain_type);
	void (*domain_free)(struct iommu_domain *);

	int (*attach_dev)(struct iommu_domain *domain, struct device *dev);
	void (*detach_dev)(struct iommu_domain *domain, struct device *dev);
	int (*map)(struct iommu_domain *domain, unsigned long iova,
		   phys_addr_t paddr, size_t size, int prot);
	size_t (*unmap)(struct iommu_domain *domain, unsigned long iova,
		     size_t size);
	size_t (*map_sg)(struct iommu_domain *domain, unsigned long iova,
			 struct scatterlist *sg, unsigned int nents, int prot);
	phys_addr_t (*iova_to_phys)(struct iommu_domain *domain, dma_addr_t iova);
	int (*add_device)(struct device *dev);
	void (*remove_device)(struct device *dev);
	struct iommu_group *(*device_group)(struct device *dev);
	int (*domain_get_attr)(struct iommu_domain *domain,
			       enum iommu_attr attr, void *data);
	int (*domain_set_attr)(struct iommu_domain *domain,
			       enum iommu_attr attr, void *data);

	
	void (*get_dm_regions)(struct device *dev, struct list_head *list);
	void (*put_dm_regions)(struct device *dev, struct list_head *list);
	void (*apply_dm_region)(struct device *dev, struct iommu_domain *domain,
				struct iommu_dm_region *region);

	
	int (*domain_window_enable)(struct iommu_domain *domain, u32 wnd_nr,
				    phys_addr_t paddr, u64 size, int prot);
	void (*domain_window_disable)(struct iommu_domain *domain, u32 wnd_nr);
	
	int (*domain_set_windows)(struct iommu_domain *domain, u32 w_count);
	
	u32 (*domain_get_windows)(struct iommu_domain *domain);

	int (*of_xlate)(struct device *dev, struct of_phandle_args *args);

	unsigned long pgsize_bitmap;
};

#define IOMMU_GROUP_NOTIFY_ADD_DEVICE		1 
#define IOMMU_GROUP_NOTIFY_DEL_DEVICE		2 
#define IOMMU_GROUP_NOTIFY_BIND_DRIVER		3 
#define IOMMU_GROUP_NOTIFY_BOUND_DRIVER		4 
#define IOMMU_GROUP_NOTIFY_UNBIND_DRIVER	5 
#define IOMMU_GROUP_NOTIFY_UNBOUND_DRIVER	6 

extern int bus_set_iommu(struct bus_type *bus, const struct iommu_ops *ops);
extern bool iommu_present(struct bus_type *bus);
extern bool iommu_capable(struct bus_type *bus, enum iommu_cap cap);
extern struct iommu_domain *iommu_domain_alloc(struct bus_type *bus);
extern struct iommu_group *iommu_group_get_by_id(int id);
extern void iommu_domain_free(struct iommu_domain *domain);
extern int iommu_attach_device(struct iommu_domain *domain,
			       struct device *dev);
extern void iommu_detach_device(struct iommu_domain *domain,
				struct device *dev);
extern struct iommu_domain *iommu_get_domain_for_dev(struct device *dev);
extern int iommu_map(struct iommu_domain *domain, unsigned long iova,
		     phys_addr_t paddr, size_t size, int prot);
extern size_t iommu_unmap(struct iommu_domain *domain, unsigned long iova,
		       size_t size);
extern size_t default_iommu_map_sg(struct iommu_domain *domain, unsigned long iova,
				struct scatterlist *sg,unsigned int nents,
				int prot);
extern phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, dma_addr_t iova);
extern void iommu_set_fault_handler(struct iommu_domain *domain,
			iommu_fault_handler_t handler, void *token);

extern void iommu_get_dm_regions(struct device *dev, struct list_head *list);
extern void iommu_put_dm_regions(struct device *dev, struct list_head *list);
extern int iommu_request_dm_for_dev(struct device *dev);

extern int iommu_attach_group(struct iommu_domain *domain,
			      struct iommu_group *group);
extern void iommu_detach_group(struct iommu_domain *domain,
			       struct iommu_group *group);
extern struct iommu_group *iommu_group_alloc(void);
extern void *iommu_group_get_iommudata(struct iommu_group *group);
extern void iommu_group_set_iommudata(struct iommu_group *group,
				      void *iommu_data,
				      void (*release)(void *iommu_data));
extern int iommu_group_set_name(struct iommu_group *group, const char *name);
extern int iommu_group_add_device(struct iommu_group *group,
				  struct device *dev);
extern void iommu_group_remove_device(struct device *dev);
extern int iommu_group_for_each_dev(struct iommu_group *group, void *data,
				    int (*fn)(struct device *, void *));
extern struct iommu_group *iommu_group_get(struct device *dev);
extern void iommu_group_put(struct iommu_group *group);
extern int iommu_group_register_notifier(struct iommu_group *group,
					 struct notifier_block *nb);
extern int iommu_group_unregister_notifier(struct iommu_group *group,
					   struct notifier_block *nb);
extern int iommu_group_id(struct iommu_group *group);
extern struct iommu_group *iommu_group_get_for_dev(struct device *dev);
extern struct iommu_domain *iommu_group_default_domain(struct iommu_group *);

extern int iommu_domain_get_attr(struct iommu_domain *domain, enum iommu_attr,
				 void *data);
extern int iommu_domain_set_attr(struct iommu_domain *domain, enum iommu_attr,
				 void *data);
struct device *iommu_device_create(struct device *parent, void *drvdata,
				   const struct attribute_group **groups,
				   const char *fmt, ...) __printf(4, 5);
void iommu_device_destroy(struct device *dev);
int iommu_device_link(struct device *dev, struct device *link);
void iommu_device_unlink(struct device *dev, struct device *link);


extern int iommu_domain_window_enable(struct iommu_domain *domain, u32 wnd_nr,
				      phys_addr_t offset, u64 size,
				      int prot);
extern void iommu_domain_window_disable(struct iommu_domain *domain, u32 wnd_nr);

static inline int report_iommu_fault(struct iommu_domain *domain,
		struct device *dev, unsigned long iova, int flags)
{
	int ret = -ENOSYS;

	
	if (domain->handler)
		ret = domain->handler(domain, dev, iova, flags,
						domain->handler_token);

	trace_io_page_fault(dev, iova, flags);
	return ret;
}

static inline size_t iommu_map_sg(struct iommu_domain *domain,
				  unsigned long iova, struct scatterlist *sg,
				  unsigned int nents, int prot)
{
	return domain->ops->map_sg(domain, iova, sg, nents, prot);
}


extern struct iommu_group *pci_device_group(struct device *dev);

extern struct iommu_group *generic_device_group(struct device *dev);


struct iommu_fwspec {
	const struct iommu_ops	*ops;
	struct fwnode_handle	*iommu_fwnode;
	void			*iommu_priv;
	unsigned int		num_ids;
	u32			ids[1];
};

int iommu_fwspec_init(struct device *dev, struct fwnode_handle *iommu_fwnode,
		      const struct iommu_ops *ops);
void iommu_fwspec_free(struct device *dev);
int iommu_fwspec_add_ids(struct device *dev, u32 *ids, int num_ids);

#else 

struct iommu_ops {};
struct iommu_group {};
struct iommu_fwspec {};

static inline bool iommu_present(struct bus_type *bus)
{
	return false;
}

static inline bool iommu_capable(struct bus_type *bus, enum iommu_cap cap)
{
	return false;
}

static inline struct iommu_domain *iommu_domain_alloc(struct bus_type *bus)
{
	return NULL;
}

static inline struct iommu_group *iommu_group_get_by_id(int id)
{
	return NULL;
}

static inline void iommu_domain_free(struct iommu_domain *domain)
{
}

static inline int iommu_attach_device(struct iommu_domain *domain,
				      struct device *dev)
{
	return -ENODEV;
}

static inline void iommu_detach_device(struct iommu_domain *domain,
				       struct device *dev)
{
}

static inline struct iommu_domain *iommu_get_domain_for_dev(struct device *dev)
{
	return NULL;
}

static inline int iommu_map(struct iommu_domain *domain, unsigned long iova,
			    phys_addr_t paddr, int gfp_order, int prot)
{
	return -ENODEV;
}

static inline int iommu_unmap(struct iommu_domain *domain, unsigned long iova,
			      int gfp_order)
{
	return -ENODEV;
}

static inline size_t iommu_map_sg(struct iommu_domain *domain,
				  unsigned long iova, struct scatterlist *sg,
				  unsigned int nents, int prot)
{
	return -ENODEV;
}

static inline int iommu_domain_window_enable(struct iommu_domain *domain,
					     u32 wnd_nr, phys_addr_t paddr,
					     u64 size, int prot)
{
	return -ENODEV;
}

static inline void iommu_domain_window_disable(struct iommu_domain *domain,
					       u32 wnd_nr)
{
}

static inline phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, dma_addr_t iova)
{
	return 0;
}

static inline void iommu_set_fault_handler(struct iommu_domain *domain,
				iommu_fault_handler_t handler, void *token)
{
}

static inline void iommu_get_dm_regions(struct device *dev,
					struct list_head *list)
{
}

static inline void iommu_put_dm_regions(struct device *dev,
					struct list_head *list)
{
}

static inline int iommu_request_dm_for_dev(struct device *dev)
{
	return -ENODEV;
}

static inline int iommu_attach_group(struct iommu_domain *domain,
				     struct iommu_group *group)
{
	return -ENODEV;
}

static inline void iommu_detach_group(struct iommu_domain *domain,
				      struct iommu_group *group)
{
}

static inline struct iommu_group *iommu_group_alloc(void)
{
	return ERR_PTR(-ENODEV);
}

static inline void *iommu_group_get_iommudata(struct iommu_group *group)
{
	return NULL;
}

static inline void iommu_group_set_iommudata(struct iommu_group *group,
					     void *iommu_data,
					     void (*release)(void *iommu_data))
{
}

static inline int iommu_group_set_name(struct iommu_group *group,
				       const char *name)
{
	return -ENODEV;
}

static inline int iommu_group_add_device(struct iommu_group *group,
					 struct device *dev)
{
	return -ENODEV;
}

static inline void iommu_group_remove_device(struct device *dev)
{
}

static inline int iommu_group_for_each_dev(struct iommu_group *group,
					   void *data,
					   int (*fn)(struct device *, void *))
{
	return -ENODEV;
}

static inline struct iommu_group *iommu_group_get(struct device *dev)
{
	return NULL;
}

static inline void iommu_group_put(struct iommu_group *group)
{
}

static inline int iommu_group_register_notifier(struct iommu_group *group,
						struct notifier_block *nb)
{
	return -ENODEV;
}

static inline int iommu_group_unregister_notifier(struct iommu_group *group,
						  struct notifier_block *nb)
{
	return 0;
}

static inline int iommu_group_id(struct iommu_group *group)
{
	return -ENODEV;
}

static inline int iommu_domain_get_attr(struct iommu_domain *domain,
					enum iommu_attr attr, void *data)
{
	return -EINVAL;
}

static inline int iommu_domain_set_attr(struct iommu_domain *domain,
					enum iommu_attr attr, void *data)
{
	return -EINVAL;
}

static inline struct device *iommu_device_create(struct device *parent,
					void *drvdata,
					const struct attribute_group **groups,
					const char *fmt, ...)
{
	return ERR_PTR(-ENODEV);
}

static inline void iommu_device_destroy(struct device *dev)
{
}

static inline int iommu_device_link(struct device *dev, struct device *link)
{
	return -EINVAL;
}

static inline void iommu_device_unlink(struct device *dev, struct device *link)
{
}

static inline int iommu_fwspec_init(struct device *dev,
				    struct fwnode_handle *iommu_fwnode,
				    const struct iommu_ops *ops)
{
	return -ENODEV;
}

static inline void iommu_fwspec_free(struct device *dev)
{
}

static inline int iommu_fwspec_add_ids(struct device *dev, u32 *ids,
				       int num_ids)
{
	return -ENODEV;
}

#endif 

#endif 
