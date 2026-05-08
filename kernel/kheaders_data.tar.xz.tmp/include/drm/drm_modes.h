
#ifndef __DRM_MODES_H__
#define __DRM_MODES_H__

#include <linux/hdmi.h>

#include <drm/drm_mode_object.h>
#include <drm/drm_connector.h>

struct videomode;




enum drm_mode_status {
	MODE_OK = 0,
	MODE_HSYNC,
	MODE_VSYNC,
	MODE_H_ILLEGAL,
	MODE_V_ILLEGAL,
	MODE_BAD_WIDTH,
	MODE_NOMODE,
	MODE_NO_INTERLACE,
	MODE_NO_DBLESCAN,
	MODE_NO_VSCAN,
	MODE_MEM,
	MODE_VIRTUAL_X,
	MODE_VIRTUAL_Y,
	MODE_MEM_VIRT,
	MODE_NOCLOCK,
	MODE_CLOCK_HIGH,
	MODE_CLOCK_LOW,
	MODE_CLOCK_RANGE,
	MODE_BAD_HVALUE,
	MODE_BAD_VVALUE,
	MODE_BAD_VSCAN,
	MODE_HSYNC_NARROW,
	MODE_HSYNC_WIDE,
	MODE_HBLANK_NARROW,
	MODE_HBLANK_WIDE,
	MODE_VSYNC_NARROW,
	MODE_VSYNC_WIDE,
	MODE_VBLANK_NARROW,
	MODE_VBLANK_WIDE,
	MODE_PANEL,
	MODE_INTERLACE_WIDTH,
	MODE_ONE_WIDTH,
	MODE_ONE_HEIGHT,
	MODE_ONE_SIZE,
	MODE_NO_REDUCED,
	MODE_NO_STEREO,
	MODE_STALE = -3,
	MODE_BAD = -2,
	MODE_ERROR = -1
};

#define DRM_MODE_TYPE_CLOCK_CRTC_C (DRM_MODE_TYPE_CLOCK_C | \
				    DRM_MODE_TYPE_CRTC_C)

#define DRM_MODE(nm, t, c, hd, hss, hse, ht, hsk, vd, vss, vse, vt, vs, f) \
	.name = nm, .status = 0, .type = (t), .clock = (c), \
	.hdisplay = (hd), .hsync_start = (hss), .hsync_end = (hse), \
	.htotal = (ht), .hskew = (hsk), .vdisplay = (vd), \
	.vsync_start = (vss), .vsync_end = (vse), .vtotal = (vt), \
	.vscan = (vs), .flags = (f), \
	.base.type = DRM_MODE_OBJECT_MODE

#define CRTC_INTERLACE_HALVE_V	(1 << 0) 
#define CRTC_STEREO_DOUBLE	(1 << 1) 
#define CRTC_NO_DBLSCAN		(1 << 2) 
#define CRTC_NO_VSCAN		(1 << 3) 
#define CRTC_STEREO_DOUBLE_ONLY	(CRTC_STEREO_DOUBLE | CRTC_NO_DBLSCAN | CRTC_NO_VSCAN)

#define DRM_MODE_FLAG_3D_MAX	DRM_MODE_FLAG_3D_SIDE_BY_SIDE_HALF


struct drm_display_mode {
	
	struct list_head head;

	
	struct drm_mode_object base;

	
	char name[DRM_DISPLAY_MODE_LEN];

	
	enum drm_mode_status status;

	
	unsigned int type;

	
	int clock;		
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int hskew;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	int vscan;
	
	unsigned int flags;

	
	int width_mm;

	
	int height_mm;

	
	int crtc_clock;
	int crtc_hdisplay;
	int crtc_hblank_start;
	int crtc_hblank_end;
	int crtc_hsync_start;
	int crtc_hsync_end;
	int crtc_htotal;
	int crtc_hskew;
	int crtc_vdisplay;
	int crtc_vblank_start;
	int crtc_vblank_end;
	int crtc_vsync_start;
	int crtc_vsync_end;
	int crtc_vtotal;

	
	int *private;

	
	int private_flags;

	
	int vrefresh;

	
	int hsync;

	
	enum hdmi_picture_aspect picture_aspect_ratio;
};

