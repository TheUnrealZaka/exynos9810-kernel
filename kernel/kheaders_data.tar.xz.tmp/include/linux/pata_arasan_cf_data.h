

#ifndef _PATA_ARASAN_CF_DATA_H
#define _PATA_ARASAN_CF_DATA_H

#include <linux/platform_device.h>

struct arasan_cf_pdata {
	u8 cf_if_clk;
	#define CF_IF_CLK_100M			(0x0)
	#define CF_IF_CLK_75M			(0x1)
	#define CF_IF_CLK_66M			(0x2)
	#define CF_IF_CLK_50M			(0x3)
	#define CF_IF_CLK_40M			(0x4)
	#define CF_IF_CLK_33M			(0x5)
	#define CF_IF_CLK_25M			(0x6)
	#define CF_IF_CLK_125M			(0x7)
	#define CF_IF_CLK_150M			(0x8)
	#define CF_IF_CLK_166M			(0x9)
	#define CF_IF_CLK_200M			(0xA)
	
	u32 quirk;
	#define CF_BROKEN_PIO			(1)
	#define CF_BROKEN_MWDMA			(1 << 1)
	#define CF_BROKEN_UDMA			(1 << 2)
};

static inline void
set_arasan_cf_pdata(struct platform_device *pdev, struct arasan_cf_pdata *data)
{
	pdev->dev.platform_data = data;
}
#endif 
