

#ifndef __KXTJ9_H__
#define __KXTJ9_H__

#define KXTJ9_I2C_ADDR		0x0F

struct kxtj9_platform_data {
	unsigned int min_interval;	
	unsigned int init_interval;	

	
	u8 axis_map_x;
	u8 axis_map_y;
	u8 axis_map_z;

	
	bool negate_x;
	bool negate_y;
	bool negate_z;

	
	
	#define RES_8BIT		0
	#define RES_12BIT		(1 << 6)
	u8 res_12bit;
	
	#define KXTJ9_G_2G		0
	#define KXTJ9_G_4G		(1 << 3)
	#define KXTJ9_G_8G		(1 << 4)
	u8 g_range;

	int (*init)(void);
	void (*exit)(void);
	int (*power_on)(void);
	int (*power_off)(void);
};
#endif  
