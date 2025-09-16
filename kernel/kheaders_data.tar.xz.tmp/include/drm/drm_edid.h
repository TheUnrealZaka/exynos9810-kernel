
#ifndef __DRM_EDID_H__
#define __DRM_EDID_H__

#include <linux/types.h>

struct drm_device;
struct i2c_adapter;

#define EDID_LENGTH 128
#define DDC_ADDR 0x50
#define DDC_ADDR2 0x52 

#define CEA_EXT	    0x02
#define VTB_EXT	    0x10
#define DI_EXT	    0x40
#define LS_EXT	    0x50
#define MI_EXT	    0x60
#define DISPLAYID_EXT 0x70

struct est_timings {
	u8 t1;
	u8 t2;
	u8 mfg_rsvd;
} __attribute__((packed));


#define EDID_TIMING_ASPECT_SHIFT 6
#define EDID_TIMING_ASPECT_MASK  (0x3 << EDID_TIMING_ASPECT_SHIFT)


#define EDID_TIMING_VFREQ_SHIFT  0
#define EDID_TIMING_VFREQ_MASK   (0x3f << EDID_TIMING_VFREQ_SHIFT)

struct std_timing {
	u8 hsize; 
	u8 vfreq_aspect;
} __attribute__((packed));

#define DRM_EDID_PT_HSYNC_POSITIVE (1 << 1)
#define DRM_EDID_PT_VSYNC_POSITIVE (1 << 2)
#define DRM_EDID_PT_SEPARATE_SYNC  (3 << 3)
#define DRM_EDID_PT_STEREO         (1 << 5)
#define DRM_EDID_PT_INTERLACED     (1 << 7)


struct detailed_pixel_timing {
	u8 hactive_lo;
	u8 hblank_lo;
	u8 hactive_hblank_hi;
	u8 vactive_lo;
	u8 vblank_lo;
	u8 vactive_vblank_hi;
	u8 hsync_offset_lo;
	u8 hsync_pulse_width_lo;
	u8 vsync_offset_pulse_width_lo;
	u8 hsync_vsync_offset_pulse_width_hi;
	u8 width_mm_lo;
	u8 height_mm_lo;
	u8 width_height_mm_hi;
	u8 hborder;
	u8 vborder;
	u8 misc;
} __attribute__((packed));


struct detailed_data_string {
	u8 str[13];
} __attribute__((packed));

struct detailed_data_monitor_range {
	u8 min_vfreq;
	u8 max_vfreq;
	u8 min_hfreq_khz;
	u8 max_hfreq_khz;
	u8 pixel_clock_mhz; 
	u8 flags;
	union {
		struct {
			u8 reserved;
			u8 hfreq_start_khz; 
			u8 c; 
			__le16 m;
			u8 k;
			u8 j; 
		} __attribute__((packed)) gtf2;
		struct {
			u8 version;
			u8 data1; 
			u8 data2; 
			u8 supported_aspects;
			u8 flags; 
			u8 supported_scalings;
			u8 preferred_refresh;
		} __attribute__((packed)) cvt;
	} __attribute__((packed)) formula;
} __attribute__((packed));

struct detailed_data_wpindex {
	u8 white_yx_lo; 
	u8 white_x_hi;
	u8 white_y_hi;
	u8 gamma; 
} __attribute__((packed));

struct detailed_data_color_point {
	u8 windex1;
	u8 wpindex1[3];
	u8 windex2;
	u8 wpindex2[3];
} __attribute__((packed));

struct cvt_timing {
	u8 code[3];
} __attribute__((packed));

struct detailed_non_pixel {
	u8 pad1;
	u8 type; 
	u8 pad2;
	union {
		struct detailed_data_string str;
		struct detailed_data_monitor_range range;
		struct detailed_data_wpindex color;
		struct std_timing timings[6];
		struct cvt_timing cvt[4];
	} __attribute__((packed)) data;
} __attribute__((packed));

