

#ifndef _LINUX_VEXPRESS_H
#define _LINUX_VEXPRESS_H

#include <linux/device.h>
#include <linux/regmap.h>

#define VEXPRESS_SITE_MB		0
#define VEXPRESS_SITE_DB1		1
#define VEXPRESS_SITE_DB2		2
#define VEXPRESS_SITE_MASTER		0xf



void vexpress_config_set_master(u32 site);
u32 vexpress_config_get_master(void);

void vexpress_config_lock(void *arg);
void vexpress_config_unlock(void *arg);

int vexpress_config_get_topo(struct device_node *node, u32 *site,
		u32 *position, u32 *dcc);



struct vexpress_config_bridge_ops {
	struct regmap * (*regmap_init)(struct device *dev, void *context);
	void (*regmap_exit)(struct regmap *regmap, void *context);
};

struct device *vexpress_config_bridge_register(struct device *parent,
		struct vexpress_config_bridge_ops *ops, void *context);



struct regmap *devm_regmap_init_vexpress_config(struct device *dev);



void vexpress_flags_set(u32 data);

#endif
