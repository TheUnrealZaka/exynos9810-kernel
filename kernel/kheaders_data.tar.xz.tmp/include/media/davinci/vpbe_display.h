
#ifndef VPBE_DISPLAY_H
#define VPBE_DISPLAY_H


#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <media/v4l2-fh.h>
#include <media/videobuf2-v4l2.h>
#include <media/videobuf2-dma-contig.h>
#include <media/davinci/vpbe_types.h>
#include <media/davinci/vpbe_osd.h>
#include <media/davinci/vpbe.h>

#define VPBE_DISPLAY_MAX_DEVICES 2

enum vpbe_display_device_id {
	VPBE_DISPLAY_DEVICE_0,
	VPBE_DISPLAY_DEVICE_1
};

#define VPBE_DISPLAY_DRV_NAME	"vpbe-display"

#define VPBE_DISPLAY_MAJOR_RELEASE              1
#define VPBE_DISPLAY_MINOR_RELEASE              0
#define VPBE_DISPLAY_BUILD                      1
#define VPBE_DISPLAY_VERSION_CODE ((VPBE_DISPLAY_MAJOR_RELEASE << 16) | \
	(VPBE_DISPLAY_MINOR_RELEASE << 8)  | \
	VPBE_DISPLAY_BUILD)

#define VPBE_DISPLAY_VALID_FIELD(field)   ((V4L2_FIELD_NONE == field) || \
	 (V4L2_FIELD_ANY == field) || (V4L2_FIELD_INTERLACED == field))


#define VPBE_DISPLAY_H_EXP_RATIO_N	9
#define VPBE_DISPLAY_H_EXP_RATIO_D	8
#define VPBE_DISPLAY_V_EXP_RATIO_N	6
#define VPBE_DISPLAY_V_EXP_RATIO_D	5


#define VPBE_DISPLAY_ZOOM_4X	4
#define VPBE_DISPLAY_ZOOM_2X	2


struct display_layer_info {
	int enable;
	
	enum osd_layer id;
	struct osd_layer_config config;
	enum osd_zoom_factor h_zoom;
	enum osd_zoom_factor v_zoom;
	enum osd_h_exp_ratio h_exp;
	enum osd_v_exp_ratio v_exp;
};

struct vpbe_disp_buffer {
	struct vb2_v4l2_buffer vb;
	struct list_head list;
};


struct vpbe_layer {
	
	struct vpbe_display *disp_dev;
	
	struct vpbe_disp_buffer *cur_frm;
	
	struct vpbe_disp_buffer *next_frm;
	
	struct vb2_queue buffer_queue;
	
	struct list_head dma_queue;
	
	spinlock_t irqlock;
	
	
	struct video_device video_dev;
	
	struct v4l2_pix_format pix_fmt;
	enum v4l2_field buf_field;
	
	struct display_layer_info layer_info;
	
	unsigned char window_enable;
	
	unsigned int usrs;
	
	unsigned int field_id;
	
	enum vpbe_display_device_id device_id;
	
	struct mutex opslock;
	u8 layer_first_int;
};


struct vpbe_display {
	
	
	spinlock_t dma_queue_lock;
	
	unsigned int cbcr_ofst;
	struct vpbe_layer *dev[VPBE_DISPLAY_MAX_DEVICES];
	struct vpbe_device *vpbe_dev;
	struct osd_state *osd_device;
};

struct buf_config_params {
	unsigned char min_numbuffers;
	unsigned char numbuffers[VPBE_DISPLAY_MAX_DEVICES];
	unsigned int min_bufsize[VPBE_DISPLAY_MAX_DEVICES];
	unsigned int layer_bufsize[VPBE_DISPLAY_MAX_DEVICES];
};

#endif	