#define EDID_DETAIL_EST_TIMINGS 0xf7
#define EDID_DETAIL_CVT_3BYTE 0xf8
#define EDID_DETAIL_COLOR_MGMT_DATA 0xf9
#define EDID_DETAIL_STD_MODES 0xfa
#define EDID_DETAIL_MONITOR_CPDATA 0xfb
#define EDID_DETAIL_MONITOR_NAME 0xfc
#define EDID_DETAIL_MONITOR_RANGE 0xfd
#define EDID_DETAIL_MONITOR_STRING 0xfe
#define EDID_DETAIL_MONITOR_SERIAL 0xff

struct detailed_timing {
	__le16 pixel_clock; 
	union {
		struct detailed_pixel_timing pixel_data;
		struct detailed_non_pixel other_data;
	} __attribute__((packed)) data;
} __attribute__((packed));

#define DRM_EDID_INPUT_SERRATION_VSYNC (1 << 0)
#define DRM_EDID_INPUT_SYNC_ON_GREEN   (1 << 1)
#define DRM_EDID_INPUT_COMPOSITE_SYNC  (1 << 2)
#define DRM_EDID_INPUT_SEPARATE_SYNCS  (1 << 3)
#define DRM_EDID_INPUT_BLANK_TO_BLACK  (1 << 4)
#define DRM_EDID_INPUT_VIDEO_LEVEL     (3 << 5)
#define DRM_EDID_INPUT_DIGITAL         (1 << 7)
#define DRM_EDID_DIGITAL_DEPTH_MASK    (7 << 4)
#define DRM_EDID_DIGITAL_DEPTH_UNDEF   (0 << 4)
#define DRM_EDID_DIGITAL_DEPTH_6       (1 << 4)
#define DRM_EDID_DIGITAL_DEPTH_8       (2 << 4)
#define DRM_EDID_DIGITAL_DEPTH_10      (3 << 4)
#define DRM_EDID_DIGITAL_DEPTH_12      (4 << 4)
#define DRM_EDID_DIGITAL_DEPTH_14      (5 << 4)
#define DRM_EDID_DIGITAL_DEPTH_16      (6 << 4)
#define DRM_EDID_DIGITAL_DEPTH_RSVD    (7 << 4)
#define DRM_EDID_DIGITAL_TYPE_UNDEF    (0)
#define DRM_EDID_DIGITAL_TYPE_DVI      (1)
#define DRM_EDID_DIGITAL_TYPE_HDMI_A   (2)
#define DRM_EDID_DIGITAL_TYPE_HDMI_B   (3)
#define DRM_EDID_DIGITAL_TYPE_MDDI     (4)
#define DRM_EDID_DIGITAL_TYPE_DP       (5)

#define DRM_EDID_FEATURE_DEFAULT_GTF      (1 << 0)
#define DRM_EDID_FEATURE_PREFERRED_TIMING (1 << 1)
#define DRM_EDID_FEATURE_STANDARD_COLOR   (1 << 2)

#define DRM_EDID_FEATURE_DISPLAY_TYPE     (3 << 3) 

#define DRM_EDID_FEATURE_COLOR_MASK	  (3 << 3)
#define DRM_EDID_FEATURE_RGB		  (0 << 3)
#define DRM_EDID_FEATURE_RGB_YCRCB444	  (1 << 3)
#define DRM_EDID_FEATURE_RGB_YCRCB422	  (2 << 3)
#define DRM_EDID_FEATURE_RGB_YCRCB	  (3 << 3) 

#define DRM_EDID_FEATURE_PM_ACTIVE_OFF    (1 << 5)
#define DRM_EDID_FEATURE_PM_SUSPEND       (1 << 6)
#define DRM_EDID_FEATURE_PM_STANDBY       (1 << 7)

#define DRM_EDID_HDMI_DC_48               (1 << 6)
#define DRM_EDID_HDMI_DC_36               (1 << 5)
#define DRM_EDID_HDMI_DC_30               (1 << 4)
#define DRM_EDID_HDMI_DC_Y444             (1 << 3)


#define DRM_ELD_HEADER_BLOCK_SIZE	4

#define DRM_ELD_VER			0
# define DRM_ELD_VER_SHIFT		3
# define DRM_ELD_VER_MASK		(0x1f << 3)
# define DRM_ELD_VER_CEA861D		(2 << 3) 
# define DRM_ELD_VER_CANNED		(0x1f << 3)

