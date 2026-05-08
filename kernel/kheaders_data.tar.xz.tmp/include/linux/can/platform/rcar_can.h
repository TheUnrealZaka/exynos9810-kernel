#ifndef _CAN_PLATFORM_RCAR_CAN_H_
#define _CAN_PLATFORM_RCAR_CAN_H_

#include <linux/types.h>


enum CLKR {
	CLKR_CLKP1 = 0,	
	CLKR_CLKP2 = 1,	
	CLKR_CLKEXT = 3	
};

struct rcar_can_platform_data {
	enum CLKR clock_select;	
};

#endif	
