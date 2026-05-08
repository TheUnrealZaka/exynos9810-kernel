

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <linux/ioport.h>
#include <linux/kobject.h>
#include <linux/klist.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/pinctrl/devinfo.h>
#include <linux/pm.h>
#include <linux/atomic.h>
#include <linux/ratelimit.h>
#include <linux/uidgid.h>
#include <linux/gfp.h>
#include <asm/device.h>

struct device;
struct device_private;
struct device_driver;
struct driver_private;
struct module;
struct class;
struct subsys_private;
struct bus_type;
struct device_node;
struct fwnode_handle;
struct iommu_ops;
struct iommu_group;
struct iommu_fwspec;

struct bus_attribute {
	struct attribute	attr;
	ssize_t (*show)(struct bus_type *bus, char *buf);
	ssize_t (*store)(struct bus_type *bus, const char *buf, size_t count);
};

#define BUS_ATTR(_name, _mode, _show, _store)	\
	struct bus_attribute bus_attr_##_name = __ATTR(_name, _mode, _show, _store)
#define BUS_ATTR_RW(_name) \
	struct bus_attribute bus_attr_##_name = __ATTR_RW(_name)
#define BUS_ATTR_RO(_name) \
	struct bus_attribute bus_attr_##_name = __ATTR_RO(_name)

extern int __must_check bus_create_file(struct bus_type *,
					struct bus_attribute *);
extern void bus_remove_file(struct bus_type *, struct bus_attribute *);


struct bus_type {
	const char		*name;
	const char		*dev_name;
	struct device		*dev_root;
	struct device_attribute	*dev_attrs;	
	const struct attribute_group **bus_groups;
	const struct attribute_group **dev_groups;
	const struct attribute_group **drv_groups;

	int (*match)(struct device *dev, struct device_driver *drv);
	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown)(struct device *dev);

	int (*online)(struct device *dev);
	int (*offline)(struct device *dev);

	int (*suspend)(struct device *dev, pm_message_t state);
	int (*resume)(struct device *dev);

	const struct dev_pm_ops *pm;

	const struct iommu_ops *iommu_ops;

	struct subsys_private *p;
	struct lock_class_key lock_key;
};

extern int __must_check bus_register(struct bus_type *bus);

extern void bus_unregister(struct bus_type *bus);

extern int __must_check bus_rescan_devices(struct bus_type *bus);


struct subsys_dev_iter {
	struct klist_iter		ki;
	const struct device_type	*type;
};
void subsys_dev_iter_init(struct subsys_dev_iter *iter,
			 struct bus_type *subsys,
			 struct device *start,
			 const struct device_type *type);
struct device *subsys_dev_iter_next(struct subsys_dev_iter *iter);
void subsys_dev_iter_exit(struct subsys_dev_iter *iter);

int bus_for_each_dev(struct bus_type *bus, struct device *start, void *data,
		     int (*fn)(struct device *dev, void *data));
struct device *bus_find_device(struct bus_type *bus, struct device *start,
			       void *data,
			       int (*match)(struct device *dev, void *data));
struct device *bus_find_device_by_name(struct bus_type *bus,
				       struct device *start,
				       const char *name);
struct device *subsys_find_device_by_id(struct bus_type *bus, unsigned int id,
					struct device *hint);
int bus_for_each_drv(struct bus_type *bus, struct device_driver *start,
		     void *data, int (*fn)(struct device_driver *, void *));
void bus_sort_breadthfirst(struct bus_type *bus,
			   int (*compare)(const struct device *a,
					  const struct device *b));

struct notifier_block;

extern int bus_register_notifier(struct bus_type *bus,
				 struct notifier_block *nb);
extern int bus_unregister_notifier(struct bus_type *bus,
				   struct notifier_block *nb);


#define BUS_NOTIFY_ADD_DEVICE		0x00000001 
#define BUS_NOTIFY_DEL_DEVICE		0x00000002 
#define BUS_NOTIFY_REMOVED_DEVICE	0x00000003 
#define BUS_NOTIFY_BIND_DRIVER		0x00000004 
#define BUS_NOTIFY_BOUND_DRIVER		0x00000005 
#define BUS_NOTIFY_UNBIND_DRIVER	0x00000006 
#define BUS_NOTIFY_UNBOUND_DRIVER	0x00000007 
#define BUS_NOTIFY_DRIVER_NOT_BOUND	0x00000008 

