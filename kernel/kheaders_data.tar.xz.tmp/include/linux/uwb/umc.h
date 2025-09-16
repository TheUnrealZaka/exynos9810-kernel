

#ifndef _LINUX_UWB_UMC_H_
#define _LINUX_UWB_UMC_H_

#include <linux/device.h>
#include <linux/pci.h>


#define UMC_CAP_ID_WHCI_RC      0x00 
#define UMC_CAP_ID_WHCI_WUSB_HC 0x01 


struct umc_dev {
	u16		version;
	u8		cap_id;
	u8		bar;
	struct resource resource;
	unsigned	irq;
	struct device	dev;
};

#define to_umc_dev(d) container_of(d, struct umc_dev, dev)


struct umc_driver {
	char *name;
	u8 cap_id;
	int (*match)(struct umc_driver *, struct umc_dev *);
	const void *match_data;

	int  (*probe)(struct umc_dev *);
	void (*remove)(struct umc_dev *);
	int  (*pre_reset)(struct umc_dev *);
	int  (*post_reset)(struct umc_dev *);

	struct device_driver driver;
};

#define to_umc_driver(d) container_of(d, struct umc_driver, driver)

extern struct bus_type umc_bus_type;

struct umc_dev *umc_device_create(struct device *parent, int n);
int __must_check umc_device_register(struct umc_dev *umc);
void umc_device_unregister(struct umc_dev *umc);

int __must_check __umc_driver_register(struct umc_driver *umc_drv,
				       struct module *mod,
				       const char *mod_name);


#define umc_driver_register(umc_drv) \
	__umc_driver_register(umc_drv, THIS_MODULE, KBUILD_MODNAME)

void umc_driver_unregister(struct umc_driver *umc_drv);


int umc_match_pci_id(struct umc_driver *umc_drv, struct umc_dev *umc);


static inline struct pci_dev *umc_parent_pci_dev(struct umc_dev *umc_dev)
{
	struct pci_dev *pci_dev = NULL;
	if (dev_is_pci(umc_dev->dev.parent))
		pci_dev = to_pci_dev(umc_dev->dev.parent);
	return pci_dev;
}


static inline struct umc_dev *umc_dev_get(struct umc_dev *umc_dev)
{
	get_device(&umc_dev->dev);
	return umc_dev;
}


static inline void umc_dev_put(struct umc_dev *umc_dev)
{
	put_device(&umc_dev->dev);
}


static inline void umc_set_drvdata(struct umc_dev *umc_dev, void *data)
{
	dev_set_drvdata(&umc_dev->dev, data);
}


static inline void *umc_get_drvdata(struct umc_dev *umc_dev)
{
	return dev_get_drvdata(&umc_dev->dev);
}

int umc_controller_reset(struct umc_dev *umc);

#endif 
