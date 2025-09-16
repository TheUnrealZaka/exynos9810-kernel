

#ifndef __LINUX_DRM_SIMPLE_KMS_HELPER_H
#define __LINUX_DRM_SIMPLE_KMS_HELPER_H

struct drm_simple_display_pipe;


struct drm_simple_display_pipe_funcs {
	
	void (*enable)(struct drm_simple_display_pipe *pipe,
		       struct drm_crtc_state *crtc_state);
	
	void (*disable)(struct drm_simple_display_pipe *pipe);

	
	int (*check)(struct drm_simple_display_pipe *pipe,
		     struct drm_plane_state *plane_state,
		     struct drm_crtc_state *crtc_state);
	
	void (*update)(struct drm_simple_display_pipe *pipe,
		       struct drm_plane_state *plane_state);

	
	int (*prepare_fb)(struct drm_simple_display_pipe *pipe,
			  struct drm_plane_state *plane_state);

	
	void (*cleanup_fb)(struct drm_simple_display_pipe *pipe,
			   struct drm_plane_state *plane_state);
};


struct drm_simple_display_pipe {
	struct drm_crtc crtc;
	struct drm_plane plane;
	struct drm_encoder encoder;
	struct drm_connector *connector;

	const struct drm_simple_display_pipe_funcs *funcs;
};

int drm_simple_display_pipe_attach_bridge(struct drm_simple_display_pipe *pipe,
					  struct drm_bridge *bridge);

void drm_simple_display_pipe_detach_bridge(struct drm_simple_display_pipe *pipe);

int drm_simple_display_pipe_init(struct drm_device *dev,
			struct drm_simple_display_pipe *pipe,
			const struct drm_simple_display_pipe_funcs *funcs,
			const uint32_t *formats, unsigned int format_count,
			struct drm_connector *connector);

#endif 
