

#ifndef SI4713_H
#define SI4713_H


#define SI4713_I2C_ADDR_BUSEN_HIGH	0x63
#define SI4713_I2C_ADDR_BUSEN_LOW	0x11


struct si4713_platform_data {
	bool is_platform_device;
};


struct si4713_rnl {
	__u32 index;		
	__u32 frequency;	
	__s32 rnl;		
	__u32 reserved[4];	
};


#define SI4713_IOC_MEASURE_RNL	_IOWR('V', BASE_VIDIOC_PRIVATE + 0, \
						struct si4713_rnl)

#endif 
