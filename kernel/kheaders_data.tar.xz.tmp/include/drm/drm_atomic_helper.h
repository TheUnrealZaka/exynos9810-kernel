

#ifndef DRM_ATOMIC_HELPER_H_
#define DRM_ATOMIC_HELPER_H_

#include <drm/drm_crtc.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_modeset_helper.h>

struct drm_atomic_state;

int drm_atomic_helper_check_modeset(struct drm_device *dev,
				struct drm_atomic_state *state);
int drm_atomic_helper_check_planes(struct drm_device *dev,
			       struct drm_atomic_state *state);
int drm_atomic_helper_check(struct drm_device *dev,
			    struct drm_atomic_state *state);
void drm_atomic_helper_commit_tail(struct drm_atomic_state *state);
int drm_atomic_helper_commit(struct drm_device *dev,
			     struct drm_atomic_state *state,
			     bool nonblock);

int drm_atomic_helper_wait_for_fences(struct drm_device *dev,
					struct drm_atomic_state *state,
					bool pre_swap);
bool drm_atomic_helper_framebuffer_changed(struct drm_device *dev,
					   struct drm_atomic_state *old_state,
					   struct drm_crtc *crtc);

void drm_atomic_helper_wait_for_vblanks(struct drm_device *dev,
					struct drm_atomic_state *old_state);

void
drm_atomic_helper_update_legacy_modeset_state(struct drm_device *dev,
					      struct drm_atomic_state *old_state);

void drm_atomic_helper_commit_modeset_disables(struct drm_device *dev,
					       struct drm_atomic_state *state);
void drm_atomic_helper_commit_modeset_enables(struct drm_device *dev,
					  struct drm_atomic_state *old_state);

int drm_atomic_helper_prepare_planes(struct drm_device *dev,
				     struct drm_atomic_state *state);

#define DRM_PLANE_COMMIT_ACTIVE_ONLY			BIT(0)
#define DRM_PLANE_COMMIT_NO_DISABLE_AFTER_MODESET	BIT(1)

void drm_atomic_helper_commit_planes(struct drm_device *dev,
				     struct drm_atomic_state *state,
				     uint32_t flags);
void drm_atomic_helper_cleanup_planes(struct drm_device *dev,
				      struct drm_atomic_state *old_state);
void drm_atomic_helper_commit_planes_on_crtc(struct drm_crtc_state *old_crtc_state);
void
drm_atomic_helper_disable_planes_on_crtc(struct drm_crtc_state *old_crtc_state,
					 bool atomic);

void drm_atomic_helper_swap_state(struct drm_atomic_state *state,
				  bool stall);


int drm_atomic_helper_setup_commit(struct drm_atomic_state *state,
				   bool nonblock);
void drm_atomic_helper_wait_for_dependencies(struct drm_atomic_state *state);
void drm_atomic_helper_commit_hw_done(struct drm_atomic_state *state);
void drm_atomic_helper_commit_cleanup_done(struct drm_atomic_state *state);


int drm_atomic_helper_update_plane(struct drm_plane *plane,
				   struct drm_crtc *crtc,
				   struct drm_framebuffer *fb,
				   int crtc_x, int crtc_y,
				   unsigned int crtc_w, unsigned int crtc_h,
				   uint32_t src_x, uint32_t src_y,
				   uint32_t src_w, uint32_t src_h);
int drm_atomic_helper_disable_plane(struct drm_plane *plane);
int __drm_atomic_helper_disable_plane(struct drm_plane *plane,
		struct drm_plane_state *plane_state);
int drm_atomic_helper_set_config(struct drm_mode_set *set);
int __drm_atomic_helper_set_config(struct drm_mode_set *set,
		struct drm_atomic_state *state);

int drm_atomic_helper_disable_all(struct drm_device *dev,
				  struct drm_modeset_acquire_ctx *ctx);
struct drm_atomic_state *drm_atomic_helper_suspend(struct drm_device *dev);
int drm_atomic_helper_resume(struct drm_device *dev,
			     struct drm_atomic_state *state);

