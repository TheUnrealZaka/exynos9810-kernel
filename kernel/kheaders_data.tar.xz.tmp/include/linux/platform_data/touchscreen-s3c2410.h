

#ifndef __TOUCHSCREEN_S3C2410_H
#define __TOUCHSCREEN_S3C2410_H

struct s3c2410_ts_mach_info {
	int delay;
	int presc;
	int oversampling_shift;
	void (*cfg_gpio)(struct platform_device *dev);
};

extern void s3c24xx_ts_set_platdata(struct s3c2410_ts_mach_info *);
extern void s3c64xx_ts_set_platdata(struct s3c2410_ts_mach_info *);


extern void s3c24xx_ts_cfg_gpio(struct platform_device *dev);

#endif 
