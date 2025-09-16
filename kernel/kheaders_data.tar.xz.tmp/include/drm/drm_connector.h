

#ifndef __DRM_CONNECTOR_H__
#define __DRM_CONNECTOR_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>

#include <uapi/drm/drm_mode.h>

struct drm_device;

struct drm_connector_helper_funcs;
struct drm_device;
struct drm_crtc;
struct drm_encoder;
struct drm_property;
struct drm_property_blob;
struct edid;

enum drm_connector_force {
	DRM_FORCE_UNSPECIFIED,
	DRM_FORCE_OFF,
	DRM_FORCE_ON,         
	DRM_FORCE_ON_DIGITAL, 
};


enum drm_connector_status {
	
	connector_status_connected = 1,
	
	connector_status_disconnected = 2,
	
	connector_status_unknown = 3,
};

enum subpixel_order {
	SubPixelUnknown = 0,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone,
};


struct drm_display_info {
	
	char name[DRM_DISPLAY_INFO_LEN];

	
        unsigned int width_mm;
	
	unsigned int height_mm;

	
	unsigned int pixel_clock;
	
	unsigned int bpc;

	
	enum subpixel_order subpixel_order;

#define DRM_COLOR_FORMAT_RGB444		(1<<0)
#define DRM_COLOR_FORMAT_YCRCB444	(1<<1)
#define DRM_COLOR_FORMAT_YCRCB422	(1<<2)

	
	u32 color_formats;

	
	const u32 *bus_formats;
	
	unsigned int num_bus_formats;

#define DRM_BUS_FLAG_DE_LOW		(1<<0)
#define DRM_BUS_FLAG_DE_HIGH		(1<<1)

#define DRM_BUS_FLAG_PIXDATA_POSEDGE	(1<<2)

#define DRM_BUS_FLAG_PIXDATA_NEGEDGE	(1<<3)

	
	u32 bus_flags;

	
	int max_tmds_clock;

	
	bool dvi_dual;

	
	u8 edid_hdmi_dc_modes;

	
	u8 cea_rev;
};

int drm_display_info_set_bus_formats(struct drm_display_info *info,
				     const u32 *formats,
				     unsigned int num_formats);


struct drm_connector_state {
	struct drm_connector *connector;

	
	struct drm_crtc *crtc;

	struct drm_encoder *best_encoder;

	struct drm_atomic_state *state;
};


struct drm_connector_funcs {
	
	int (*dpms)(struct drm_connector *connector, int mode);

	
	void (*reset)(struct drm_connector *connector);

	
	enum drm_connector_status (*detect)(struct drm_connector *connector,
					    bool force);

	
	void (*force)(struct drm_connector *connector);

	
	int (*fill_modes)(struct drm_connector *connector, uint32_t max_width, uint32_t max_height);

	
	int (*set_property)(struct drm_connector *connector, struct drm_property *property,
			     uint64_t val);

	
	int (*late_register)(struct drm_connector *connector);

	
	void (*early_unregister)(struct drm_connector *connector);

	
	void (*destroy)(struct drm_connector *connector);

	
	struct drm_connector_state *(*atomic_duplicate_state)(struct drm_connector *connector);

	
	void (*atomic_destroy_state)(struct drm_connector *connector,
				     struct drm_connector_state *state);

	
	int (*atomic_set_property)(struct drm_connector *connector,
				   struct drm_connector_state *state,
				   struct drm_property *property,
				   uint64_t val);

	
	int (*atomic_get_property)(struct drm_connector *connector,
				   const struct drm_connector_state *state,
				   struct drm_property *property,
				   uint64_t *val);
};


struct drm_cmdline_mode {
	bool specified;
	bool refresh_specified;
	bool bpp_specified;
	int xres, yres;
	int bpp;
	int refresh;
	bool rb;
	bool interlace;
	bool cvt;
	bool margins;
	enum drm_connector_force force;
};


struct drm_connector {
	struct drm_device *dev;
	struct device *kdev;
	struct device_attribute *attr;
	struct list_head head;

	struct drm_mode_object base;

	char *name;

	
	struct mutex mutex;

	
	unsigned index;

	int connector_type;
	int connector_type_id;
	bool interlace_allowed;
	bool doublescan_allowed;
	bool stereo_allowed;
	
	bool registered;
	struct list_head modes; 

