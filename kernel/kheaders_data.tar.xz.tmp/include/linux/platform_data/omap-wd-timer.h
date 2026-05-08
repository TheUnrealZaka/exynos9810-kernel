

#ifndef __LINUX_PLATFORM_DATA_OMAP_WD_TIMER_H
#define __LINUX_PLATFORM_DATA_OMAP_WD_TIMER_H

#include <linux/types.h>


#define OMAP_MPU_WD_RST_SRC_ID_SHIFT				3


struct omap_wd_timer_platform_data {
	u32 (*read_reset_sources)(void);
};

#endif
