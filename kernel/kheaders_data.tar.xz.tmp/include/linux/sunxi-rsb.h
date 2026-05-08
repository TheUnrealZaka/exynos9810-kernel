
#ifndef _SUNXI_RSB_H
#define _SUNXI_RSB_H

#include <linux/device.h>
#include <linux/regmap.h>
#include <linux/types.h>

struct sunxi_rsb;


struct sunxi_rsb_device {
	struct device		dev;
	struct sunxi_rsb	*rsb;
	int			irq;
	u8			rtaddr;
	u16			hwaddr;
};

static inline struct sunxi_rsb_device *to_sunxi_rsb_device(struct device *d)
{
	return container_of(d, struct sunxi_rsb_device, dev);
}

static inline void *sunxi_rsb_device_get_drvdata(const struct sunxi_rsb_device *rdev)
{
	return dev_get_drvdata(&rdev->dev);
}

static inline void sunxi_rsb_device_set_drvdata(struct sunxi_rsb_device *rdev,
						void *data)
{
	dev_set_drvdata(&rdev->dev, data);
}


struct sunxi_rsb_driver {
	struct device_driver driver;
	int (*probe)(struct sunxi_rsb_device *rdev);
	int (*remove)(struct sunxi_rsb_device *rdev);
};

static inline struct sunxi_rsb_driver *to_sunxi_rsb_driver(struct device_driver *d)
{
	return container_of(d, struct sunxi_rsb_driver, driver);
}

int sunxi_rsb_driver_register(struct sunxi_rsb_driver *rdrv);


static inline void sunxi_rsb_driver_unregister(struct sunxi_rsb_driver *rdrv)
{
	if (rdrv)
		driver_unregister(&rdrv->driver);
}

#define module_sunxi_rsb_driver(__sunxi_rsb_driver) \
	module_driver(__sunxi_rsb_driver, sunxi_rsb_driver_register, \
			sunxi_rsb_driver_unregister)

struct regmap *__devm_regmap_init_sunxi_rsb(struct sunxi_rsb_device *rdev,
					    const struct regmap_config *config,
					    struct lock_class_key *lock_key,
					    const char *lock_name);


#define devm_regmap_init_sunxi_rsb(rdev, config)			\
	__regmap_lockdep_wrapper(__devm_regmap_init_sunxi_rsb, #config,	\
				 rdev, config)

#endif 