#define DRM_ELD_BASELINE_ELD_LEN	2	


#define DRM_ELD_CEA_EDID_VER_MNL	4
# define DRM_ELD_CEA_EDID_VER_SHIFT	5
# define DRM_ELD_CEA_EDID_VER_MASK	(7 << 5)
# define DRM_ELD_CEA_EDID_VER_NONE	(0 << 5)
# define DRM_ELD_CEA_EDID_VER_CEA861	(1 << 5)
# define DRM_ELD_CEA_EDID_VER_CEA861A	(2 << 5)
# define DRM_ELD_CEA_EDID_VER_CEA861BCD	(3 << 5)
# define DRM_ELD_MNL_SHIFT		0
# define DRM_ELD_MNL_MASK		(0x1f << 0)

#define DRM_ELD_SAD_COUNT_CONN_TYPE	5
# define DRM_ELD_SAD_COUNT_SHIFT	4
# define DRM_ELD_SAD_COUNT_MASK		(0xf << 4)
# define DRM_ELD_CONN_TYPE_SHIFT	2
# define DRM_ELD_CONN_TYPE_MASK		(3 << 2)
# define DRM_ELD_CONN_TYPE_HDMI		(0 << 2)
# define DRM_ELD_CONN_TYPE_DP		(1 << 2)
# define DRM_ELD_SUPPORTS_AI		(1 << 1)
# define DRM_ELD_SUPPORTS_HDCP		(1 << 0)

#define DRM_ELD_AUD_SYNCH_DELAY		6	
# define DRM_ELD_AUD_SYNCH_DELAY_MAX	0xfa	

#define DRM_ELD_SPEAKER			7
# define DRM_ELD_SPEAKER_RLRC		(1 << 6)
# define DRM_ELD_SPEAKER_FLRC		(1 << 5)
# define DRM_ELD_SPEAKER_RC		(1 << 4)
# define DRM_ELD_SPEAKER_RLR		(1 << 3)
# define DRM_ELD_SPEAKER_FC		(1 << 2)
# define DRM_ELD_SPEAKER_LFE		(1 << 1)
# define DRM_ELD_SPEAKER_FLR		(1 << 0)

#define DRM_ELD_PORT_ID			8	
# define DRM_ELD_PORT_ID_LEN		8

#define DRM_ELD_MANUFACTURER_NAME0	16
#define DRM_ELD_MANUFACTURER_NAME1	17

#define DRM_ELD_PRODUCT_CODE0		18
#define DRM_ELD_PRODUCT_CODE1		19

#define DRM_ELD_MONITOR_NAME_STRING	20	

#define DRM_ELD_CEA_SAD(mnl, sad)	(20 + (mnl) + 3 * (sad))

struct edid {
	u8 header[8];
	
	u8 mfg_id[2];
	u8 prod_code[2];
	u32 serial; 
	u8 mfg_week;
	u8 mfg_year;
	
	u8 version;
	u8 revision;
	
	u8 input;
	u8 width_cm;
	u8 height_cm;
	u8 gamma;
	u8 features;
	
	u8 red_green_lo;
	u8 black_white_lo;
	u8 red_x;
	u8 red_y;
	u8 green_x;
	u8 green_y;
	u8 blue_x;
	u8 blue_y;
	u8 white_x;
	u8 white_y;
	
	struct est_timings established_timings;
	
	struct std_timing standard_timings[8];
	
	struct detailed_timing detailed_timings[4];
	
	u8 extensions;
	
	u8 checksum;
} __attribute__((packed));

#define EDID_PRODUCT_ID(e) ((e)->prod_code[0] | ((e)->prod_code[1] << 8))


struct cea_sad {
	u8 format;
	u8 channels; 
	u8 freq;
	u8 byte2; 
};

struct drm_encoder;
struct drm_connector;
struct drm_display_mode;
struct hdmi_avi_infoframe;
struct hdmi_vendor_infoframe;

void drm_edid_to_eld(struct drm_connector *connector, struct edid *edid);
int drm_edid_to_sad(struct edid *edid, struct cea_sad **sads);
int drm_edid_to_speaker_allocation(struct edid *edid, u8 **sadb);
int drm_av_sync_delay(struct drm_connector *connector,
		      const struct drm_display_mode *mode);
