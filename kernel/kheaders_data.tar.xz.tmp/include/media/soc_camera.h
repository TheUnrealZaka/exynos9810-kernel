

#ifndef SOC_CAMERA_H
#define SOC_CAMERA_H

#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/pm.h>
#include <linux/videodev2.h>
#include <media/videobuf-core.h>
#include <media/videobuf2-v4l2.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>

struct file;
struct soc_camera_desc;
struct soc_camera_async_client;

struct soc_camera_device {
	struct list_head list;		
	struct soc_camera_desc *sdesc;
	struct device *pdev;		
	struct device *parent;		
	struct device *control;		
	s32 user_width;
	s32 user_height;
	u32 bytesperline;		
	u32 sizeimage;
	enum v4l2_colorspace colorspace;
	unsigned char iface;		
	unsigned char devnum;		
	struct soc_camera_sense *sense;	
	struct video_device *vdev;
	struct v4l2_ctrl_handler ctrl_handler;
	const struct soc_camera_format_xlate *current_fmt;
	struct soc_camera_format_xlate *user_formats;
	int num_user_formats;
	enum v4l2_field field;		
	void *host_priv;		
	
	int use_count;
	struct file *streamer;		
	struct v4l2_clk *clk;
	
	struct soc_camera_async_client *sasc;
	
	union {
		struct videobuf_queue vb_vidq;
		struct vb2_queue vb2_vidq;
	};
};


#define SOCAM_HOST_CAP_STRIDE		(1 << 0)

enum soc_camera_subdev_role {
	SOCAM_SUBDEV_DATA_SOURCE = 1,
	SOCAM_SUBDEV_DATA_SINK,
	SOCAM_SUBDEV_DATA_PROCESSOR,
};

struct soc_camera_async_subdev {
	struct v4l2_async_subdev asd;
	enum soc_camera_subdev_role role;
};

struct soc_camera_host {
	struct v4l2_device v4l2_dev;
	struct list_head list;
	struct mutex host_lock;		
	struct mutex clk_lock;		
	unsigned char nr;		
	u32 capabilities;
	struct soc_camera_device *icd;	
	void *priv;
	const char *drv_name;
	struct soc_camera_host_ops *ops;
	struct v4l2_async_subdev **asd;	
	unsigned int *asd_sizes;	
};

struct soc_camera_host_ops {
	struct module *owner;
	int (*add)(struct soc_camera_device *);
	void (*remove)(struct soc_camera_device *);
	int (*clock_start)(struct soc_camera_host *);
	void (*clock_stop)(struct soc_camera_host *);
	
	int (*get_formats)(struct soc_camera_device *, unsigned int,
			   struct soc_camera_format_xlate *);
	void (*put_formats)(struct soc_camera_device *);
	int (*get_selection)(struct soc_camera_device *, struct v4l2_selection *);
	int (*set_selection)(struct soc_camera_device *, struct v4l2_selection *);
	
	int (*set_liveselection)(struct soc_camera_device *, struct v4l2_selection *);
	int (*set_fmt)(struct soc_camera_device *, struct v4l2_format *);
	int (*try_fmt)(struct soc_camera_device *, struct v4l2_format *);
	void (*init_videobuf)(struct videobuf_queue *,
			      struct soc_camera_device *);
	int (*init_videobuf2)(struct vb2_queue *,
			      struct soc_camera_device *);
	int (*reqbufs)(struct soc_camera_device *, struct v4l2_requestbuffers *);
	int (*querycap)(struct soc_camera_host *, struct v4l2_capability *);
	int (*set_bus_param)(struct soc_camera_device *);
	int (*get_parm)(struct soc_camera_device *, struct v4l2_streamparm *);
	int (*set_parm)(struct soc_camera_device *, struct v4l2_streamparm *);
	int (*enum_framesizes)(struct soc_camera_device *, struct v4l2_frmsizeenum *);
	unsigned int (*poll)(struct file *, poll_table *);
};

#define SOCAM_SENSOR_INVERT_PCLK	(1 << 0)
#define SOCAM_SENSOR_INVERT_MCLK	(1 << 1)
#define SOCAM_SENSOR_INVERT_HSYNC	(1 << 2)
#define SOCAM_SENSOR_INVERT_VSYNC	(1 << 3)
#define SOCAM_SENSOR_INVERT_DATA	(1 << 4)

struct i2c_board_info;
struct regulator_bulk_data;

struct soc_camera_subdev_desc {
	
	unsigned long flags;

	
	void *drv_priv;

	
	bool unbalanced_power;
	unsigned long clock_state;

	
	int (*power)(struct device *, int);
	int (*reset)(struct device *);

	
	int (*set_bus_param)(struct soc_camera_subdev_desc *, unsigned long flags);
	unsigned long (*query_bus_param)(struct soc_camera_subdev_desc *);
	void (*free_bus)(struct soc_camera_subdev_desc *);

	
	struct v4l2_subdev_platform_data sd_pdata;
};

struct soc_camera_host_desc {
	
	int bus_id;
	int i2c_adapter_id;
	struct i2c_board_info *board_info;
	const char *module_name;

	
	int (*add_device)(struct soc_camera_device *);
	void (*del_device)(struct soc_camera_device *);
};


struct soc_camera_desc {
	struct soc_camera_subdev_desc subdev_desc;
	struct soc_camera_host_desc host_desc;
};


struct soc_camera_link {
	

	
	unsigned long flags;

	void *priv;

	
	bool unbalanced_power;
	
	unsigned long clock_state;

	
	int (*power)(struct device *, int);
	int (*reset)(struct device *);
	