	enum drm_connector_status status;

	
	struct list_head probed_modes;

	
	struct drm_display_info display_info;
	const struct drm_connector_funcs *funcs;

	struct drm_property_blob *edid_blob_ptr;
	struct drm_object_properties properties;

	
	struct drm_property_blob *path_blob_ptr;

	
	struct drm_property_blob *tile_blob_ptr;



#define DRM_CONNECTOR_POLL_HPD (1 << 0)

#define DRM_CONNECTOR_POLL_CONNECT (1 << 1)


#define DRM_CONNECTOR_POLL_DISCONNECT (1 << 2)

	
	uint8_t polled;

	
	int dpms;

	const struct drm_connector_helper_funcs *helper_private;

	
	struct drm_cmdline_mode cmdline_mode;
	enum drm_connector_force force;
	bool override_edid;

#define DRM_CONNECTOR_MAX_ENCODER 3
	uint32_t encoder_ids[DRM_CONNECTOR_MAX_ENCODER];
	struct drm_encoder *encoder; 

#define MAX_ELD_BYTES	128
	
	uint8_t eld[MAX_ELD_BYTES];
	bool latency_present[2];
	int video_latency[2];	
	int audio_latency[2];
	int null_edid_counter; 
	unsigned bad_edid_counter;

	
	bool edid_corrupt;

	struct dentry *debugfs_entry;

	struct drm_connector_state *state;

	
	bool has_tile;
	struct drm_tile_group *tile_group;
	bool tile_is_single_monitor;

	uint8_t num_h_tile, num_v_tile;
	uint8_t tile_h_loc, tile_v_loc;
	uint16_t tile_h_size, tile_v_size;
};

#define obj_to_connector(x) container_of(x, struct drm_connector, base)

int drm_connector_init(struct drm_device *dev,
		       struct drm_connector *connector,
		       const struct drm_connector_funcs *funcs,
		       int connector_type);
int drm_connector_register(struct drm_connector *connector);
void drm_connector_unregister(struct drm_connector *connector);
int drm_mode_connector_attach_encoder(struct drm_connector *connector,
				      struct drm_encoder *encoder);

void drm_connector_cleanup(struct drm_connector *connector);
static inline unsigned drm_connector_index(struct drm_connector *connector)
{
	return connector->index;
}


static inline struct drm_connector *drm_connector_lookup(struct drm_device *dev,
		uint32_t id)
{
	struct drm_mode_object *mo;
	mo = drm_mode_object_find(dev, id, DRM_MODE_OBJECT_CONNECTOR);
	return mo ? obj_to_connector(mo) : NULL;
}


static inline void drm_connector_reference(struct drm_connector *connector)
{
	drm_mode_object_reference(&connector->base);
}


static inline void drm_connector_unreference(struct drm_connector *connector)
{
	drm_mode_object_unreference(&connector->base);
}

const char *drm_get_connector_status_name(enum drm_connector_status status);
const char *drm_get_subpixel_order_name(enum subpixel_order order);
const char *drm_get_dpms_name(int val);
const char *drm_get_dvi_i_subconnector_name(int val);
const char *drm_get_dvi_i_select_name(int val);
const char *drm_get_tv_subconnector_name(int val);
const char *drm_get_tv_select_name(int val);

int drm_mode_create_dvi_i_properties(struct drm_device *dev);
int drm_mode_create_tv_properties(struct drm_device *dev,
				  unsigned int num_modes,
				  const char * const modes[]);
int drm_mode_create_scaling_mode_property(struct drm_device *dev);
int drm_mode_create_aspect_ratio_property(struct drm_device *dev);
int drm_mode_create_suggested_offset_properties(struct drm_device *dev);

int drm_mode_connector_set_path_property(struct drm_connector *connector,
					 const char *path);
int drm_mode_connector_set_tile_property(struct drm_connector *connector);
int drm_mode_connector_update_edid_property(struct drm_connector *connector,
					    const struct edid *edid);


#define drm_for_each_connector(connector, dev) \
	for (assert_drm_connector_list_read_locked(&(dev)->mode_config),	\
	     connector = list_first_entry(&(dev)->mode_config.connector_list,	\
					  struct drm_connector, head);		\
	     &connector->head != (&(dev)->mode_config.connector_list);		\
	     connector = list_next_entry(connector, head))

#endif
