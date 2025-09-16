
#ifndef __ASOC_TI_MCBSP_H
#define __ASOC_TI_MCBSP_H

#include <linux/spinlock.h>
#include <linux/clk.h>

#define MCBSP_CONFIG_TYPE2	0x2
#define MCBSP_CONFIG_TYPE3	0x3
#define MCBSP_CONFIG_TYPE4	0x4


struct omap_mcbsp_ops {
	void (*request)(unsigned int);
	void (*free)(unsigned int);
};

struct omap_mcbsp_platform_data {
	struct omap_mcbsp_ops *ops;
	u16 buffer_size;
	u8 reg_size;
	u8 reg_step;

	
	bool has_wakeup; 
	bool has_ccr; 
	int (*force_ick_on)(struct clk *clk, bool force_on);
};


struct omap_mcbsp_dev_attr {
	const char *sidetone;
};

void omap3_mcbsp_init_pdata_callback(struct omap_mcbsp_platform_data *pdata);

#endif
