


#ifndef _DRM_GLOBAL_H_
#define _DRM_GLOBAL_H_
enum drm_global_types {
	DRM_GLOBAL_TTM_MEM = 0,
	DRM_GLOBAL_TTM_BO,
	DRM_GLOBAL_TTM_OBJECT,
	DRM_GLOBAL_NUM
};

struct drm_global_reference {
	enum drm_global_types global_type;
	size_t size;
	void *object;
	int (*init) (struct drm_global_reference *);
	void (*release) (struct drm_global_reference *);
};

extern void drm_global_init(void);
extern void drm_global_release(void);
extern int drm_global_item_ref(struct drm_global_reference *ref);
extern void drm_global_item_unref(struct drm_global_reference *ref);

#endif