	int (*set_bus_param)(struct soc_camera_link *, unsigned long flags);
	unsigned long (*query_bus_param)(struct soc_camera_link *);
	void (*free_bus)(struct soc_camera_link *);

	
	struct regulator_bulk_data *regulators;
	int num_regulators;

	void *host_priv;

	

	
	int bus_id;
	int i2c_adapter_id;
	struct i2c_board_info *board_info;
	const char *module_name;

	
	int (*add_device)(struct soc_camera_device *);
	void (*del_device)(struct soc_camera_device *);
};

static inline struct soc_camera_host *to_soc_camera_host(
	const struct device *dev)
{
	struct v4l2_device *v4l2_dev = dev_get_drvdata(dev);

	return container_of(v4l2_dev, struct soc_camera_host, v4l2_dev);
}

static inline struct soc_camera_desc *to_soc_camera_desc(
	const struct soc_camera_device *icd)
{
	return icd->sdesc;
}

static inline struct device *to_soc_camera_control(
	const struct soc_camera_device *icd)
{
	return icd->control;
}

static inline struct v4l2_subdev *soc_camera_to_subdev(
	const struct soc_camera_device *icd)
{
	struct device *control = to_soc_camera_control(icd);
	return dev_get_drvdata(control);
}

int soc_camera_host_register(struct soc_camera_host *ici);
void soc_camera_host_unregister(struct soc_camera_host *ici);

const struct soc_camera_format_xlate *soc_camera_xlate_by_fourcc(
	struct soc_camera_device *icd, unsigned int fourcc);


struct soc_camera_format_xlate {
	u32 code;
	const struct soc_mbus_pixelfmt *host_fmt;
};

#define SOCAM_SENSE_PCLK_CHANGED	(1 << 0)


struct soc_camera_sense {
	unsigned long flags;
	unsigned long master_clock;
	unsigned long pixel_clock_max;
	unsigned long pixel_clock;
};

#define SOCAM_DATAWIDTH(x)	BIT((x) - 1)
#define SOCAM_DATAWIDTH_4	SOCAM_DATAWIDTH(4)
#define SOCAM_DATAWIDTH_8	SOCAM_DATAWIDTH(8)
#define SOCAM_DATAWIDTH_9	SOCAM_DATAWIDTH(9)
#define SOCAM_DATAWIDTH_10	SOCAM_DATAWIDTH(10)
#define SOCAM_DATAWIDTH_12	SOCAM_DATAWIDTH(12)
#define SOCAM_DATAWIDTH_15	SOCAM_DATAWIDTH(15)
#define SOCAM_DATAWIDTH_16	SOCAM_DATAWIDTH(16)
#define SOCAM_DATAWIDTH_18	SOCAM_DATAWIDTH(18)
#define SOCAM_DATAWIDTH_24	SOCAM_DATAWIDTH(24)

#define SOCAM_DATAWIDTH_MASK (SOCAM_DATAWIDTH_4 | SOCAM_DATAWIDTH_8 | \
			      SOCAM_DATAWIDTH_9 | SOCAM_DATAWIDTH_10 | \
			      SOCAM_DATAWIDTH_12 | SOCAM_DATAWIDTH_15 | \
			      SOCAM_DATAWIDTH_16 | SOCAM_DATAWIDTH_18 | \
			      SOCAM_DATAWIDTH_24)

static inline void soc_camera_limit_side(int *start, int *length,
		unsigned int start_min,
		unsigned int length_min, unsigned int length_max)
{
	if (*length < length_min)
		*length = length_min;
	else if (*length > length_max)
		*length = length_max;

	if (*start < start_min)
		*start = start_min;
	else if (*start > start_min + length_max - *length)
		*start = start_min + length_max - *length;
}

unsigned long soc_camera_apply_board_flags(struct soc_camera_subdev_desc *ssdd,
					   const struct v4l2_mbus_config *cfg);

int soc_camera_power_init(struct device *dev, struct soc_camera_subdev_desc *ssdd);
int soc_camera_power_on(struct device *dev, struct soc_camera_subdev_desc *ssdd,
			struct v4l2_clk *clk);
int soc_camera_power_off(struct device *dev, struct soc_camera_subdev_desc *ssdd,
			 struct v4l2_clk *clk);

static inline int soc_camera_set_power(struct device *dev,
		struct soc_camera_subdev_desc *ssdd, struct v4l2_clk *clk, bool on)
{
	return on ? soc_camera_power_on(dev, ssdd, clk)
		  : soc_camera_power_off(dev, ssdd, clk);
}


#include <linux/i2c.h>
static inline struct video_device *soc_camera_i2c_to_vdev(const struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct soc_camera_device *icd = v4l2_get_subdev_hostdata(sd);
	return icd ? icd->vdev : NULL;
}

static inline struct soc_camera_subdev_desc *soc_camera_i2c_to_desc(const struct i2c_client *client)
{
	return client->dev.platform_data;
}

static inline struct v4l2_subdev *soc_camera_vdev_to_subdev(struct video_device *vdev)
{
	struct soc_camera_device *icd = video_get_drvdata(vdev);
	return soc_camera_to_subdev(icd);
}

static inline struct soc_camera_device *soc_camera_from_vb2q(const struct vb2_queue *vq)
{
	return container_of(vq, struct soc_camera_device, vb2_vidq);
}

static inline struct soc_camera_device *soc_camera_from_vbq(const struct videobuf_queue *vq)
{
	return container_of(vq, struct soc_camera_device, vb_vidq);
}

static inline u32 soc_camera_grp_id(const struct soc_camera_device *icd)
{
	return (icd->iface << 8) | (icd->devnum + 1);
}

void soc_camera_lock(struct vb2_queue *vq);
void soc_camera_unlock(struct vb2_queue *vq);

#endif
