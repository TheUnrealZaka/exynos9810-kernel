
#ifndef __DRM_CRTC_H__
#define __DRM_CRTC_H__

#include <linux/i2c.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/idr.h>
#include <linux/fb.h>
#include <linux/hdmi.h>
#include <linux/media-bus-format.h>
#include <uapi/drm/drm_mode.h>
#include <uapi/drm/drm_fourcc.h>
#include <drm/drm_modeset_lock.h>
#include <drm/drm_rect.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_modes.h>
#include <drm/drm_connector.h>
#include <drm/drm_encoder.h>
#include <drm/drm_property.h>
#include <drm/drm_bridge.h>
#include <drm/drm_edid.h>
#include <drm/drm_plane.h>
#include <drm/drm_blend.h>
#include <drm/drm_color_mgmt.h>

struct drm_device;
struct drm_mode_set;
struct drm_file;
struct drm_clip_rect;
struct device_node;
struct fence;
struct edid;

static inline int64_t U642I64(uint64_t val)
{
	return (int64_t)*((int64_t *)&val);
}
static inline uint64_t I642U64(int64_t val)
{
	return (uint64_t)*((uint64_t *)&val);
}


struct drm_tile_group {
	struct kref refcount;
	struct drm_device *dev;
	int id;
	u8 group_data[8];
};

struct drm_crtc;
struct drm_encoder;
struct drm_pending_vblank_event;
struct drm_plane;
struct drm_bridge;
struct drm_atomic_state;

struct drm_crtc_helper_funcs;
struct drm_encoder_helper_funcs;
struct drm_plane_helper_funcs;


struct drm_crtc_state {
	struct drm_crtc *crtc;

	bool enable;
	bool active;

	
	bool planes_changed : 1;
	bool mode_changed : 1;
	bool active_changed : 1;
	bool connectors_changed : 1;
	bool zpos_changed : 1;
	bool color_mgmt_changed : 1;

	
	u32 plane_mask;

	u32 connector_mask;
	u32 encoder_mask;

	
	u32 last_vblank_count;

	
	struct drm_display_mode adjusted_mode;

	struct drm_display_mode mode;

	
	struct drm_property_blob *mode_blob;

	
	struct drm_property_blob *degamma_lut;
	struct drm_property_blob *ctm;
	struct drm_property_blob *gamma_lut;

	
	struct drm_pending_vblank_event *event;

	struct drm_atomic_state *state;
};


struct drm_crtc_funcs {
	
	void (*reset)(struct drm_crtc *crtc);

	
	int (*cursor_set)(struct drm_crtc *crtc, struct drm_file *file_priv,
			  uint32_t handle, uint32_t width, uint32_t height);

	
	int (*cursor_set2)(struct drm_crtc *crtc, struct drm_file *file_priv,
			   uint32_t handle, uint32_t width, uint32_t height,
			   int32_t hot_x, int32_t hot_y);

	
	int (*cursor_move)(struct drm_crtc *crtc, int x, int y);

	
	int (*gamma_set)(struct drm_crtc *crtc, u16 *r, u16 *g, u16 *b,
			 uint32_t size);

	
	void (*destroy)(struct drm_crtc *crtc);

	
	int (*set_config)(struct drm_mode_set *set);

	
	int (*page_flip)(struct drm_crtc *crtc,
			 struct drm_framebuffer *fb,
			 struct drm_pending_vblank_event *event,
			 uint32_t flags);

	
	int (*page_flip_target)(struct drm_crtc *crtc,
				struct drm_framebuffer *fb,
				struct drm_pending_vblank_event *event,
				uint32_t flags, uint32_t target);

	
	int (*set_property)(struct drm_crtc *crtc,
			    struct drm_property *property, uint64_t val);

	
	struct drm_crtc_state *(*atomic_duplicate_state)(struct drm_crtc *crtc);

	
	void (*atomic_destroy_state)(struct drm_crtc *crtc,
				     struct drm_crtc_state *state);

	
	int (*atomic_set_property)(struct drm_crtc *crtc,
				   struct drm_crtc_state *state,
				   struct drm_property *property,
				   uint64_t val);
	
