

#ifndef __AS3645A_H__
#define __AS3645A_H__

#include <media/v4l2-subdev.h>

#define AS3645A_NAME				"as3645a"
#define AS3645A_I2C_ADDR			(0x60 >> 1) 

#define AS3645A_FLASH_TIMEOUT_MIN		100000	
#define AS3645A_FLASH_TIMEOUT_MAX		850000
#define AS3645A_FLASH_TIMEOUT_STEP		50000

#define AS3645A_FLASH_INTENSITY_MIN		200	
#define AS3645A_FLASH_INTENSITY_MAX_1LED	500
#define AS3645A_FLASH_INTENSITY_MAX_2LEDS	400
#define AS3645A_FLASH_INTENSITY_STEP		20

#define AS3645A_TORCH_INTENSITY_MIN		20	
#define AS3645A_TORCH_INTENSITY_MAX		160
#define AS3645A_TORCH_INTENSITY_STEP		20

#define AS3645A_INDICATOR_INTENSITY_MIN		0	
#define AS3645A_INDICATOR_INTENSITY_MAX		10000
#define AS3645A_INDICATOR_INTENSITY_STEP	2500


struct as3645a_platform_data {
	int (*set_power)(struct v4l2_subdev *subdev, int on);
	unsigned int vref;
	unsigned int peak;
	bool ext_strobe;

	
	unsigned int flash_max_current;
	unsigned int torch_max_current;
	unsigned int timeout_max;
};

#endif 