#define obj_to_mode(x) container_of(x, struct drm_display_mode, base)


static inline bool drm_mode_is_stereo(const struct drm_display_mode *mode)
{
	return mode->flags & DRM_MODE_FLAG_3D_MASK;
}

struct drm_connector;
struct drm_cmdline_mode;

struct drm_display_mode *drm_mode_create(struct drm_device *dev);
void drm_mode_destroy(struct drm_device *dev, struct drm_display_mode *mode);
void drm_mode_convert_to_umode(struct drm_mode_modeinfo *out,
			       const struct drm_display_mode *in);
int drm_mode_convert_umode(struct drm_display_mode *out,
			   const struct drm_mode_modeinfo *in);
void drm_mode_probed_add(struct drm_connector *connector, struct drm_display_mode *mode);
void drm_mode_debug_printmodeline(const struct drm_display_mode *mode);

struct drm_display_mode *drm_cvt_mode(struct drm_device *dev,
				      int hdisplay, int vdisplay, int vrefresh,
				      bool reduced, bool interlaced,
				      bool margins);
struct drm_display_mode *drm_gtf_mode(struct drm_device *dev,
				      int hdisplay, int vdisplay, int vrefresh,
				      bool interlaced, int margins);
struct drm_display_mode *drm_gtf_mode_complex(struct drm_device *dev,
					      int hdisplay, int vdisplay,
					      int vrefresh, bool interlaced,
					      int margins,
					      int GTF_M, int GTF_2C,
					      int GTF_K, int GTF_2J);
void drm_display_mode_from_videomode(const struct videomode *vm,
				     struct drm_display_mode *dmode);
void drm_display_mode_to_videomode(const struct drm_display_mode *dmode,
				   struct videomode *vm);
void drm_bus_flags_from_videomode(const struct videomode *vm, u32 *bus_flags);
int of_get_drm_display_mode(struct device_node *np,
			    struct drm_display_mode *dmode, u32 *bus_flags,
			    int index);

void drm_mode_set_name(struct drm_display_mode *mode);
int drm_mode_hsync(const struct drm_display_mode *mode);
int drm_mode_vrefresh(const struct drm_display_mode *mode);

void drm_mode_set_crtcinfo(struct drm_display_mode *p,
			   int adjust_flags);
void drm_mode_copy(struct drm_display_mode *dst,
		   const struct drm_display_mode *src);
struct drm_display_mode *drm_mode_duplicate(struct drm_device *dev,
					    const struct drm_display_mode *mode);
bool drm_mode_equal(const struct drm_display_mode *mode1,
		    const struct drm_display_mode *mode2);
bool drm_mode_equal_no_clocks(const struct drm_display_mode *mode1,
			      const struct drm_display_mode *mode2);
bool drm_mode_equal_no_clocks_no_stereo(const struct drm_display_mode *mode1,
					const struct drm_display_mode *mode2);


enum drm_mode_status drm_mode_validate_basic(const struct drm_display_mode *mode);
enum drm_mode_status drm_mode_validate_size(const struct drm_display_mode *mode,
					    int maxX, int maxY);
void drm_mode_prune_invalid(struct drm_device *dev,
			    struct list_head *mode_list, bool verbose);
void drm_mode_sort(struct list_head *mode_list);
void drm_mode_connector_list_update(struct drm_connector *connector);


bool
drm_mode_parse_command_line_for_connector(const char *mode_option,
					  struct drm_connector *connector,
					  struct drm_cmdline_mode *mode);
struct drm_display_mode *
drm_mode_create_from_cmdline_mode(struct drm_device *dev,
				  struct drm_cmdline_mode *cmd);

#endif 
