

#ifndef __DRM_BRIDGE_H__
#define __DRM_BRIDGE_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_modes.h>

struct drm_bridge;


struct drm_bridge_funcs {
	
	int (*attach)(struct drm_bridge *bridge);

	
	void (*detach)(struct drm_bridge *bridge);

	
	bool (*mode_fixup)(struct drm_bridge *bridge,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	
	void (*disable)(struct drm_bridge *bridge);

	
	void (*post_disable)(struct drm_bridge *bridge);

	
	void (*mode_set)(struct drm_bridge *bridge,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode);
	
	void (*pre_enable)(struct drm_bridge *bridge);

	
	void (*enable)(struct drm_bridge *bridge);
};


struct drm_bridge {
	struct drm_device *dev;
	struct drm_encoder *encoder;
	struct drm_bridge *next;
#ifdef CONFIG_OF
	struct device_node *of_node;
#endif
	struct list_head list;

	const struct drm_bridge_funcs *funcs;
	void *driver_private;
};

int drm_bridge_add(struct drm_bridge *bridge);
void drm_bridge_remove(struct drm_bridge *bridge);
struct drm_bridge *of_drm_find_bridge(struct device_node *np);
int drm_bridge_attach(struct drm_device *dev, struct drm_bridge *bridge);
void drm_bridge_detach(struct drm_bridge *bridge);

bool drm_bridge_mode_fixup(struct drm_bridge *bridge,
			const struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode);
void drm_bridge_disable(struct drm_bridge *bridge);
void drm_bridge_post_disable(struct drm_bridge *bridge);
void drm_bridge_mode_set(struct drm_bridge *bridge,
			struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode);
void drm_bridge_pre_enable(struct drm_bridge *bridge);
void drm_bridge_enable(struct drm_bridge *bridge);

#endif