extern struct kset *bus_get_kset(struct bus_type *bus);
extern struct klist *bus_get_device_klist(struct bus_type *bus);


enum probe_type {
	PROBE_DEFAULT_STRATEGY,
	PROBE_PREFER_ASYNCHRONOUS,
	PROBE_FORCE_SYNCHRONOUS,
};


struct device_driver {
	const char		*name;
	struct bus_type		*bus;

	struct module		*owner;
	const char		*mod_name;	

	bool suppress_bind_attrs;	
	enum probe_type probe_type;

	const struct of_device_id	*of_match_table;
	const struct acpi_device_id	*acpi_match_table;

	int (*probe) (struct device *dev);
	int (*remove) (struct device *dev);
	void (*shutdown) (struct device *dev);
	int (*suspend) (struct device *dev, pm_message_t state);
	int (*resume) (struct device *dev);
	const struct attribute_group **groups;

	const struct dev_pm_ops *pm;

	struct driver_private *p;
};


extern int __must_check driver_register(struct device_driver *drv);
extern void driver_unregister(struct device_driver *drv);

extern struct device_driver *driver_find(const char *name,
					 struct bus_type *bus);
extern int driver_probe_done(void);
extern void wait_for_device_probe(void);




struct driver_attribute {
	struct attribute attr;
	ssize_t (*show)(struct device_driver *driver, char *buf);
	ssize_t (*store)(struct device_driver *driver, const char *buf,
			 size_t count);
};

#define DRIVER_ATTR(_name, _mode, _show, _store) \
	struct driver_attribute driver_attr_##_name = __ATTR(_name, _mode, _show, _store)
#define DRIVER_ATTR_RW(_name) \
	struct driver_attribute driver_attr_##_name = __ATTR_RW(_name)
#define DRIVER_ATTR_RO(_name) \
	struct driver_attribute driver_attr_##_name = __ATTR_RO(_name)
#define DRIVER_ATTR_WO(_name) \
	struct driver_attribute driver_attr_##_name = __ATTR_WO(_name)

extern int __must_check driver_create_file(struct device_driver *driver,
					const struct driver_attribute *attr);
extern void driver_remove_file(struct device_driver *driver,
			       const struct driver_attribute *attr);

extern int __must_check driver_for_each_device(struct device_driver *drv,
					       struct device *start,
					       void *data,
					       int (*fn)(struct device *dev,
							 void *));
struct device *driver_find_device(struct device_driver *drv,
				  struct device *start, void *data,
				  int (*match)(struct device *dev, void *data));


struct subsys_interface {
	const char *name;
	struct bus_type *subsys;
	struct list_head node;
	int (*add_dev)(struct device *dev, struct subsys_interface *sif);
	void (*remove_dev)(struct device *dev, struct subsys_interface *sif);
};

int subsys_interface_register(struct subsys_interface *sif);
void subsys_interface_unregister(struct subsys_interface *sif);

int subsys_system_register(struct bus_type *subsys,
			   const struct attribute_group **groups);
int subsys_virtual_register(struct bus_type *subsys,
			    const struct attribute_group **groups);


struct class {
	const char		*name;
	struct module		*owner;

	struct class_attribute		*class_attrs;
	const struct attribute_group	**dev_groups;
	struct kobject			*dev_kobj;

	int (*dev_uevent)(struct device *dev, struct kobj_uevent_env *env);
	char *(*devnode)(struct device *dev, umode_t *mode);

	void (*class_release)(struct class *class);
	void (*dev_release)(struct device *dev);

	int (*suspend)(struct device *dev, pm_message_t state);
	int (*resume)(struct device *dev);
	int (*shutdown)(struct device *dev);

	const struct kobj_ns_type_operations *ns_type;
	const void *(*namespace)(struct device *dev);

	const struct dev_pm_ops *pm;

	struct subsys_private *p;
};

struct class_dev_iter {
	struct klist_iter		ki;
	const struct device_type	*type;
};

