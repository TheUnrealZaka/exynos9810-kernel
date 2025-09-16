

#ifndef __LINUX_PCA963X_H
#define __LINUX_PCA963X_H
#include <linux/leds.h>

enum pca963x_outdrv {
	PCA963X_OPEN_DRAIN,
	PCA963X_TOTEM_POLE, 
};

enum pca963x_blink_type {
	PCA963X_SW_BLINK,
	PCA963X_HW_BLINK,
};

struct pca963x_platform_data {
	struct led_platform_data leds;
	enum pca963x_outdrv outdrv;
	enum pca963x_blink_type blink_type;
};

#endif 
