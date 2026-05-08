

#ifndef DRM_ATOMIC_H_
#define DRM_ATOMIC_H_

#include <drm/drm_crtc.h>


struct drm_crtc_commit {
	
	struct drm_crtc *crtc;

	
	struct kref ref;

	
	struct completion flip_done;

	
	struct completion hw_done;

	
	struct completion cleanup_done;

	
	struct list_head commit_entry;

	
	struct drm_pending_vblank_event *event;
};

struct __drm_planes_state {
	struct drm_plane *ptr;
	struct drm_plane_state *state;
};

struct __drm_crtcs_state {
	struct drm_crtc *ptr;
	struct drm_crtc_state *state;
	struct drm_crtc_commit *commit;
	s32 __user *out_fence_ptr;
};

struct __drm_connnectors_state {
	struct drm_connector *ptr;
	struct drm_connector_state *state;
};


struct drm_atomic_state {
	struct drm_device *dev;
	bool allow_modeset : 1;
	bool legacy_cursor_update : 1;
	bool legacy_set_config : 1;
	struct __drm_planes_state *planes;
	struct __drm_crtcs_state *crtcs;
	int num_connector;
	struct __drm_connnectors_state *connectors;

	struct drm_modeset_acquire_ctx *acquire_ctx;

	
	struct work_struct commit_work;
};

void drm_crtc_commit_put(struct drm_crtc_commit *commit);
static inline void drm_crtc_commit_get(struct drm_crtc_commit *commit)
{
	kref_get(&commit->ref);
}

struct drm_atomic_state * __must_check
drm_atomic_state_alloc(struct drm_device *dev);
void drm_atomic_state_clear(struct drm_atomic_state *state);
void drm_atomic_state_free(struct drm_atomic_state *state);

int  __must_check
drm_atomic_state_init(struct drm_device *dev, struct drm_atomic_state *state);
void drm_atomic_state_default_clear(struct drm_atomic_state *state);
void drm_atomic_state_default_release(struct drm_atomic_state *state);

struct drm_crtc_state * __must_check
drm_atomic_get_crtc_state(struct drm_atomic_state *state,
			  struct drm_crtc *crtc);
int drm_atomic_crtc_set_property(struct drm_crtc *crtc,
		struct drm_crtc_state *state, struct drm_property *property,
		uint64_t val);
struct drm_plane_state * __must_check
drm_atomic_get_plane_state(struct drm_atomic_state *state,
			   struct drm_plane *plane);
int drm_atomic_plane_set_property(struct drm_plane *plane,
		struct drm_plane_state *state, struct drm_property *property,
		uint64_t val);
struct drm_connector_state * __must_check
drm_atomic_get_connector_state(struct drm_atomic_state *state,
			       struct drm_connector *connector);
int drm_atomic_connector_set_property(struct drm_connector *connector,
		struct drm_connector_state *state, struct drm_property *property,
		uint64_t val);


static inline struct drm_crtc_state *
drm_atomic_get_existing_crtc_state(struct drm_atomic_state *state,
				   struct drm_crtc *crtc)
{
	return state->crtcs[drm_crtc_index(crtc)].state;
}


static inline struct drm_plane_state *
drm_atomic_get_existing_plane_state(struct drm_atomic_state *state,
				    struct drm_plane *plane)
{
	return state->planes[drm_plane_index(plane)].state;
}


static inline struct drm_connector_state *
drm_atomic_get_existing_connector_state(struct drm_atomic_state *state,
					struct drm_connector *connector)
{
	int index = drm_connector_index(connector);

	if (index >= state->num_connector)
		return NULL;

	return state->connectors[index].state;
}


static inline const struct drm_plane_state *
__drm_atomic_get_current_plane_state(struct drm_atomic_state *state,
				     struct drm_plane *plane)
{
	if (state->planes[drm_plane_index(plane)].state)
		return state->planes[drm_plane_index(plane)].state;

	return plane->state;
}

int __must_check
drm_atomic_set_mode_for_crtc(struct drm_crtc_state *state,
			     struct drm_display_mode *mode);
int __must_check
drm_atomic_set_mode_prop_for_crtc(struct drm_crtc_state *state,
				  struct drm_property_blob *blob);
int __must_check
drm_atomic_set_crtc_for_plane(struct drm_plane_state *plane_state,
			      struct drm_crtc *crtc);
void drm_atomic_set_fb_for_plane(struct drm_plane_state *plane_state,
				 struct drm_framebuffer *fb);
void drm_atomic_set_fence_for_plane(struct drm_plane_state *plane_state,
				    struct fence *fence);
int __must_check
drm_atomic_set_crtc_for_connector(struct drm_connector_state *conn_state,
				  struct drm_crtc *crtc);
int __must_check
drm_atomic_add_affected_connectors(struct drm_atomic_state *state,
				   struct drm_crtc *crtc);
int __must_check
drm_atomic_add_affected_planes(struct drm_atomic_state *state,
			       struct drm_crtc *crtc);

void drm_atomic_legacy_backoff(struct drm_atomic_state *state);

void
drm_atomic_clean_old_fb(struct drm_device *dev, unsigned plane_mask, int ret);

int __must_check drm_atomic_check_only(struct drm_atomic_state *state);
int __must_check drm_atomic_commit(struct drm_atomic_state *state);
int __must_check drm_atomic_nonblocking_commit(struct drm_atomic_state *state);

#define for_each_connector_in_state(__state, connector, connector_state, __i) \
	for ((__i) = 0;							\
	     (__i) < (__state)->num_connector &&				\
	     ((connector) = (__state)->connectors[__i].ptr,			\
	     (connector_state) = (__state)->connectors[__i].state, 1); 	\
	     (__i)++)							\
		for_each_if (connector)

#define for_each_crtc_in_state(__state, crtc, crtc_state, __i)	\
	for ((__i) = 0;						\
	     (__i) < (__state)->dev->mode_config.num_crtc &&	\
	     ((crtc) = (__state)->crtcs[__i].ptr,			\
	     (crtc_state) = (__state)->crtcs[__i].state, 1);	\
	     (__i)++)						\
		for_each_if (crtc_state)

#define for_each_plane_in_state(__state, plane, plane_state, __i)		\
	for ((__i) = 0;							\
	     (__i) < (__state)->dev->mode_config.num_total_plane &&	\
	     ((plane) = (__state)->planes[__i].ptr,				\
	     (plane_state) = (__state)->planes[__i].state, 1);		\
	     (__i)++)							\
		for_each_if (plane_state)


static inline bool
drm_atomic_crtc_needs_modeset(struct drm_crtc_state *state)
{
	return state->mode_changed || state->active_changed ||
	       state->connectors_changed;
}


#endif 
