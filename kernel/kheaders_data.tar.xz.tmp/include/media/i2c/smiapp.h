

#ifndef __SMIAPP_H_
#define __SMIAPP_H_

#include <media/v4l2-subdev.h>

#define SMIAPP_NAME		"smiapp"

#define SMIAPP_DFL_I2C_ADDR	(0x20 >> 1) 
#define SMIAPP_ALT_I2C_ADDR	(0x6e >> 1) 

#define SMIAPP_CSI_SIGNALLING_MODE_CCP2_DATA_CLOCK	0
#define SMIAPP_CSI_SIGNALLING_MODE_CCP2_DATA_STROBE	1
#define SMIAPP_CSI_SIGNALLING_MODE_CSI2			2


enum smiapp_module_board_orient {
	SMIAPP_MODULE_BOARD_ORIENT_0 = 0,
	SMIAPP_MODULE_BOARD_ORIENT_180,
};

struct smiapp_flash_strobe_parms {
	u8 mode;
	u32 strobe_width_high_us;
	u16 strobe_delay;
	u16 stobe_start_point;
	u8 trigger;
};

struct smiapp_hwconfig {
	
	unsigned short i2c_addr_dfl;	
	unsigned short i2c_addr_alt;	

	uint32_t nvm_size;		
	uint32_t ext_clk;		

	unsigned int lanes;		
	uint32_t csi_signalling_mode;	
	uint64_t *op_sys_clock;

	enum smiapp_module_board_orient module_board_orient;

	struct smiapp_flash_strobe_parms *strobe_setup;
};

#endif 
