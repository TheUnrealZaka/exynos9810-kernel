

#ifndef __LINUX_DEVFREQ_EVENT_H__
#define __LINUX_DEVFREQ_EVENT_H__

#include <linux/device.h>


struct devfreq_event_dev {
	struct list_head node;

	struct device dev;
	struct mutex lock;
	u32 enable_count;

	const struct devfreq_event_desc *desc;
};


struct devfreq_event_data {
	unsigned long load_count;
	unsigned long total_count;
};


struct devfreq_event_ops {
	
	int (*enable)(struct devfreq_event_dev *edev);
	int (*disable)(struct devfreq_event_dev *edev);
	int (*reset)(struct devfreq_event_dev *edev);

	
	int (*set_event)(struct devfreq_event_dev *edev);
	int (*get_event)(struct devfreq_event_dev *edev,
			 struct devfreq_event_data *edata);
};


struct devfreq_event_desc {
	const char *name;
	void *driver_data;

	const struct devfreq_event_ops *ops;
};

#if defined(CONFIG_PM_DEVFREQ_EVENT)
extern int devfreq_event_enable_edev(struct devfreq_event_dev *edev);
extern int devfreq_event_disable_edev(struct devfreq_event_dev *edev);
extern bool devfreq_event_is_enabled(struct devfreq_event_dev *edev);
extern int devfreq_event_set_event(struct devfreq_event_dev *edev);
extern int devfreq_event_get_event(struct devfreq_event_dev *edev,
				struct devfreq_event_data *edata);
extern int devfreq_event_reset_event(struct devfreq_event_dev *edev);
extern struct devfreq_event_dev *devfreq_event_get_edev_by_phandle(
				struct device *dev, int index);
extern int devfreq_event_get_edev_count(struct device *dev);
extern struct devfreq_event_dev *devfreq_event_add_edev(struct device *dev,
				struct devfreq_event_desc *desc);
extern int devfreq_event_remove_edev(struct devfreq_event_dev *edev);
extern struct devfreq_event_dev *devm_devfreq_event_add_edev(struct device *dev,
				struct devfreq_event_desc *desc);
extern void devm_devfreq_event_remove_edev(struct device *dev,
				struct devfreq_event_dev *edev);
static inline void *devfreq_event_get_drvdata(struct devfreq_event_dev *edev)
{
	return edev->desc->driver_data;
}
#else
static inline int devfreq_event_enable_edev(struct devfreq_event_dev *edev)
{
	return -EINVAL;
}

static inline int devfreq_event_disable_edev(struct devfreq_event_dev *edev)
{
	return -EINVAL;
}

static inline bool devfreq_event_is_enabled(struct devfreq_event_dev *edev)
{
	return false;
}

static inline int devfreq_event_set_event(struct devfreq_event_dev *edev)
{
	return -EINVAL;
}

static inline int devfreq_event_get_event(struct devfreq_event_dev *edev,
					struct devfreq_event_data *edata)
{
	return -EINVAL;
}

static inline int devfreq_event_reset_event(struct devfreq_event_dev *edev)
{
	return -EINVAL;
}

static inline struct devfreq_event_dev *devfreq_event_get_edev_by_phandle(
					struct device *dev, int index)
{
	return ERR_PTR(-EINVAL);
}

static inline int devfreq_event_get_edev_count(struct device *dev)
{
	return -EINVAL;
}

static inline struct devfreq_event_dev *devfreq_event_add_edev(struct device *dev,
					struct devfreq_event_desc *desc)
{
	return ERR_PTR(-EINVAL);
}

static inline int devfreq_event_remove_edev(struct devfreq_event_dev *edev)
{
	return -EINVAL;
}

static inline struct devfreq_event_dev *devm_devfreq_event_add_edev(
					struct device *dev,
					struct devfreq_event_desc *desc)
{
	return ERR_PTR(-EINVAL);
}

static inline void devm_devfreq_event_remove_edev(struct device *dev,
					struct devfreq_event_dev *edev)
{
}

static inline void *devfreq_event_get_drvdata(struct devfreq_event_dev *edev)
{
	return NULL;
}
#endif 

#endif 