extern struct kobject *sysfs_dev_block_kobj;
extern struct kobject *sysfs_dev_char_kobj;
extern int __must_check __class_register(struct class *class,
					 struct lock_class_key *key);
extern void class_unregister(struct class *class);


#define class_register(class)			\
({						\
	static struct lock_class_key __key;	\
	__class_register(class, &__key);	\
})

struct class_compat;
struct class_compat *class_compat_register(const char *name);
void class_compat_unregister(struct class_compat *cls);
int class_compat_create_link(struct class_compat *cls, struct device *dev,
			     struct device *device_link);
void class_compat_remove_link(struct class_compat *cls, struct device *dev,
			      struct device *device_link);

extern void class_dev_iter_init(struct class_dev_iter *iter,
				struct class *class,
				struct device *start,
				const struct device_type *type);
extern struct device *class_dev_iter_next(struct class_dev_iter *iter);
extern void class_dev_iter_exit(struct class_dev_iter *iter);

extern int class_for_each_device(struct class *class, struct device *start,
				 void *data,
				 int (*fn)(struct device *dev, void *data));
extern struct device *class_find_device(struct class *class,
					struct device *start, const void *data,
					int (*match)(struct device *, const void *));

struct class_attribute {
	struct attribute attr;
	ssize_t (*show)(struct class *class, struct class_attribute *attr,
			char *buf);
	ssize_t (*store)(struct class *class, struct class_attribute *attr,
			const char *buf, size_t count);
};

#define CLASS_ATTR(_name, _mode, _show, _store) \
	struct class_attribute class_attr_##_name = __ATTR(_name, _mode, _show, _store)
#define CLASS_ATTR_RW(_name) \
	struct class_attribute class_attr_##_name = __ATTR_RW(_name)
#define CLASS_ATTR_RO(_name) \
	struct class_attribute class_attr_##_name = __ATTR_RO(_name)

extern int __must_check class_create_file_ns(struct class *class,
					     const struct class_attribute *attr,
					     const void *ns);
extern void class_remove_file_ns(struct class *class,
				 const struct class_attribute *attr,
				 const void *ns);

static inline int __must_check class_create_file(struct class *class,
					const struct class_attribute *attr)
{
	return class_create_file_ns(class, attr, NULL);
}

static inline void class_remove_file(struct class *class,
				     const struct class_attribute *attr)
{
	return class_remove_file_ns(class, attr, NULL);
}


struct class_attribute_string {
	struct class_attribute attr;
	char *str;
};


#define _CLASS_ATTR_STRING(_name, _mode, _str) \
	{ __ATTR(_name, _mode, show_class_attr_string, NULL), _str }
#define CLASS_ATTR_STRING(_name, _mode, _str) \
	struct class_attribute_string class_attr_##_name = \
		_CLASS_ATTR_STRING(_name, _mode, _str)

extern ssize_t show_class_attr_string(struct class *class, struct class_attribute *attr,
                        char *buf);

struct class_interface {
	struct list_head	node;
	struct class		*class;

	int (*add_dev)		(struct device *, struct class_interface *);
	void (*remove_dev)	(struct device *, struct class_interface *);
};

extern int __must_check class_interface_register(struct class_interface *);
extern void class_interface_unregister(struct class_interface *);

extern struct class * __must_check __class_create(struct module *owner,
						  const char *name,
						  struct lock_class_key *key);
extern void class_destroy(struct class *cls);


#define class_create(owner, name)		\
({						\
	static struct lock_class_key __key;	\
	__class_create(owner, name, &__key);	\
})


struct device_type {
	const char *name;
	const struct attribute_group **groups;
	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
	char *(*devnode)(struct device *dev, umode_t *mode,
			 kuid_t *uid, kgid_t *gid);
	void (*release)(struct device *dev);

	const struct dev_pm_ops *pm;
};


struct device_attribute {
	struct attribute	attr;
	ssize_t (*show)(struct device *dev, struct device_attribute *attr,
			char *buf);
	ssize_t (*store)(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);
};

struct dev_ext_attribute {
	struct device_attribute attr;
	void *var;
};

ssize_t device_show_ulong(struct device *dev, struct device_attribute *attr,
			  char *buf);
ssize_t device_store_ulong(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count);
ssize_t device_show_int(struct device *dev, struct device_attribute *attr,
			char *buf);
