

#ifndef _LINUX_SPI_TSC2005_H
#define _LINUX_SPI_TSC2005_H

#include <linux/types.h>

struct tsc2005_platform_data {
	int		ts_pressure_max;
	int		ts_pressure_fudge;
	int		ts_x_max;
	int		ts_x_fudge;
	int		ts_y_max;
	int		ts_y_fudge;
	int		ts_x_plate_ohm;
	unsigned int	esd_timeout_ms;
	void		(*set_reset)(bool enable);
};

#endif
