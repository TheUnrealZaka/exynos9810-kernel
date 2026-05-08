

#ifndef __ASM_ARCH_IMX_ESDHC_H
#define __ASM_ARCH_IMX_ESDHC_H

#include <linux/types.h>

enum wp_types {
	ESDHC_WP_NONE,		
	ESDHC_WP_CONTROLLER,	
	ESDHC_WP_GPIO,		
};

enum cd_types {
	ESDHC_CD_NONE,		
	ESDHC_CD_CONTROLLER,	
	ESDHC_CD_GPIO,		
	ESDHC_CD_PERMANENT,	
};



struct esdhc_platform_data {
	unsigned int wp_gpio;
	unsigned int cd_gpio;
	enum wp_types wp_type;
	enum cd_types cd_type;
	int max_bus_width;
	bool support_vsel;
	unsigned int delay_line;
	unsigned int tuning_step;       
	unsigned int tuning_start_tap;	
};
#endif 
