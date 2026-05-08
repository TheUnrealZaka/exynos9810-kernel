

#ifndef __DRM_ENCODER_H__
#define __DRM_ENCODER_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>


struct drm_encoder_funcs {
	
	void (*reset)(struct drm_encoder *encoder);

	
	void (*destroy)(struct drm_encoder *encoder);

	
	int (*late_register)(struct drm_encoder *encoder);

	
	void (*early_unregister)(struct drm_encoder *encoder);
};


struct drm_encoder {
	struct drm_device *dev;
	struct list_head head;

	struct drm_mode_object base;
	char *name;
	
	int encoder_type;

	
	unsigned index;

	
	uint32_t possible_crtcs;

	
	uint32_t possible_clones;

	struct drm_crtc *crtc;
	struct drm_bridge *bridge;
	const struct drm_encoder_funcs *funcs;
	const struct drm_encoder_helper_funcs *helper_private;
};

#define obj_to_encoder(x) container_of(x, struct drm_encoder, base)

__printf(5, 6)
int drm_encoder_init(struct drm_device *dev,
		     struct drm_encoder *encoder,
		     const struct drm_encoder_funcs *funcs,
		     int encoder_type, const char *name, ...);


static inline unsigned int drm_encoder_index(struct drm_encoder *encoder)
{
	return encoder->index;
}


static inline uint32_t drm_crtc_mask(struct drm_crtc *crtc);


static inline bool drm_encoder_crtc_ok(struct drm_encoder *encoder,
				       struct drm_crtc *crtc)
{
	return !!(encoder->possible_crtcs & drm_crtc_mask(crtc));
}


static inline struct drm_encoder *drm_encoder_find(struct drm_device *dev,
						   uint32_t id)
{
	struct drm_mode_object *mo;

	mo = drm_mode_object_find(dev, id, DRM_MODE_OBJECT_ENCODER);

	return mo ? obj_to_encoder(mo) : NULL;
}

void drm_encoder_cleanup(struct drm_encoder *encoder);


#define drm_for_each_encoder_mask(encoder, dev, encoder_mask) \
	list_for_each_entry((encoder), &(dev)->mode_config.encoder_list, head) \
		for_each_if ((encoder_mask) & (1 << drm_encoder_index(encoder)))


#define drm_for_each_encoder(encoder, dev) \
	list_for_each_entry(encoder, &(dev)->mode_config.encoder_list, head)

#endif