ssize_t device_store_int(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);
ssize_t device_show_bool(struct device *dev, struct device_attribute *attr,
			char *buf);
ssize_t device_store_bool(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);

#define DEVICE_ATTR(_name, _mode, _show, _store) \
	struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)
#define DEVICE_ATTR_RW(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RW(_name)
#define DEVICE_ATTR_RO(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RO(_name)
#define DEVICE_ATTR_WO(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_WO(_name)
#define DEVICE_ULONG_ATTR(_name, _mode, _var) \
	struct dev_ext_attribute dev_attr_##_name = \
		{ __ATTR(_name, _mode, device_show_ulong, device_store_ulong), &(_var) }
#define DEVICE_INT_ATTR(_name, _mode, _var) \
	struct dev_ext_attribute dev_attr_##_name = \
		{ __ATTR(_name, _mode, device_show_int, device_store_int), &(_var) }
#define DEVICE_BOOL_ATTR(_name, _mode, _var) \
	struct dev_ext_attribute dev_attr_##_name = \
		{ __ATTR(_name, _mode, device_show_bool, device_store_bool), &(_var) }
#define DEVICE_ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store) \
	struct device_attribute dev_attr_##_name =		\
		__ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store)

extern int device_create_file(struct device *device,
			      const struct device_attribute *entry);
extern void device_remove_file(struct device *dev,
			       const struct device_attribute *attr);
extern bool device_remove_file_self(struct device *dev,
				    const struct device_attribute *attr);
extern int __must_check device_create_bin_file(struct device *dev,
					const struct bin_attribute *attr);
extern void device_remove_bin_file(struct device *dev,
				   const struct bin_attribute *attr);


typedef void (*dr_release_t)(struct device *dev, void *res);
typedef int (*dr_match_t)(struct device *dev, void *res, void *match_data);

#ifdef CONFIG_DEBUG_DEVRES
extern void *__devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp,
				 int nid, const char *name) __malloc;
#define devres_alloc(release, size, gfp) \
	__devres_alloc_node(release, size, gfp, NUMA_NO_NODE, #release)
#define devres_alloc_node(release, size, gfp, nid) \
	__devres_alloc_node(release, size, gfp, nid, #release)
#else
extern void *devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp,
			       int nid) __malloc;
static inline void *devres_alloc(dr_release_t release, size_t size, gfp_t gfp)
{
	return devres_alloc_node(release, size, gfp, NUMA_NO_NODE);
}
#endif

extern void devres_for_each_res(struct device *dev, dr_release_t release,
				dr_match_t match, void *match_data,
				void (*fn)(struct device *, void *, void *),
				void *data);
extern void devres_free(void *res);
extern void devres_add(struct device *dev, void *res);
extern void *devres_find(struct device *dev, dr_release_t release,
			 dr_match_t match, void *match_data);
extern void *devres_get(struct device *dev, void *new_res,
			dr_match_t match, void *match_data);
extern void *devres_remove(struct device *dev, dr_release_t release,
			   dr_match_t match, void *match_data);
extern int devres_destroy(struct device *dev, dr_release_t release,
			  dr_match_t match, void *match_data);
extern int devres_release(struct device *dev, dr_release_t release,
			  dr_match_t match, void *match_data);


extern void * __must_check devres_open_group(struct device *dev, void *id,
					     gfp_t gfp);
extern void devres_close_group(struct device *dev, void *id);
extern void devres_remove_group(struct device *dev, void *id);
extern int devres_release_group(struct device *dev, void *id);


extern void *devm_kmalloc(struct device *dev, size_t size, gfp_t gfp) __malloc;
extern __printf(3, 0)
char *devm_kvasprintf(struct device *dev, gfp_t gfp, const char *fmt,
		      va_list ap) __malloc;
extern __printf(3, 4)
char *devm_kasprintf(struct device *dev, gfp_t gfp, const char *fmt, ...) __malloc;
static inline void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
	return devm_kmalloc(dev, size, gfp | __GFP_ZERO);
}
static inline void *devm_kmalloc_array(struct device *dev,
				       size_t n, size_t size, gfp_t flags)
{
	if (size != 0 && n > SIZE_MAX / size)
		return NULL;
	return devm_kmalloc(dev, n * size, flags);
}
static inline void *devm_kcalloc(struct device *dev,
				 size_t n, size_t size, gfp_t flags)
{
	return devm_kmalloc_array(dev, n, size, flags | __GFP_ZERO);
}
extern void devm_kfree(struct device *dev, void *p);
extern char *devm_kstrdup(struct device *dev, const char *s, gfp_t gfp) __malloc;
extern void *devm_kmemdup(struct device *dev, const void *src, size_t len,
			  gfp_t gfp);

