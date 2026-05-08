

#ifndef _V4L2_FLASH_H
#define _V4L2_FLASH_H

#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>

struct led_classdev_flash;
struct led_classdev;
struct v4l2_flash;
enum led_brightness;


struct v4l2_flash_ctrl_data {
	struct v4l2_ctrl_config config;
	u32 cid;
};


struct v4l2_flash_ops {
	int (*external_strobe_set)(struct v4l2_flash *v4l2_flash,
					bool enable);
	enum led_brightness (*intensity_to_led_brightness)
		(struct v4l2_flash *v4l2_flash, s32 intensity);
	s32 (*led_brightness_to_intensity)
		(struct v4l2_flash *v4l2_flash, enum led_brightness);
};


struct v4l2_flash_config {
	char dev_name[32];
	struct led_flash_setting torch_intensity;
	struct led_flash_setting indicator_intensity;
	u32 flash_faults;
	unsigned int has_external_strobe:1;
};


struct v4l2_flash {
	struct led_classdev_flash *fled_cdev;
	struct led_classdev_flash *iled_cdev;
	const struct v4l2_flash_ops *ops;

	struct v4l2_subdev sd;
	struct v4l2_ctrl_handler hdl;
	struct v4l2_ctrl **ctrls;
};

static inline struct v4l2_flash *v4l2_subdev_to_v4l2_flash(
							struct v4l2_subdev *sd)
{
	return container_of(sd, struct v4l2_flash, sd);
}

static inline struct v4l2_flash *v4l2_ctrl_to_v4l2_flash(struct v4l2_ctrl *c)
{
	return container_of(c->handler, struct v4l2_flash, hdl);
}

#if IS_ENABLED(CONFIG_V4L2_FLASH_LED_CLASS)

struct v4l2_flash *v4l2_flash_init(
	struct device *dev, struct device_node *of_node,
	struct led_classdev_flash *fled_cdev,
	struct led_classdev_flash *iled_cdev,
	const struct v4l2_flash_ops *ops,
	struct v4l2_flash_config *config);


void v4l2_flash_release(struct v4l2_flash *v4l2_flash);

#else
static inline struct v4l2_flash *v4l2_flash_init(
	struct device *dev, struct device_node *of_node,
	struct led_classdev_flash *fled_cdev,
	struct led_classdev_flash *iled_cdev,
	const struct v4l2_flash_ops *ops,
	struct v4l2_flash_config *config)
{
	return NULL;
}

static inline void v4l2_flash_release(struct v4l2_flash *v4l2_flash)
{
}
#endif 

#endif 
