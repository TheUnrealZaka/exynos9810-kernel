

#ifndef DRM_MODESET_LOCK_H_
#define DRM_MODESET_LOCK_H_

#include <linux/ww_mutex.h>

struct drm_modeset_lock;


struct drm_modeset_acquire_ctx {

	struct ww_acquire_ctx ww_ctx;

	
	struct drm_modeset_lock *contended;

	
	struct list_head locked;

	
	bool trylock_only;
};


struct drm_modeset_lock {
	
	struct ww_mutex mutex;

	
	struct list_head head;
};

extern struct ww_class crtc_ww_class;

void drm_modeset_acquire_init(struct drm_modeset_acquire_ctx *ctx,
		uint32_t flags);
void drm_modeset_acquire_fini(struct drm_modeset_acquire_ctx *ctx);
void drm_modeset_drop_locks(struct drm_modeset_acquire_ctx *ctx);
void drm_modeset_backoff(struct drm_modeset_acquire_ctx *ctx);
int drm_modeset_backoff_interruptible(struct drm_modeset_acquire_ctx *ctx);


static inline void drm_modeset_lock_init(struct drm_modeset_lock *lock)
{
	ww_mutex_init(&lock->mutex, &crtc_ww_class);
	INIT_LIST_HEAD(&lock->head);
}


static inline void drm_modeset_lock_fini(struct drm_modeset_lock *lock)
{
	WARN_ON(!list_empty(&lock->head));
}


static inline bool drm_modeset_is_locked(struct drm_modeset_lock *lock)
{
	return ww_mutex_is_locked(&lock->mutex);
}

int drm_modeset_lock(struct drm_modeset_lock *lock,
		struct drm_modeset_acquire_ctx *ctx);
int drm_modeset_lock_interruptible(struct drm_modeset_lock *lock,
		struct drm_modeset_acquire_ctx *ctx);
void drm_modeset_unlock(struct drm_modeset_lock *lock);

struct drm_device;
struct drm_crtc;
struct drm_plane;

void drm_modeset_lock_all(struct drm_device *dev);
void drm_modeset_unlock_all(struct drm_device *dev);
void drm_modeset_lock_crtc(struct drm_crtc *crtc,
			   struct drm_plane *plane);
void drm_modeset_unlock_crtc(struct drm_crtc *crtc);
void drm_warn_on_modeset_not_all_locked(struct drm_device *dev);
struct drm_modeset_acquire_ctx *
drm_modeset_legacy_acquire_ctx(struct drm_crtc *crtc);

int drm_modeset_lock_all_ctx(struct drm_device *dev,
			     struct drm_modeset_acquire_ctx *ctx);

#endif 
