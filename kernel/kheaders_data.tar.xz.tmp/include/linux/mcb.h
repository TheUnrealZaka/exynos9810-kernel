
#ifndef _LINUX_MCB_H
#define _LINUX_MCB_H

#include <linux/mod_devicetable.h>
#include <linux/device.h>
#include <linux/irqreturn.h>

#define CHAMELEON_FILENAME_LEN 12

struct mcb_driver;
struct mcb_device;


struct mcb_bus {
	struct device dev;
	struct device *carrier;
	int bus_nr;
	u8 revision;
	char model;
	u8 minor;
	char name[CHAMELEON_FILENAME_LEN + 1];
	int (*get_irq)(struct mcb_device *dev);
};

static inline struct mcb_bus *to_mcb_bus(struct device *dev)
{
	return container_of(dev, struct mcb_bus, dev);
}


struct mcb_device {
	struct device dev;
	struct mcb_bus *bus;
	bool is_added;
	struct mcb_driver *driver;
	u16 id;
	int inst;
	int group;
	int var;
	int bar;
	int rev;
	struct resource irq;
	struct resource mem;
	struct device *dma_dev;
};

static inline struct mcb_device *to_mcb_device(struct device *dev)
{
	return container_of(dev, struct mcb_device, dev);
}


struct mcb_driver {
	struct device_driver driver;
	const struct mcb_device_id *id_table;
	int (*probe)(struct mcb_device *mdev, const struct mcb_device_id *id);
	void (*remove)(struct mcb_device *mdev);
	void (*shutdown)(struct mcb_device *mdev);
};

static inline struct mcb_driver *to_mcb_driver(struct device_driver *drv)
{
	return container_of(drv, struct mcb_driver, driver);
}

static inline void *mcb_get_drvdata(struct mcb_device *dev)
{
	return dev_get_drvdata(&dev->dev);
}

static inline void mcb_set_drvdata(struct mcb_device *dev, void *data)
{
	dev_set_drvdata(&dev->dev, data);
}

extern int __must_check __mcb_register_driver(struct mcb_driver *drv,
					struct module *owner,
					const char *mod_name);
#define mcb_register_driver(driver)		\
	__mcb_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)
extern void mcb_unregister_driver(struct mcb_driver *driver);
#define module_mcb_driver(__mcb_driver)		\
	module_driver(__mcb_driver, mcb_register_driver, mcb_unregister_driver);
extern void mcb_bus_add_devices(const struct mcb_bus *bus);
extern int mcb_device_register(struct mcb_bus *bus, struct mcb_device *dev);
extern struct mcb_bus *mcb_alloc_bus(struct device *carrier);
extern struct mcb_bus *mcb_bus_get(struct mcb_bus *bus);
extern void mcb_bus_put(struct mcb_bus *bus);
extern struct mcb_device *mcb_alloc_dev(struct mcb_bus *bus);
extern void mcb_free_dev(struct mcb_device *dev);
extern void mcb_release_bus(struct mcb_bus *bus);
extern struct resource *mcb_request_mem(struct mcb_device *dev,
					const char *name);
extern void mcb_release_mem(struct resource *mem);
extern int mcb_get_irq(struct mcb_device *dev);

#endif 
