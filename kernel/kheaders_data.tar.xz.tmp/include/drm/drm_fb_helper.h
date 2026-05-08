
#ifndef DRM_FB_HELPER_H
#define DRM_FB_HELPER_H

struct drm_fb_helper;

#include <drm/drm_crtc.h>
#include <linux/kgdb.h>

enum mode_set_atomic {
	LEAVE_ATOMIC_MODE_SET,
	ENTER_ATOMIC_MODE_SET,
};

struct drm_fb_offset {
	int x, y;
};

struct drm_fb_helper_crtc {
	struct drm_mode_set mode_set;
	struct drm_display_mode *desired_mode;
	int x, y;
};


struct drm_fb_helper_surface_size {
	u32 fb_width;
	u32 fb_height;
	u32 surface_width;
	u32 surface_height;
	u32 surface_bpp;
	u32 surface_depth;
};


struct drm_fb_helper_funcs {
	
	void (*gamma_set)(struct drm_crtc *crtc, u16 red, u16 green,
			  u16 blue, int regno);
	
	void (*gamma_get)(struct drm_crtc *crtc, u16 *red, u16 *green,
			  u16 *blue, int regno);

	
	int (*fb_probe)(struct drm_fb_helper *helper,
			struct drm_fb_helper_surface_size *sizes);

	
	bool (*initial_config)(struct drm_fb_helper *fb_helper,
			       struct drm_fb_helper_crtc **crtcs,
			       struct drm_display_mode **modes,
			       struct drm_fb_offset *offsets,
			       bool *enabled, int width, int height);
};

struct drm_fb_helper_connector {
	struct drm_connector *connector;
};


struct drm_fb_helper {
	struct drm_framebuffer *fb;
	struct drm_device *dev;
	int crtc_count;
	struct drm_fb_helper_crtc *crtc_info;
	int connector_count;
	int connector_info_alloc_count;
	struct drm_fb_helper_connector **connector_info;
	const struct drm_fb_helper_funcs *funcs;
	struct fb_info *fbdev;
	u32 pseudo_palette[17];
	struct drm_clip_rect dirty_clip;
	spinlock_t dirty_lock;
	struct work_struct dirty_work;
	struct work_struct resume_work;

	
	struct list_head kernel_fb_list;

	
	bool delayed_hotplug;
};


#define DRM_FB_HELPER_DEFAULT_OPS \
	.fb_check_var	= drm_fb_helper_check_var, \
	.fb_set_par	= drm_fb_helper_set_par, \
	.fb_setcmap	= drm_fb_helper_setcmap, \
	.fb_blank	= drm_fb_helper_blank, \
	.fb_pan_display	= drm_fb_helper_pan_display

#ifdef CONFIG_DRM_FBDEV_EMULATION
void drm_fb_helper_prepare(struct drm_device *dev, struct drm_fb_helper *helper,
			   const struct drm_fb_helper_funcs *funcs);
int drm_fb_helper_init(struct drm_device *dev,
		       struct drm_fb_helper *helper, int crtc_count,
		       int max_conn);
void drm_fb_helper_fini(struct drm_fb_helper *helper);
int drm_fb_helper_blank(int blank, struct fb_info *info);
int drm_fb_helper_pan_display(struct fb_var_screeninfo *var,
			      struct fb_info *info);
int drm_fb_helper_set_par(struct fb_info *info);
int drm_fb_helper_check_var(struct fb_var_screeninfo *var,
			    struct fb_info *info);

int drm_fb_helper_restore_fbdev_mode_unlocked(struct drm_fb_helper *fb_helper);

struct fb_info *drm_fb_helper_alloc_fbi(struct drm_fb_helper *fb_helper);
void drm_fb_helper_unregister_fbi(struct drm_fb_helper *fb_helper);
void drm_fb_helper_release_fbi(struct drm_fb_helper *fb_helper);
void drm_fb_helper_fill_var(struct fb_info *info, struct drm_fb_helper *fb_helper,
			    uint32_t fb_width, uint32_t fb_height);
void drm_fb_helper_fill_fix(struct fb_info *info, uint32_t pitch,
			    uint32_t depth);

void drm_fb_helper_unlink_fbi(struct drm_fb_helper *fb_helper);

void drm_fb_helper_deferred_io(struct fb_info *info,
			       struct list_head *pagelist);

ssize_t drm_fb_helper_sys_read(struct fb_info *info, char __user *buf,
			       size_t count, loff_t *ppos);
ssize_t drm_fb_helper_sys_write(struct fb_info *info, const char __user *buf,
				size_t count, loff_t *ppos);

void drm_fb_helper_sys_fillrect(struct fb_info *info,
				const struct fb_fillrect *rect);
void drm_fb_helper_sys_copyarea(struct fb_info *info,
				const struct fb_copyarea *area);
void drm_fb_helper_sys_imageblit(struct fb_info *info,
				 const struct fb_image *image);

void drm_fb_helper_cfb_fillrect(struct fb_info *info,
				const struct fb_fillrect *rect);
void drm_fb_helper_cfb_copyarea(struct fb_info *info,
				const struct fb_copyarea *area);
void drm_fb_helper_cfb_imageblit(struct fb_info *info,
				 const struct fb_image *image);

void drm_fb_helper_set_suspend(struct drm_fb_helper *fb_helper, bool suspend);
void drm_fb_helper_set_suspend_unlocked(struct drm_fb_helper *fb_helper,
					bool suspend);

int drm_fb_helper_setcmap(struct fb_cmap *cmap, struct fb_info *info);

