

#ifndef __DRM_FRAMEBUFFER_H__
#define __DRM_FRAMEBUFFER_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>

struct drm_framebuffer;
struct drm_file;
struct drm_device;


struct drm_framebuffer_funcs {
	
	void (*destroy)(struct drm_framebuffer *framebuffer);

	
	int (*create_handle)(struct drm_framebuffer *fb,
			     struct drm_file *file_priv,
			     unsigned int *handle);
	
	int (*dirty)(struct drm_framebuffer *framebuffer,
		     struct drm_file *file_priv, unsigned flags,
		     unsigned color, struct drm_clip_rect *clips,
		     unsigned num_clips);
};


struct drm_framebuffer {
	
	struct drm_device *dev;
	
	struct list_head head;

	
	struct drm_mode_object base;
	
	const struct drm_framebuffer_funcs *funcs;
	
	unsigned int pitches[4];
	
	unsigned int offsets[4];
	
	uint64_t modifier[4];
	
	unsigned int width;
	
	unsigned int height;
	
	unsigned int depth;
	
	int bits_per_pixel;
	
	int flags;
	
	uint32_t pixel_format; 
	
	int hot_x;
	
	int hot_y;
	
	struct list_head filp_head;
};

#define obj_to_fb(x) container_of(x, struct drm_framebuffer, base)

int drm_framebuffer_init(struct drm_device *dev,
			 struct drm_framebuffer *fb,
			 const struct drm_framebuffer_funcs *funcs);
struct drm_framebuffer *drm_framebuffer_lookup(struct drm_device *dev,
					       uint32_t id);
void drm_framebuffer_remove(struct drm_framebuffer *fb);
void drm_framebuffer_cleanup(struct drm_framebuffer *fb);
void drm_framebuffer_unregister_private(struct drm_framebuffer *fb);


static inline void drm_framebuffer_reference(struct drm_framebuffer *fb)
{
	drm_mode_object_reference(&fb->base);
}


static inline void drm_framebuffer_unreference(struct drm_framebuffer *fb)
{
	drm_mode_object_unreference(&fb->base);
}


static inline uint32_t drm_framebuffer_read_refcount(struct drm_framebuffer *fb)
{
	return atomic_read(&fb->base.refcount.refcount);
}


#define drm_for_each_fb(fb, dev) \
	for (WARN_ON(!mutex_is_locked(&(dev)->mode_config.fb_lock)),		\
	     fb = list_first_entry(&(dev)->mode_config.fb_list,	\
					  struct drm_framebuffer, head);	\
	     &fb->head != (&(dev)->mode_config.fb_list);			\
	     fb = list_next_entry(fb, head))
#endif