extern unsigned long devm_get_free_pages(struct device *dev,
					 gfp_t gfp_mask, unsigned int order);
extern void devm_free_pages(struct device *dev, unsigned long addr);

void __iomem *devm_ioremap_resource(struct device *dev,
				    const struct resource *res);


int devm_add_action(struct device *dev, void (*action)(void *), void *data);
void devm_remove_action(struct device *dev, void (*action)(void *), void *data);

static inline int devm_add_action_or_reset(struct device *dev,
					   void (*action)(void *), void *data)
{
	int ret;

	ret = devm_add_action(dev, action, data);
	if (ret)
		action(data);

	return ret;
}

struct device_dma_parameters {
	
	unsigned int max_segment_size;
	unsigned long segment_boundary_mask;
};


struct device {
	struct device		*parent;

	struct device_private	*p;

	struct kobject kobj;
	const char		*init_name; 
	const struct device_type *type;

	struct mutex		mutex;	

	struct bus_type	*bus;		
	struct device_driver *driver;	
	void		*platform_data;	
	void		*driver_data;	
	struct dev_pm_info	power;
	struct dev_pm_domain	*pm_domain;

#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
	struct irq_domain	*msi_domain;
#endif
#ifdef CONFIG_PINCTRL
	struct dev_pin_info	*pins;
#endif
#ifdef CONFIG_GENERIC_MSI_IRQ
	raw_spinlock_t		msi_lock;
	struct list_head	msi_list;
#endif

#ifdef CONFIG_NUMA
	int		numa_node;	
#endif
	u64		*dma_mask;	
	u64		coherent_dma_mask;
	unsigned long	dma_pfn_offset;

	struct device_dma_parameters *dma_parms;

	struct list_head	dma_pools;	

	struct dma_coherent_mem	*dma_mem; 
#ifdef CONFIG_DMA_CMA
	struct cma *cma_area;		
#endif
	
	struct dev_archdata	archdata;

	struct device_node	*of_node; 
	struct fwnode_handle	*fwnode; 

	dev_t			devt;	
	u32			id;	

	spinlock_t		devres_lock;
	struct list_head	devres_head;

	struct klist_node	knode_class;
	struct class		*class;
	const struct attribute_group **groups;	

	void	(*release)(struct device *dev);
	struct iommu_group	*iommu_group;
	struct iommu_fwspec	*iommu_fwspec;

	bool			offline_disabled:1;
	bool			offline:1;
};

static inline struct device *kobj_to_dev(struct kobject *kobj)
{
	return container_of(kobj, struct device, kobj);
}


#include <linux/pm_wakeup.h>

static inline const char *dev_name(const struct device *dev)
{
	
	if (dev->init_name)
		return dev->init_name;

	return kobject_name(&dev->kobj);
}

extern __printf(2, 3)
int dev_set_name(struct device *dev, const char *name, ...);

#ifdef CONFIG_NUMA
static inline int dev_to_node(struct device *dev)
{
	return dev->numa_node;
}
static inline void set_dev_node(struct device *dev, int node)
{
	dev->numa_node = node;
}
#else
static inline int dev_to_node(struct device *dev)
{
	return -1;
}
static inline void set_dev_node(struct device *dev, int node)
{
}
#endif

static inline struct irq_domain *dev_get_msi_domain(const struct device *dev)
{
#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
	return dev->msi_domain;
#else
	return NULL;
#endif
}

static inline void dev_set_msi_domain(struct device *dev, struct irq_domain *d)
{
#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
	dev->msi_domain = d;
#endif
}

static inline void *dev_get_drvdata(const struct device *dev)
{
	return dev->driver_data;
}