int drm_fb_helper_hotplug_event(struct drm_fb_helper *fb_helper);
int drm_fb_helper_initial_config(struct drm_fb_helper *fb_helper, int bpp_sel);
int drm_fb_helper_single_add_all_connectors(struct drm_fb_helper *fb_helper);
int drm_fb_helper_debug_enter(struct fb_info *info);
int drm_fb_helper_debug_leave(struct fb_info *info);
struct drm_display_mode *
drm_has_preferred_mode(struct drm_fb_helper_connector *fb_connector,
			int width, int height);
struct drm_display_mode *
drm_pick_cmdline_mode(struct drm_fb_helper_connector *fb_helper_conn,
		      int width, int height);

int drm_fb_helper_add_one_connector(struct drm_fb_helper *fb_helper, struct drm_connector *connector);
int drm_fb_helper_remove_one_connector(struct drm_fb_helper *fb_helper,
				       struct drm_connector *connector);
#else
static inline void drm_fb_helper_prepare(struct drm_device *dev,
					struct drm_fb_helper *helper,
					const struct drm_fb_helper_funcs *funcs)
{
}

static inline int drm_fb_helper_init(struct drm_device *dev,
		       struct drm_fb_helper *helper, int crtc_count,
		       int max_conn)
{
	return 0;
}

static inline void drm_fb_helper_fini(struct drm_fb_helper *helper)
{
}

static inline int drm_fb_helper_blank(int blank, struct fb_info *info)
{
	return 0;
}

static inline int drm_fb_helper_pan_display(struct fb_var_screeninfo *var,
					    struct fb_info *info)
{
	return 0;
}

static inline int drm_fb_helper_set_par(struct fb_info *info)
{
	return 0;
}

static inline int drm_fb_helper_check_var(struct fb_var_screeninfo *var,
					  struct fb_info *info)
{
	return 0;
}

static inline int
drm_fb_helper_restore_fbdev_mode_unlocked(struct drm_fb_helper *fb_helper)
{
	return 0;
}

static inline struct fb_info *
drm_fb_helper_alloc_fbi(struct drm_fb_helper *fb_helper)
{
	return NULL;
}

static inline void drm_fb_helper_unregister_fbi(struct drm_fb_helper *fb_helper)
{
}
static inline void drm_fb_helper_release_fbi(struct drm_fb_helper *fb_helper)
{
}

static inline void drm_fb_helper_fill_var(struct fb_info *info,
					  struct drm_fb_helper *fb_helper,
					  uint32_t fb_width, uint32_t fb_height)
{
}

static inline void drm_fb_helper_fill_fix(struct fb_info *info, uint32_t pitch,
					  uint32_t depth)
{
}

static inline int drm_fb_helper_setcmap(struct fb_cmap *cmap,
					struct fb_info *info)
{
	return 0;
}

static inline void drm_fb_helper_unlink_fbi(struct drm_fb_helper *fb_helper)
{
}

static inline void drm_fb_helper_deferred_io(struct fb_info *info,
					     struct list_head *pagelist)
{
}

static inline ssize_t drm_fb_helper_sys_read(struct fb_info *info,
					     char __user *buf, size_t count,
					     loff_t *ppos)
{
	return -ENODEV;
}

static inline ssize_t drm_fb_helper_sys_write(struct fb_info *info,
					      const char __user *buf,
					      size_t count, loff_t *ppos)
{
	return -ENODEV;
}

static inline void drm_fb_helper_sys_fillrect(struct fb_info *info,
					      const struct fb_fillrect *rect)
{
}

static inline void drm_fb_helper_sys_copyarea(struct fb_info *info,
					      const struct fb_copyarea *area)
{
}

static inline void drm_fb_helper_sys_imageblit(struct fb_info *info,
					       const struct fb_image *image)
{
}

static inline void drm_fb_helper_cfb_fillrect(struct fb_info *info,
					      const struct fb_fillrect *rect)
{
}

static inline void drm_fb_helper_cfb_copyarea(struct fb_info *info,
					      const struct fb_copyarea *area)
{
}

static inline void drm_fb_helper_cfb_imageblit(struct fb_info *info,
					       const struct fb_image *image)
{
}

static inline void drm_fb_helper_set_suspend(struct drm_fb_helper *fb_helper,
					     bool suspend)
{
}

static inline void
drm_fb_helper_set_suspend_unlocked(struct drm_fb_helper *fb_helper, bool suspend)
{
}

static inline int drm_fb_helper_hotplug_event(struct drm_fb_helper *fb_helper)
{
	return 0;
}

static inline int drm_fb_helper_initial_config(struct drm_fb_helper *fb_helper,
					       int bpp_sel)
{
	return 0;
}

static inline int
drm_fb_helper_single_add_all_connectors(struct drm_fb_helper *fb_helper)
{
	return 0;
}

static inline int drm_fb_helper_debug_enter(struct fb_info *info)
{
	return 0;
}

static inline int drm_fb_helper_debug_leave(struct fb_info *info)
{
	return 0;
}

static inline struct drm_display_mode *
drm_has_preferred_mode(struct drm_fb_helper_connector *fb_connector,
		       int width, int height)
{
	return NULL;
}

static inline struct drm_display_mode *
drm_pick_cmdline_mode(struct drm_fb_helper_connector *fb_helper_conn,
		      int width, int height)
{
	return NULL;
}

static inline int
drm_fb_helper_add_one_connector(struct drm_fb_helper *fb_helper,
				struct drm_connector *connector)
{
	return 0;
}

static inline int
drm_fb_helper_remove_one_connector(struct drm_fb_helper *fb_helper,
				   struct drm_connector *connector)
{
	return 0;
}

#endif

static inline int
drm_fb_helper_remove_conflicting_framebuffers(struct apertures_struct *a,
					      const char *name, bool primary)
{
#if IS_REACHABLE(CONFIG_FB)
	return remove_conflicting_framebuffers(a, name, primary);
#else
	return 0;
#endif
}

#endif