	int (*atomic_get_property)(struct drm_crtc *crtc,
				   const struct drm_crtc_state *state,
				   struct drm_property *property,
				   uint64_t *val);

	
	int (*late_register)(struct drm_crtc *crtc);

	
	void (*early_unregister)(struct drm_crtc *crtc);
};


struct drm_crtc {
	struct drm_device *dev;
	struct device_node *port;
	struct list_head head;

	char *name;

	
	struct drm_modeset_lock mutex;

	struct drm_mode_object base;

	
	struct drm_plane *primary;
	struct drm_plane *cursor;

	
	unsigned index;

	
	int cursor_x;
	int cursor_y;

	bool enabled;

	
	struct drm_display_mode mode;

	
	struct drm_display_mode hwmode;

	int x, y;
	const struct drm_crtc_funcs *funcs;

	
	uint32_t gamma_size;
	uint16_t *gamma_store;

	
	const struct drm_crtc_helper_funcs *helper_private;

	struct drm_object_properties properties;

	
	struct drm_crtc_state *state;

	
	struct list_head commit_list;

	
	spinlock_t commit_lock;

	
	struct drm_modeset_acquire_ctx *acquire_ctx;

	
	unsigned int fence_context;

	

	spinlock_t fence_lock;
	
	unsigned long fence_seqno;

	
	char timeline_name[32];
};


struct drm_mode_set {
	struct drm_framebuffer *fb;
	struct drm_crtc *crtc;
	struct drm_display_mode *mode;

	uint32_t x;
	uint32_t y;

	struct drm_connector **connectors;
	size_t num_connectors;
};


struct drm_mode_config_funcs {
	
	struct drm_framebuffer *(*fb_create)(struct drm_device *dev,
					     struct drm_file *file_priv,
					     const struct drm_mode_fb_cmd2 *mode_cmd);

	
	void (*output_poll_changed)(struct drm_device *dev);

	
	int (*atomic_check)(struct drm_device *dev,
			    struct drm_atomic_state *state);

	
	int (*atomic_commit)(struct drm_device *dev,
			     struct drm_atomic_state *state,
			     bool nonblock);

	
	struct drm_atomic_state *(*atomic_state_alloc)(struct drm_device *dev);

	
	void (*atomic_state_clear)(struct drm_atomic_state *state);

	
	void (*atomic_state_free)(struct drm_atomic_state *state);
};


struct drm_mode_config {
	struct mutex mutex; 
	struct drm_modeset_lock connection_mutex; 
	struct drm_modeset_acquire_ctx *acquire_ctx; 

	
	struct mutex idr_mutex;

	
	struct idr crtc_idr;

	
	struct idr tile_idr;

	struct mutex fb_lock; 
	int num_fb;
	struct list_head fb_list;

	
	int num_connector;
	
	struct ida connector_ida;
	
	struct list_head connector_list;
	int num_encoder;
	struct list_head encoder_list;

	
	int num_overlay_plane;
	int num_total_plane;
	struct list_head plane_list;

	int num_crtc;
	struct list_head crtc_list;

	struct list_head property_list;

	int min_width, min_height;
	int max_width, max_height;
	const struct drm_mode_config_funcs *funcs;
	resource_size_t fb_base;

	
	bool poll_enabled;
	bool poll_running;
	bool delayed_event;
	struct delayed_work output_poll_work;

	struct mutex blob_lock;

	
	struct list_head property_blob_list;
	
	struct drm_property *edid_property;
	
	struct drm_property *dpms_property;
	
	struct drm_property *path_property;
	
	struct drm_property *tile_property;
	
	struct drm_property *plane_type_property;
	
	struct drm_property *rotation_property;
	
	struct drm_property *prop_src_x;
	
	struct drm_property *prop_src_y;
	
	struct drm_property *prop_src_w;
	
	struct drm_property *prop_src_h;
	
	struct drm_property *prop_crtc_x;
	
	struct drm_property *prop_crtc_y;
	
	struct drm_property *prop_crtc_w;
	
	struct drm_property *prop_crtc_h;
	