static inline void dev_set_drvdata(struct device *dev, void *data)
{
	dev->driver_data = data;
}

static inline struct pm_subsys_data *dev_to_psd(struct device *dev)
{
	return dev ? dev->power.subsys_data : NULL;
}

static inline unsigned int dev_get_uevent_suppress(const struct device *dev)
{
	return dev->kobj.uevent_suppress;
}

static inline void dev_set_uevent_suppress(struct device *dev, int val)
{
	dev->kobj.uevent_suppress = val;
}

static inline int device_is_registered(struct device *dev)
{
	return dev->kobj.state_in_sysfs;
}

static inline void device_enable_async_suspend(struct device *dev)
{
	if (!dev->power.is_prepared)
		dev->power.async_suspend = true;
}

static inline void device_disable_async_suspend(struct device *dev)
{
	if (!dev->power.is_prepared)
		dev->power.async_suspend = false;
}

static inline bool device_async_suspend_enabled(struct device *dev)
{
	return !!dev->power.async_suspend;
}

static inline void dev_pm_syscore_device(struct device *dev, bool val)
{
#ifdef CONFIG_PM_SLEEP
	dev->power.syscore = val;
#endif
}

static inline void device_lock(struct device *dev)
{
	mutex_lock(&dev->mutex);
}

static inline int device_lock_interruptible(struct device *dev)
{
	return mutex_lock_interruptible(&dev->mutex);
}

static inline int device_trylock(struct device *dev)
{
	return mutex_trylock(&dev->mutex);
}

static inline void device_unlock(struct device *dev)
{
	mutex_unlock(&dev->mutex);
}

static inline void device_lock_assert(struct device *dev)
{
	lockdep_assert_held(&dev->mutex);
}

static inline struct device_node *dev_of_node(struct device *dev)
{
	if (!IS_ENABLED(CONFIG_OF))
		return NULL;
	return dev->of_node;
}

void driver_init(void);


extern int __must_check device_register(struct device *dev);
extern void device_unregister(struct device *dev);
extern void device_initialize(struct device *dev);
extern int __must_check device_add(struct device *dev);
extern void device_del(struct device *dev);
extern int device_for_each_child(struct device *dev, void *data,
		     int (*fn)(struct device *dev, void *data));
extern int device_for_each_child_reverse(struct device *dev, void *data,
		     int (*fn)(struct device *dev, void *data));
extern struct device *device_find_child(struct device *dev, void *data,
				int (*match)(struct device *dev, void *data));
extern int device_rename(struct device *dev, const char *new_name);
extern int device_move(struct device *dev, struct device *new_parent,
		       enum dpm_order dpm_order);
extern const char *device_get_devnode(struct device *dev,
				      umode_t *mode, kuid_t *uid, kgid_t *gid,
				      const char **tmp);

static inline bool device_supports_offline(struct device *dev)
{
	return dev->bus && dev->bus->offline && dev->bus->online;
}

extern void lock_device_hotplug(void);
extern void unlock_device_hotplug(void);
extern int lock_device_hotplug_sysfs(void);
extern int device_offline(struct device *dev);
extern int device_online(struct device *dev);
extern void set_primary_fwnode(struct device *dev, struct fwnode_handle *fwnode);
extern void set_secondary_fwnode(struct device *dev, struct fwnode_handle *fwnode);


extern struct device *__root_device_register(const char *name,
					     struct module *owner);


#define root_device_register(name) \
	__root_device_register(name, THIS_MODULE)

extern void root_device_unregister(struct device *root);

static inline void *dev_get_platdata(const struct device *dev)
{
	return dev->platform_data;
}


extern int __must_check device_bind_driver(struct device *dev);
extern void device_release_driver(struct device *dev);
extern int  __must_check device_attach(struct device *dev);
extern int __must_check driver_attach(struct device_driver *drv);
extern void device_initial_probe(struct device *dev);
extern int __must_check device_reprobe(struct device *dev);

extern bool device_is_bound(struct device *dev);


extern __printf(5, 0)
struct device *device_create_vargs(struct class *cls, struct device *parent,
				   dev_t devt, void *drvdata,
				   const char *fmt, va_list vargs);
