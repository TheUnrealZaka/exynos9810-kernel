
#ifndef __LINUX_FLASH_LEDS_H_INCLUDED
#define __LINUX_FLASH_LEDS_H_INCLUDED

#include <linux/leds.h>

struct device_node;
struct led_classdev_flash;


#define LED_FAULT_OVER_VOLTAGE		(1 << 0)
#define LED_FAULT_TIMEOUT		(1 << 1)
#define LED_FAULT_OVER_TEMPERATURE	(1 << 2)
#define LED_FAULT_SHORT_CIRCUIT		(1 << 3)
#define LED_FAULT_OVER_CURRENT		(1 << 4)
#define LED_FAULT_INDICATOR		(1 << 5)
#define LED_FAULT_UNDER_VOLTAGE		(1 << 6)
#define LED_FAULT_INPUT_VOLTAGE		(1 << 7)
#define LED_FAULT_LED_OVER_TEMPERATURE	(1 << 8)
#define LED_NUM_FLASH_FAULTS		9

#define LED_FLASH_SYSFS_GROUPS_SIZE	5

struct led_flash_ops {
	
	int (*flash_brightness_set)(struct led_classdev_flash *fled_cdev,
					u32 brightness);
	
	int (*flash_brightness_get)(struct led_classdev_flash *fled_cdev,
					u32 *brightness);
	
	int (*strobe_set)(struct led_classdev_flash *fled_cdev, bool state);
	
	int (*strobe_get)(struct led_classdev_flash *fled_cdev, bool *state);
	
	int (*timeout_set)(struct led_classdev_flash *fled_cdev, u32 timeout);
	
	int (*fault_get)(struct led_classdev_flash *fled_cdev, u32 *fault);
};


struct led_flash_setting {
	
	u32 min;
	
	u32 max;
	
	u32 step;
	
	u32 val;
};

struct led_classdev_flash {
	
	struct led_classdev led_cdev;

	
	const struct led_flash_ops *ops;

	
	struct led_flash_setting brightness;

	
	struct led_flash_setting timeout;

	
	const struct attribute_group *sysfs_groups[LED_FLASH_SYSFS_GROUPS_SIZE];
};

static inline struct led_classdev_flash *lcdev_to_flcdev(
						struct led_classdev *lcdev)
{
	return container_of(lcdev, struct led_classdev_flash, led_cdev);
}


extern int led_classdev_flash_register(struct device *parent,
				struct led_classdev_flash *fled_cdev);


extern void led_classdev_flash_unregister(struct led_classdev_flash *fled_cdev);


static inline int led_set_flash_strobe(struct led_classdev_flash *fled_cdev,
					bool state)
{
	return fled_cdev->ops->strobe_set(fled_cdev, state);
}


static inline int led_get_flash_strobe(struct led_classdev_flash *fled_cdev,
					bool *state)
{
	if (fled_cdev->ops->strobe_get)
		return fled_cdev->ops->strobe_get(fled_cdev, state);

	return -EINVAL;
}


extern int led_set_flash_brightness(struct led_classdev_flash *fled_cdev,
					u32 brightness);


extern int led_update_flash_brightness(struct led_classdev_flash *fled_cdev);


extern int led_set_flash_timeout(struct led_classdev_flash *fled_cdev,
					u32 timeout);


extern int led_get_flash_fault(struct led_classdev_flash *fled_cdev,
					u32 *fault);

#endif	
