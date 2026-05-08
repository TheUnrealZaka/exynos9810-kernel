

#ifndef __DRM_BLEND_H__
#define __DRM_BLEND_H__

#include <linux/list.h>
#include <linux/ctype.h>

struct drm_device;
struct drm_atomic_state;


#define DRM_ROTATE_0	BIT(0)
#define DRM_ROTATE_90	BIT(1)
#define DRM_ROTATE_180	BIT(2)
#define DRM_ROTATE_270	BIT(3)
#define DRM_ROTATE_MASK (DRM_ROTATE_0   | DRM_ROTATE_90 | \
			 DRM_ROTATE_180 | DRM_ROTATE_270)
#define DRM_REFLECT_X	BIT(4)
#define DRM_REFLECT_Y	BIT(5)
#define DRM_REFLECT_MASK (DRM_REFLECT_X | DRM_REFLECT_Y)

struct drm_property *drm_mode_create_rotation_property(struct drm_device *dev,
						       unsigned int supported_rotations);
unsigned int drm_rotation_simplify(unsigned int rotation,
				   unsigned int supported_rotations);

int drm_plane_create_zpos_property(struct drm_plane *plane,
				   unsigned int zpos,
				   unsigned int min, unsigned int max);
int drm_plane_create_zpos_immutable_property(struct drm_plane *plane,
					     unsigned int zpos);
int drm_atomic_normalize_zpos(struct drm_device *dev,
			      struct drm_atomic_state *state);
#endif
