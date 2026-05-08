

#ifndef __DRM_KMS_HELPER_H__
#define __DRM_KMS_HELPER_H__

#include <drm/drmP.h>

void drm_helper_move_panel_connectors_to_head(struct drm_device *);

void drm_helper_mode_fill_fb_struct(struct drm_framebuffer *fb,
				    const struct drm_mode_fb_cmd2 *mode_cmd);

int drm_crtc_init(struct drm_device *dev, struct drm_crtc *crtc,
		  const struct drm_crtc_funcs *funcs);

#endif
