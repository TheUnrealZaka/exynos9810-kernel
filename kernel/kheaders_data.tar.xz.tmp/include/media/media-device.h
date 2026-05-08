

#ifndef _MEDIA_DEVICE_H
#define _MEDIA_DEVICE_H

#include <linux/list.h>
#include <linux/mutex.h>

#include <media/media-devnode.h>
#include <media/media-entity.h>

struct ida;
struct device;


struct media_entity_notify {
	struct list_head list;
	void *notify_data;
	void (*notify)(struct media_entity *entity, void *notify_data);
};


struct media_device_ops {
	int (*link_notify)(struct media_link *link, u32 flags,
			   unsigned int notification);
};


struct media_device {
	
	struct device *dev;
	struct media_devnode *devnode;

	char model[32];
	char driver_name[32];
	char serial[40];
	char bus_info[32];
	u32 hw_revision;
	u32 driver_version;

	u64 topology_version;

	u32 id;
	struct ida entity_internal_idx;
	int entity_internal_idx_max;

	struct list_head entities;
	struct list_head interfaces;
	struct list_head pads;
	struct list_head links;

	
	struct list_head entity_notify;

	
	struct mutex graph_mutex;
	struct media_entity_graph pm_count_walk;

	void *source_priv;
	int (*enable_source)(struct media_entity *entity,
			     struct media_pipeline *pipe);
	void (*disable_source)(struct media_entity *entity);

	const struct media_device_ops *ops;
};


struct pci_dev;
struct usb_device;

#ifdef CONFIG_MEDIA_CONTROLLER


#define MEDIA_DEV_NOTIFY_PRE_LINK_CH	0
#define MEDIA_DEV_NOTIFY_POST_LINK_CH	1


static inline __must_check int media_entity_enum_init(
	struct media_entity_enum *ent_enum, struct media_device *mdev)
{
	return __media_entity_enum_init(ent_enum,
					mdev->entity_internal_idx_max + 1);
}


void media_device_init(struct media_device *mdev);


void media_device_cleanup(struct media_device *mdev);


int __must_check __media_device_register(struct media_device *mdev,
					 struct module *owner);



#define media_device_register(mdev) __media_device_register(mdev, THIS_MODULE)


void media_device_unregister(struct media_device *mdev);


int __must_check media_device_register_entity(struct media_device *mdev,
					      struct media_entity *entity);


void media_device_unregister_entity(struct media_entity *entity);



int __must_check media_device_register_entity_notify(struct media_device *mdev,
					struct media_entity_notify *nptr);


void media_device_unregister_entity_notify(struct media_device *mdev,
					struct media_entity_notify *nptr);


struct media_device *media_device_get_devres(struct device *dev);


struct media_device *media_device_find_devres(struct device *dev);


#define media_device_for_each_entity(entity, mdev)			\
	list_for_each_entry(entity, &(mdev)->entities, graph_obj.list)


#define media_device_for_each_intf(intf, mdev)			\
	list_for_each_entry(intf, &(mdev)->interfaces, graph_obj.list)


#define media_device_for_each_pad(pad, mdev)			\
	list_for_each_entry(pad, &(mdev)->pads, graph_obj.list)


#define media_device_for_each_link(link, mdev)			\
	list_for_each_entry(link, &(mdev)->links, graph_obj.list)


void media_device_pci_init(struct media_device *mdev,
			   struct pci_dev *pci_dev,
			   const char *name);

void __media_device_usb_init(struct media_device *mdev,
			     struct usb_device *udev,
			     const char *board_name,
			     const char *driver_name);

#else
static inline int media_device_register(struct media_device *mdev)
{
	return 0;
}
static inline void media_device_unregister(struct media_device *mdev)
{
}
static inline int media_device_register_entity(struct media_device *mdev,
						struct media_entity *entity)
{
	return 0;
}
static inline void media_device_unregister_entity(struct media_entity *entity)
{
}
static inline int media_device_register_entity_notify(
					struct media_device *mdev,
					struct media_entity_notify *nptr)
{
	return 0;
}
static inline void media_device_unregister_entity_notify(
					struct media_device *mdev,
					struct media_entity_notify *nptr)
{
}
static inline struct media_device *media_device_get_devres(struct device *dev)
{
	return NULL;
}
static inline struct media_device *media_device_find_devres(struct device *dev)
{
	return NULL;
}

static inline void media_device_pci_init(struct media_device *mdev,
					 struct pci_dev *pci_dev,
					 char *name)
{
}

static inline void __media_device_usb_init(struct media_device *mdev,
					   struct usb_device *udev,
					   char *board_name,
					   char *driver_name)
{
}

#endif 


#define media_device_usb_init(mdev, udev, name) \
	__media_device_usb_init(mdev, udev, name, KBUILD_MODNAME)

#endif