int drm_atomic_helper_crtc_set_property(struct drm_crtc *crtc,
					struct drm_property *property,
					uint64_t val);
int drm_atomic_helper_plane_set_property(struct drm_plane *plane,
					struct drm_property *property,
					uint64_t val);
int drm_atomic_helper_connector_set_property(struct drm_connector *connector,
					struct drm_property *property,
					uint64_t val);
int drm_atomic_helper_page_flip(struct drm_crtc *crtc,
				struct drm_framebuffer *fb,
				struct drm_pending_vblank_event *event,
				uint32_t flags);
int drm_atomic_helper_connector_dpms(struct drm_connector *connector,
				     int mode);
struct drm_encoder *
drm_atomic_helper_best_encoder(struct drm_connector *connector);


void drm_atomic_helper_crtc_reset(struct drm_crtc *crtc);
void __drm_atomic_helper_crtc_duplicate_state(struct drm_crtc *crtc,
					      struct drm_crtc_state *state);
struct drm_crtc_state *
drm_atomic_helper_crtc_duplicate_state(struct drm_crtc *crtc);
void __drm_atomic_helper_crtc_destroy_state(struct drm_crtc_state *state);
void drm_atomic_helper_crtc_destroy_state(struct drm_crtc *crtc,
					  struct drm_crtc_state *state);

void drm_atomic_helper_plane_reset(struct drm_plane *plane);
void __drm_atomic_helper_plane_duplicate_state(struct drm_plane *plane,
					       struct drm_plane_state *state);
struct drm_plane_state *
drm_atomic_helper_plane_duplicate_state(struct drm_plane *plane);
void __drm_atomic_helper_plane_destroy_state(struct drm_plane_state *state);
void drm_atomic_helper_plane_destroy_state(struct drm_plane *plane,
					  struct drm_plane_state *state);

void __drm_atomic_helper_connector_reset(struct drm_connector *connector,
					 struct drm_connector_state *conn_state);
void drm_atomic_helper_connector_reset(struct drm_connector *connector);
void
__drm_atomic_helper_connector_duplicate_state(struct drm_connector *connector,
					   struct drm_connector_state *state);
struct drm_connector_state *
drm_atomic_helper_connector_duplicate_state(struct drm_connector *connector);
struct drm_atomic_state *
drm_atomic_helper_duplicate_state(struct drm_device *dev,
				  struct drm_modeset_acquire_ctx *ctx);
void
__drm_atomic_helper_connector_destroy_state(struct drm_connector_state *state);
void drm_atomic_helper_connector_destroy_state(struct drm_connector *connector,
					  struct drm_connector_state *state);
int drm_atomic_helper_legacy_gamma_set(struct drm_crtc *crtc,
				       u16 *red, u16 *green, u16 *blue,
				       uint32_t size);


#define drm_atomic_crtc_for_each_plane(plane, crtc) \
	drm_for_each_plane_mask(plane, (crtc)->dev, (crtc)->state->plane_mask)


#define drm_atomic_crtc_state_for_each_plane(plane, crtc_state) \
	drm_for_each_plane_mask(plane, (crtc_state)->state->dev, (crtc_state)->plane_mask)


#define drm_atomic_crtc_state_for_each_plane_state(plane, plane_state, crtc_state) \
	drm_for_each_plane_mask(plane, (crtc_state)->state->dev, (crtc_state)->plane_mask) \
		for_each_if ((plane_state = \
			      __drm_atomic_get_current_plane_state((crtc_state)->state, \
								   plane)))


static inline bool
drm_atomic_plane_disabling(struct drm_plane *plane,
			   struct drm_plane_state *old_state)
{
	
	WARN_ON((plane->state->crtc == NULL && plane->state->fb != NULL) ||
		(plane->state->crtc != NULL && plane->state->fb == NULL));

	
	return (!old_state || old_state->crtc) && !plane->state->crtc;
}

#endif 
