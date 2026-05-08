

#ifndef _LINUX_DRV260X_PDATA_H
#define _LINUX_DRV260X_PDATA_H

struct drv260x_platform_data {
	u32 library_selection;
	u32 mode;
	u32 vib_rated_voltage;
	u32 vib_overdrive_voltage;
};

#endif
