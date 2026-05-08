

#ifndef _DRM_AUTH_H_
#define _DRM_AUTH_H_


struct drm_master {
	struct kref refcount;
	struct drm_device *dev;
	char *unique;
	int unique_len;
	struct idr magic_map;
	struct drm_lock_data lock;
	void *driver_priv;
};

struct drm_master *drm_master_get(struct drm_master *master);
void drm_master_put(struct drm_master **master);
bool drm_is_current_master(struct drm_file *fpriv);

#endif