extern __printf(5, 6)
struct device *device_create(struct class *cls, struct device *parent,
			     dev_t devt, void *drvdata,
			     const char *fmt, ...);
extern __printf(6, 7)
struct device *device_create_with_groups(struct class *cls,
			     struct device *parent, dev_t devt, void *drvdata,
			     const struct attribute_group **groups,
			     const char *fmt, ...);
extern void device_destroy(struct class *cls, dev_t devt);



extern int (*platform_notify)(struct device *dev);

extern int (*platform_notify_remove)(struct device *dev);



extern struct device *get_device(struct device *dev);
extern void put_device(struct device *dev);

#ifdef CONFIG_DEVTMPFS
extern int devtmpfs_create_node(struct device *dev);
extern int devtmpfs_delete_node(struct device *dev);
extern int devtmpfs_mount(const char *mntdir);
#else
static inline int devtmpfs_create_node(struct device *dev) { return 0; }
static inline int devtmpfs_delete_node(struct device *dev) { return 0; }
static inline int devtmpfs_mount(const char *mountpoint) { return 0; }
#endif


extern void device_shutdown(void);


extern const char *dev_driver_string(const struct device *dev);


#ifdef CONFIG_PRINTK

extern __printf(3, 0)
int dev_vprintk_emit(int level, const struct device *dev,
		     const char *fmt, va_list args);
extern __printf(3, 4)
int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...);

extern __printf(3, 4)
void dev_printk(const char *level, const struct device *dev,
		const char *fmt, ...);
