

#include <linux/device.h>


extern struct bus_type container_subsys;

struct container_dev {
	struct device dev;
	int (*offline)(struct container_dev *cdev);
};

static inline struct container_dev *to_container_dev(struct device *dev)
{
	return container_of(dev, struct container_dev, dev);
}
