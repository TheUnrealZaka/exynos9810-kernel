#ifndef _LINUX_RESET_H_
#define _LINUX_RESET_H_

#include <linux/device.h>

struct reset_control;

#ifdef CONFIG_RESET_CONTROLLER

int reset_control_reset(struct reset_control *rstc);
int reset_control_assert(struct reset_control *rstc);
int reset_control_deassert(struct reset_control *rstc);
int reset_control_status(struct reset_control *rstc);

struct reset_control *__of_reset_control_get(struct device_node *node,
				     const char *id, int index, bool shared,
				     bool optional);
struct reset_control *__reset_control_get(struct device *dev, const char *id,
					  int index, bool shared,
					  bool optional);
void reset_control_put(struct reset_control *rstc);
int __device_reset(struct device *dev, bool optional);
struct reset_control *__devm_reset_control_get(struct device *dev,
				     const char *id, int index, bool shared,
				     bool optional);

#else

static inline int reset_control_reset(struct reset_control *rstc)
{
	return 0;
}

static inline int reset_control_assert(struct reset_control *rstc)
{
	return 0;
}

static inline int reset_control_deassert(struct reset_control *rstc)
{
	return 0;
}

static inline int reset_control_status(struct reset_control *rstc)
{
	return 0;
}

static inline void reset_control_put(struct reset_control *rstc)
{
}

static inline int __device_reset(struct device *dev, bool optional)
{
	return optional ? 0 : -ENOTSUPP;
}

static inline struct reset_control *__of_reset_control_get(
					struct device_node *node,
					const char *id, int index, bool shared,
					bool optional)
{
	return optional ? NULL : ERR_PTR(-ENOTSUPP);
}

static inline struct reset_control *__reset_control_get(
					struct device *dev, const char *id,
					int index, bool shared, bool optional)
{
	return optional ? NULL : ERR_PTR(-ENOTSUPP);
}

static inline struct reset_control *__devm_reset_control_get(
					struct device *dev, const char *id,
					int index, bool shared, bool optional)
{
	return optional ? NULL : ERR_PTR(-ENOTSUPP);
}

#endif 

static inline int __must_check device_reset(struct device *dev)
{
	return __device_reset(dev, false);
}

static inline int device_reset_optional(struct device *dev)
{
	return __device_reset(dev, true);
}


static inline struct reset_control *
__must_check reset_control_get_exclusive(struct device *dev, const char *id)
{
	return __reset_control_get(dev, id, 0, false, false);
}


static inline struct reset_control *reset_control_get_shared(
					struct device *dev, const char *id)
{
	return __reset_control_get(dev, id, 0, true, false);
}

static inline struct reset_control *reset_control_get_optional_exclusive(
					struct device *dev, const char *id)
{
	return __reset_control_get(dev, id, 0, false, true);
}

static inline struct reset_control *reset_control_get_optional_shared(
					struct device *dev, const char *id)
{
	return __reset_control_get(dev, id, 0, true, true);
}


static inline struct reset_control *of_reset_control_get_exclusive(
				struct device_node *node, const char *id)
{
	return __of_reset_control_get(node, id, 0, false, false);
}


static inline struct reset_control *of_reset_control_get_shared(
				struct device_node *node, const char *id)
{
	return __of_reset_control_get(node, id, 0, true, false);
}


static inline struct reset_control *of_reset_control_get_exclusive_by_index(
					struct device_node *node, int index)
{
	return __of_reset_control_get(node, NULL, index, false, false);
}


static inline struct reset_control *of_reset_control_get_shared_by_index(
					struct device_node *node, int index)
{
	return __of_reset_control_get(node, NULL, index, true, false);
}


static inline struct reset_control *
__must_check devm_reset_control_get_exclusive(struct device *dev,
					      const char *id)
{
	return __devm_reset_control_get(dev, id, 0, false, false);
}


static inline struct reset_control *devm_reset_control_get_shared(
					struct device *dev, const char *id)
{
	return __devm_reset_control_get(dev, id, 0, true, false);
}

static inline struct reset_control *devm_reset_control_get_optional_exclusive(
					struct device *dev, const char *id)
{
	return __devm_reset_control_get(dev, id, 0, false, true);
}

static inline struct reset_control *devm_reset_control_get_optional_shared(
					struct device *dev, const char *id)
{
	return __devm_reset_control_get(dev, id, 0, true, true);
}


static inline struct reset_control *
devm_reset_control_get_exclusive_by_index(struct device *dev, int index)
{
	return __devm_reset_control_get(dev, NULL, index, false, false);
}


static inline struct reset_control *
devm_reset_control_get_shared_by_index(struct device *dev, int index)
{
	return __devm_reset_control_get(dev, NULL, index, true, false);
}


static inline struct reset_control *reset_control_get(
				struct device *dev, const char *id)
{
	return reset_control_get_exclusive(dev, id);
}

static inline struct reset_control *reset_control_get_optional(
					struct device *dev, const char *id)
{
	return reset_control_get_optional_exclusive(dev, id);
}

static inline struct reset_control *of_reset_control_get(
				struct device_node *node, const char *id)
{
	return of_reset_control_get_exclusive(node, id);
}

static inline struct reset_control *of_reset_control_get_by_index(
				struct device_node *node, int index)
{
	return of_reset_control_get_exclusive_by_index(node, index);
}

static inline struct reset_control *devm_reset_control_get(
				struct device *dev, const char *id)
{
	return devm_reset_control_get_exclusive(dev, id);
}

static inline struct reset_control *devm_reset_control_get_optional(
				struct device *dev, const char *id)
{
	return devm_reset_control_get_optional_exclusive(dev, id);

}

static inline struct reset_control *devm_reset_control_get_by_index(
				struct device *dev, int index)
{
	return devm_reset_control_get_exclusive_by_index(dev, index);
}
#endif
