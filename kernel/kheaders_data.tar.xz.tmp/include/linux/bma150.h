

#ifndef _BMA150_H_
#define _BMA150_H_

#define BMA150_DRIVER		"bma150"

#define BMA150_RANGE_2G		0
#define BMA150_RANGE_4G		1
#define BMA150_RANGE_8G		2

#define BMA150_BW_25HZ		0
#define BMA150_BW_50HZ		1
#define BMA150_BW_100HZ		2
#define BMA150_BW_190HZ		3
#define BMA150_BW_375HZ		4
#define BMA150_BW_750HZ		5
#define BMA150_BW_1500HZ	6

struct bma150_cfg {
	bool any_motion_int;		
	bool hg_int;			
	bool lg_int;			
	unsigned char any_motion_dur;	
	unsigned char any_motion_thres;	
	unsigned char hg_hyst;		
	unsigned char hg_dur;		
	unsigned char hg_thres;		
	unsigned char lg_hyst;		
	unsigned char lg_dur;		
	unsigned char lg_thres;		
	unsigned char range;		
	unsigned char bandwidth;	
};

struct bma150_platform_data {
	struct bma150_cfg cfg;
	int (*irq_gpio_cfg)(void);
};

#endif 