struct drm_connector *drm_select_eld(struct drm_encoder *encoder);

#ifdef CONFIG_DRM_LOAD_EDID_FIRMWARE
int drm_load_edid_firmware(struct drm_connector *connector);
#else
static inline int drm_load_edid_firmware(struct drm_connector *connector)
{
	return 0;
}
#endif

int
drm_hdmi_avi_infoframe_from_display_mode(struct hdmi_avi_infoframe *frame,
					 const struct drm_display_mode *mode);
int
drm_hdmi_vendor_infoframe_from_display_mode(struct hdmi_vendor_infoframe *frame,
					    const struct drm_display_mode *mode);


static inline int drm_eld_mnl(const uint8_t *eld)
{
	return (eld[DRM_ELD_CEA_EDID_VER_MNL] & DRM_ELD_MNL_MASK) >> DRM_ELD_MNL_SHIFT;
}


static inline const uint8_t *drm_eld_sad(const uint8_t *eld)
{
	unsigned int ver, mnl;

	ver = (eld[DRM_ELD_VER] & DRM_ELD_VER_MASK) >> DRM_ELD_VER_SHIFT;
	if (ver != 2 && ver != 31)
		return NULL;

	mnl = drm_eld_mnl(eld);
	if (mnl > 16)
		return NULL;

	return eld + DRM_ELD_CEA_SAD(mnl, 0);
}


static inline int drm_eld_sad_count(const uint8_t *eld)
{
	return (eld[DRM_ELD_SAD_COUNT_CONN_TYPE] & DRM_ELD_SAD_COUNT_MASK) >>
		DRM_ELD_SAD_COUNT_SHIFT;
}


static inline int drm_eld_calc_baseline_block_size(const uint8_t *eld)
{
	return DRM_ELD_MONITOR_NAME_STRING - DRM_ELD_HEADER_BLOCK_SIZE +
		drm_eld_mnl(eld) + drm_eld_sad_count(eld) * 3;
}


static inline int drm_eld_size(const uint8_t *eld)
{
	return DRM_ELD_HEADER_BLOCK_SIZE + eld[DRM_ELD_BASELINE_ELD_LEN] * 4;
}


static inline u8 drm_eld_get_conn_type(const uint8_t *eld)
{
	return eld[DRM_ELD_SAD_COUNT_CONN_TYPE] & DRM_ELD_CONN_TYPE_MASK;
}

bool drm_probe_ddc(struct i2c_adapter *adapter);
struct edid *drm_do_get_edid(struct drm_connector *connector,
	int (*get_edid_block)(void *data, u8 *buf, unsigned int block,
			      size_t len),
	void *data);
struct edid *drm_get_edid(struct drm_connector *connector,
			  struct i2c_adapter *adapter);
struct edid *drm_get_edid_switcheroo(struct drm_connector *connector,
				     struct i2c_adapter *adapter);
struct edid *drm_edid_duplicate(const struct edid *edid);
int drm_add_edid_modes(struct drm_connector *connector, struct edid *edid);

u8 drm_match_cea_mode(const struct drm_display_mode *to_match);
enum hdmi_picture_aspect drm_get_cea_aspect_ratio(const u8 video_code);
bool drm_detect_hdmi_monitor(struct edid *edid);
bool drm_detect_monitor_audio(struct edid *edid);
bool drm_rgb_quant_range_selectable(struct edid *edid);
int drm_add_modes_noedid(struct drm_connector *connector,
			 int hdisplay, int vdisplay);
void drm_set_preferred_mode(struct drm_connector *connector,
			    int hpref, int vpref);

int drm_edid_header_is_valid(const u8 *raw_edid);
bool drm_edid_block_valid(u8 *raw_edid, int block, bool print_bad_edid,
			  bool *edid_corrupt);
bool drm_edid_is_valid(struct edid *edid);
void drm_edid_get_monitor_name(struct edid *edid, char *name,
			       int buflen);
struct drm_display_mode *drm_mode_find_dmt(struct drm_device *dev,
					   int hsize, int vsize, int fresh,
					   bool rb);

#endif 
