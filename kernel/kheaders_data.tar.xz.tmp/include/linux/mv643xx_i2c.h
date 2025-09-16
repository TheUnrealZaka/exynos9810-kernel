

#ifndef _MV64XXX_I2C_H_
#define _MV64XXX_I2C_H_

#include <linux/types.h>

#define MV64XXX_I2C_CTLR_NAME	"mv64xxx_i2c"


struct mv64xxx_i2c_pdata {
	u32	freq_m;
	u32	freq_n;
	u32	timeout;	
};

#endif 
