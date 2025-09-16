

#ifndef DRM_PLANE_HELPER_H
#define DRM_PLANE_HELPER_H

#include <drm/drm_rect.h>
#include <drm/drm_crtc.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_modeset_helper.h>


#define DRM_PLANE_HELPER_NO_SCALING (1<<16)

int drm_plane_helper_check_state(struct drm_plane_state *state,
				 const struct drm_rect *clip,
				 int min_scale, int max_scale,
				 bool can_position,
				 bool can_update_disabled);
int drm_plane_helper_check_update(struct drm_plane *plane,
				  struct drm_crtc *crtc,
				  struct drm_framebuffer *fb,
				  struct drm_rect *src,
				  struct drm_rect *dest,
				  const struct drm_rect *clip,
				  unsigned int rotation,
				  int min_scale,
				  int max_scale,
				  bool can_position,
				  bool can_update_disabled,
				  bool *visible);
int drm_primary_helper_update(struct drm_plane *plane,
			      struct drm_crtc *crtc,
			      struct drm_framebuffer *fb,
			      int crtc_x, int crtc_y,
			      unsigned int crtc_w, unsigned int crtc_h,
			      uint32_t src_x, uint32_t src_y,
			      uint32_t src_w, uint32_t src_h);
int drm_primary_helper_disable(struct drm_plane *plane);
void drm_primary_helper_destroy(struct drm_plane *plane);
extern const struct drm_plane_funcs drm_primary_helper_funcs;

int drm_plane_helper_update(struct drm_plane *plane, struct drm_crtc *crtc,
			    struct drm_framebuffer *fb,
			    int crtc_x, int crtc_y,
			    unsigned int crtc_w, unsigned int crtc_h,
			    uint32_t src_x, uint32_t src_y,
			    uint32_t src_w, uint32_t src_h);
int drm_plane_helper_disable(struct drm_plane *plane);


int drm_plane_helper_commit(struct drm_plane *plane,
			    struct drm_plane_state *plane_state,
			    struct drm_framebuffer *old_fb);
#endif
