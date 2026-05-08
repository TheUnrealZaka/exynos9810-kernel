
#ifndef _VPBE_H
#define _VPBE_H

#include <linux/videodev2.h>
#include <linux/i2c.h>

#include <media/v4l2-dev.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-device.h>
#include <media/davinci/vpbe_osd.h>
#include <media/davinci/vpbe_venc.h>
#include <media/davinci/vpbe_types.h>


struct osd_config_info {
	char module_name[32];
};

struct vpbe_output {
	struct v4l2_output output;
	
	char *subdev_name;
	
	char *default_mode;
	
	unsigned int num_modes;
	struct vpbe_enc_mode_info *modes;
	
	u32 if_params;
};


struct encoder_config_info {
	char module_name[32];
	
	unsigned int is_i2c:1;
	
	struct i2c_board_info board_info;
};


struct amp_config_info {
	char module_name[32];
	
	unsigned int is_i2c:1;
	
	struct i2c_board_info board_info;
};


struct vpbe_config {
	char module_name[32];
	
	int i2c_adapter_id;
	struct osd_config_info osd;
	struct encoder_config_info venc;
	
	int num_ext_encoders;
	struct encoder_config_info *ext_encoders;
	
	struct amp_config_info *amp;
	unsigned int num_outputs;
	
	struct vpbe_output *outputs;
};

struct vpbe_device;

struct vpbe_device_ops {
	
	int (*g_cropcap)(struct vpbe_device *vpbe_dev,
			 struct v4l2_cropcap *cropcap);

	
	int (*enum_outputs)(struct vpbe_device *vpbe_dev,
			    struct v4l2_output *output);

	
	int (*set_output)(struct vpbe_device *vpbe_dev,
			 int index);

	
	unsigned int (*get_output)(struct vpbe_device *vpbe_dev);

	
	int (*s_dv_timings)(struct vpbe_device *vpbe_dev,
			   struct v4l2_dv_timings *dv_timings);

	
	int (*g_dv_timings)(struct vpbe_device *vpbe_dev,
			   struct v4l2_dv_timings *dv_timings);

	
	int (*enum_dv_timings)(struct vpbe_device *vpbe_dev,
			       struct v4l2_enum_dv_timings *timings_info);

	
	int (*s_std)(struct vpbe_device *vpbe_dev, v4l2_std_id std_id);

	
	int (*g_std)(struct vpbe_device *vpbe_dev, v4l2_std_id *std_id);

	
	int (*initialize)(struct device *dev, struct vpbe_device *vpbe_dev);

	
	void (*deinitialize)(struct device *dev, struct vpbe_device *vpbe_dev);

	
	int (*get_mode_info)(struct vpbe_device *vpbe_dev,
			     struct vpbe_enc_mode_info*);

	
	int (*set_mode)(struct vpbe_device *vpbe_dev,
			struct vpbe_enc_mode_info*);
	
	int (*suspend)(struct vpbe_device *vpbe_dev);
	int (*resume)(struct vpbe_device *vpbe_dev);
};


struct vpbe_device {
	
	struct v4l2_device v4l2_dev;
	
	struct vpbe_config *cfg;
	
	struct device *pdev;
	
	struct v4l2_subdev **encoders;
	
	int current_sd_index;
	
	struct v4l2_subdev *amp;
	struct mutex lock;
	
	int initialized;
	
	struct clk *dac_clk;
	
	struct osd_state *osd_device;
	
	struct venc_platform_data *venc_device;
	

	
	int current_out_index;
	
	
	struct vpbe_enc_mode_info current_timings;
	
	struct v4l2_subdev *venc;
	
	struct vpbe_device_ops ops;
};

#endif
