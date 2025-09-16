

#ifndef __LEDS_S3C24XX_H
#define __LEDS_S3C24XX_H

#define S3C24XX_LEDF_ACTLOW	(1<<0)		
#define S3C24XX_LEDF_TRISTATE	(1<<1)		

struct s3c24xx_led_platdata {
	unsigned int		 gpio;
	unsigned int		 flags;

	char			*name;
	char			*def_trigger;
};

#endif 