	struct drm_property *prop_fb_id;
	
	struct drm_property *prop_in_fence_fd;
	
	struct drm_property *prop_out_fence_ptr;
	
	struct drm_property *prop_crtc_id;
	
	struct drm_property *prop_active;
	
	struct drm_property *prop_mode_id;

	
	struct drm_property *dvi_i_subconnector_property;
	
	struct drm_property *dvi_i_select_subconnector_property;

	
	struct drm_property *tv_subconnector_property;
	
	struct drm_property *tv_select_subconnector_property;
	
	struct drm_property *tv_mode_property;
	
	struct drm_property *tv_left_margin_property;
	
	struct drm_property *tv_right_margin_property;
	
	struct drm_property *tv_top_margin_property;
	
	struct drm_property *tv_bottom_margin_property;
	
	struct drm_property *tv_brightness_property;
	
	struct drm_property *tv_contrast_property;
	
	struct drm_property *tv_flicker_reduction_property;
	
	struct drm_property *tv_overscan_property;
	
	struct drm_property *tv_saturation_property;
	
	struct drm_property *tv_hue_property;

	
	struct drm_property *scaling_mode_property;
	
	struct drm_property *aspect_ratio_property;
	
	struct drm_property *degamma_lut_property;
	
	struct drm_property *degamma_lut_size_property;
	
	struct drm_property *ctm_property;
	
	struct drm_property *gamma_lut_property;
	
	struct drm_property *gamma_lut_size_property;

	
	struct drm_property *suggested_x_property;
	
	struct drm_property *suggested_y_property;

	
	uint32_t preferred_depth, prefer_shadow;

	
	bool async_page_flip;

	
	bool allow_fb_modifiers;

	
	uint32_t cursor_width, cursor_height;

	struct drm_mode_config_helper_funcs *helper_private;
};

#define obj_to_crtc(x) container_of(x, struct drm_crtc, base)

extern __printf(6, 7)
int drm_crtc_init_with_planes(struct drm_device *dev,
			      struct drm_crtc *crtc,
			      struct drm_plane *primary,
			      struct drm_plane *cursor,
			      const struct drm_crtc_funcs *funcs,
			      const char *name, ...);
extern void drm_crtc_cleanup(struct drm_crtc *crtc);


static inline unsigned int drm_crtc_index(const struct drm_crtc *crtc)
{
	return crtc->index;
}


static inline uint32_t drm_crtc_mask(struct drm_crtc *crtc)
{
	return 1 << drm_crtc_index(crtc);
}

extern void drm_crtc_get_hv_timing(const struct drm_display_mode *mode,
				   int *hdisplay, int *vdisplay);
extern int drm_crtc_force_disable(struct drm_crtc *crtc);
extern int drm_crtc_force_disable_all(struct drm_device *dev);

extern void drm_mode_config_init(struct drm_device *dev);
extern void drm_mode_config_reset(struct drm_device *dev);
extern void drm_mode_config_cleanup(struct drm_device *dev);

extern int drm_mode_set_config_internal(struct drm_mode_set *set);

extern struct drm_tile_group *drm_mode_create_tile_group(struct drm_device *dev,
							 char topology[8]);
extern struct drm_tile_group *drm_mode_get_tile_group(struct drm_device *dev,
					       char topology[8]);
extern void drm_mode_put_tile_group(struct drm_device *dev,
				   struct drm_tile_group *tg);


static inline struct drm_crtc *drm_crtc_find(struct drm_device *dev,
	uint32_t id)
{
	struct drm_mode_object *mo;
	mo = drm_mode_object_find(dev, id, DRM_MODE_OBJECT_CRTC);
	return mo ? obj_to_crtc(mo) : NULL;
}

#define drm_for_each_crtc(crtc, dev) \
	list_for_each_entry(crtc, &(dev)->mode_config.crtc_list, head)

static inline void
assert_drm_connector_list_read_locked(struct drm_mode_config *mode_config)
{
	
	WARN_ON(!mutex_is_locked(&mode_config->mutex) &&
		!drm_modeset_is_locked(&mode_config->connection_mutex));
}

#endif 