extern __printf(2, 3)
void dev_emerg(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void dev_alert(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void dev_crit(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void dev_err(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void dev_warn(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void dev_notice(const struct device *dev, const char *fmt, ...);
extern __printf(2, 3)
void _dev_info(const struct device *dev, const char *fmt, ...);

#else

static inline __printf(3, 0)
int dev_vprintk_emit(int level, const struct device *dev,
		     const char *fmt, va_list args)
{ return 0; }
static inline __printf(3, 4)
int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...)
{ return 0; }

static inline void __dev_printk(const char *level, const struct device *dev,
				struct va_format *vaf)
{}
static inline __printf(3, 4)
void dev_printk(const char *level, const struct device *dev,
		const char *fmt, ...)
{}

static inline __printf(2, 3)
void dev_emerg(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void dev_crit(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void dev_alert(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void dev_err(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void dev_warn(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void dev_notice(const struct device *dev, const char *fmt, ...)
{}
static inline __printf(2, 3)
void _dev_info(const struct device *dev, const char *fmt, ...)
{}

#endif



#define dev_info(dev, fmt, arg...) _dev_info(dev, fmt, ##arg)

#if defined(CONFIG_DYNAMIC_DEBUG)
#define dev_dbg(dev, format, ...)		     \
do {						     \
	dynamic_dev_dbg(dev, format, ##__VA_ARGS__); \
} while (0)
#elif defined(DEBUG)
#define dev_dbg(dev, format, arg...)		\
	dev_printk(KERN_DEBUG, dev, format, ##arg)
#else
#define dev_dbg(dev, format, arg...)				\
({								\
	if (0)							\
		dev_printk(KERN_DEBUG, dev, format, ##arg);	\
})
#endif

#ifdef CONFIG_PRINTK
#define dev_level_once(dev_level, dev, fmt, ...)			\
do {									\
	static bool __print_once __read_mostly;				\
									\
	if (!__print_once) {						\
		__print_once = true;					\
		dev_level(dev, fmt, ##__VA_ARGS__);			\
	}								\
} while (0)
#else
#define dev_level_once(dev_level, dev, fmt, ...)			\
do {									\
	if (0)								\
		dev_level(dev, fmt, ##__VA_ARGS__);			\
} while (0)
#endif

#define dev_emerg_once(dev, fmt, ...)					\
	dev_level_once(dev_emerg, dev, fmt, ##__VA_ARGS__)
#define dev_alert_once(dev, fmt, ...)					\
	dev_level_once(dev_alert, dev, fmt, ##__VA_ARGS__)
#define dev_crit_once(dev, fmt, ...)					\
	dev_level_once(dev_crit, dev, fmt, ##__VA_ARGS__)
#define dev_err_once(dev, fmt, ...)					\
	dev_level_once(dev_err, dev, fmt, ##__VA_ARGS__)
#define dev_warn_once(dev, fmt, ...)					\
	dev_level_once(dev_warn, dev, fmt, ##__VA_ARGS__)
#define dev_notice_once(dev, fmt, ...)					\
	dev_level_once(dev_notice, dev, fmt, ##__VA_ARGS__)
#define dev_info_once(dev, fmt, ...)					\
	dev_level_once(dev_info, dev, fmt, ##__VA_ARGS__)
#define dev_dbg_once(dev, fmt, ...)					\
	dev_level_once(dev_dbg, dev, fmt, ##__VA_ARGS__)

#define dev_level_ratelimited(dev_level, dev, fmt, ...)			\
do {									\
	static DEFINE_RATELIMIT_STATE(_rs,				\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);		\
	if (__ratelimit(&_rs))						\
		dev_level(dev, fmt, ##__VA_ARGS__);			\
} while (0)

#define dev_emerg_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_emerg, dev, fmt, ##__VA_ARGS__)
#define dev_alert_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_alert, dev, fmt, ##__VA_ARGS__)
#define dev_crit_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_crit, dev, fmt, ##__VA_ARGS__)
#define dev_err_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_err, dev, fmt, ##__VA_ARGS__)
#define dev_warn_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_warn, dev, fmt, ##__VA_ARGS__)
#define dev_notice_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_notice, dev, fmt, ##__VA_ARGS__)
#define dev_info_ratelimited(dev, fmt, ...)				\
	dev_level_ratelimited(dev_info, dev, fmt, ##__VA_ARGS__)
#if defined(CONFIG_DYNAMIC_DEBUG)

#define dev_dbg_ratelimited(dev, fmt, ...)				\
do {									\
	static DEFINE_RATELIMIT_STATE(_rs,				\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);		\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);			\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT) &&	\
	    __ratelimit(&_rs))						\
		__dynamic_dev_dbg(&descriptor, dev, fmt,		\
				  ##__VA_ARGS__);			\
} while (0)
#elif defined(DEBUG)
#define dev_dbg_ratelimited(dev, fmt, ...)				\
do {									\
	static DEFINE_RATELIMIT_STATE(_rs,				\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);		\
	if (__ratelimit(&_rs))						\
		dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__);	\
} while (0)
#else
#define dev_dbg_ratelimited(dev, fmt, ...)				\
do {									\
	if (0)								\
		dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__);	\
} while (0)
#endif

#ifdef VERBOSE_DEBUG
#define dev_vdbg	dev_dbg
#else
#define dev_vdbg(dev, format, arg...)				\
({								\
	if (0)							\
		dev_printk(KERN_DEBUG, dev, format, ##arg);	\
})
#endif


#define dev_WARN(dev, format, arg...) \
	WARN(1, "%s %s: " format, dev_driver_string(dev), dev_name(dev), ## arg);

#define dev_WARN_ONCE(dev, condition, format, arg...) \
	WARN_ONCE(condition, "%s %s: " format, \
			dev_driver_string(dev), dev_name(dev), ## arg)


#define MODULE_ALIAS_CHARDEV(major,minor) \
	MODULE_ALIAS("char-major-" __stringify(major) "-" __stringify(minor))
#define MODULE_ALIAS_CHARDEV_MAJOR(major) \
	MODULE_ALIAS("char-major-" __stringify(major) "-*")

#ifdef CONFIG_SYSFS_DEPRECATED
extern long sysfs_deprecated;
#else
#define sysfs_deprecated 0
#endif


#define module_driver(__driver, __register, __unregister, ...) \
static int __init __driver##_init(void) \
{ \
	return __register(&(__driver) , ##__VA_ARGS__); \
} \
module_init(__driver##_init); \
static void __exit __driver##_exit(void) \
{ \
	__unregister(&(__driver) , ##__VA_ARGS__); \
} \
module_exit(__driver##_exit);


#define builtin_driver(__driver, __register, ...) \
static int __init __driver##_init(void) \
{ \
	return __register(&(__driver) , ##__VA_ARGS__); \
} \
device_initcall(__driver##_init);

#endif 
