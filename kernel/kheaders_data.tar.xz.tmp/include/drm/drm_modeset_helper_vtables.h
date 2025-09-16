

#ifndef __DRM_MODESET_HELPER_VTABLES_H__
#define __DRM_MODESET_HELPER_VTABLES_H__

#include <drm/drm_crtc.h>



enum mode_set_atomic;


struct drm_crtc_helper_funcs {
	
	void (*dpms)(struct drm_crtc *crtc, int mode);

	
	void (*prepare)(struct drm_crtc *crtc);

	
	void (*commit)(struct drm_crtc *crtc);

	
	bool (*mode_fixup)(struct drm_crtc *crtc,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);

	
	int (*mode_set)(struct drm_crtc *crtc, struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode, int x, int y,
			struct drm_framebuffer *old_fb);

	
	void (*mode_set_nofb)(struct drm_crtc *crtc);

	
	int (*mode_set_base)(struct drm_crtc *crtc, int x, int y,
			     struct drm_framebuffer *old_fb);

	
	int (*mode_set_base_atomic)(struct drm_crtc *crtc,
				    struct drm_framebuffer *fb, int x, int y,
				    enum mode_set_atomic);

	
	void (*load_lut)(struct drm_crtc *crtc);

	
	void (*disable)(struct drm_crtc *crtc);

	
	void (*enable)(struct drm_crtc *crtc);

	
	int (*atomic_check)(struct drm_crtc *crtc,
			    struct drm_crtc_state *state);

	
	void (*atomic_begin)(struct drm_crtc *crtc,
			     struct drm_crtc_state *old_crtc_state);
	
	void (*atomic_flush)(struct drm_crtc *crtc,
			     struct drm_crtc_state *old_crtc_state);

	
	void (*atomic_disable)(struct drm_crtc *crtc,
			       struct drm_crtc_state *old_crtc_state);
};


static inline void drm_crtc_helper_add(struct drm_crtc *crtc,
				       const struct drm_crtc_helper_funcs *funcs)
{
	crtc->helper_private = funcs;
}


struct drm_encoder_helper_funcs {
	
	void (*dpms)(struct drm_encoder *encoder, int mode);

	
	bool (*mode_fixup)(struct drm_encoder *encoder,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);

	
	void (*prepare)(struct drm_encoder *encoder);

	
	void (*commit)(struct drm_encoder *encoder);

	
	void (*mode_set)(struct drm_encoder *encoder,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode);

	
	void (*atomic_mode_set)(struct drm_encoder *encoder,
				struct drm_crtc_state *crtc_state,
				struct drm_connector_state *conn_state);

	
	struct drm_crtc *(*get_crtc)(struct drm_encoder *encoder);

	
	enum drm_connector_status (*detect)(struct drm_encoder *encoder,
					    struct drm_connector *connector);

	
	void (*disable)(struct drm_encoder *encoder);

	
	void (*enable)(struct drm_encoder *encoder);

	
	int (*atomic_check)(struct drm_encoder *encoder,
			    struct drm_crtc_state *crtc_state,
			    struct drm_connector_state *conn_state);
};


static inline void drm_encoder_helper_add(struct drm_encoder *encoder,
					  const struct drm_encoder_helper_funcs *funcs)
{
	encoder->helper_private = funcs;
}


struct drm_connector_helper_funcs {
	
	int (*get_modes)(struct drm_connector *connector);

	
	enum drm_mode_status (*mode_valid)(struct drm_connector *connector,
					   struct drm_display_mode *mode);
	
	struct drm_encoder *(*best_encoder)(struct drm_connector *connector);

	
	struct drm_encoder *(*atomic_best_encoder)(struct drm_connector *connector,
						   struct drm_connector_state *connector_state);
};


static inline void drm_connector_helper_add(struct drm_connector *connector,
					    const struct drm_connector_helper_funcs *funcs)
{
	connector->helper_private = funcs;
}


struct drm_plane_helper_funcs {
	
	int (*prepare_fb)(struct drm_plane *plane,
			  struct drm_plane_state *new_state);
	
	void (*cleanup_fb)(struct drm_plane *plane,
			   struct drm_plane_state *old_state);

	
	int (*atomic_check)(struct drm_plane *plane,
			    struct drm_plane_state *state);

	
	void (*atomic_update)(struct drm_plane *plane,
			      struct drm_plane_state *old_state);
	
	void (*atomic_disable)(struct drm_plane *plane,
			       struct drm_plane_state *old_state);
};


static inline void drm_plane_helper_add(struct drm_plane *plane,
					const struct drm_plane_helper_funcs *funcs)
{
	plane->helper_private = funcs;
}


struct drm_mode_config_helper_funcs {
	
	void (*atomic_commit_tail)(struct drm_atomic_state *state);
};

#endif
