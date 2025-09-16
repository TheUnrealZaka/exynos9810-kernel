
#ifndef __MACB_PDATA_H__
#define __MACB_PDATA_H__


struct macb_platform_data {
	u32		phy_mask;
	int		phy_irq_pin;
	u8		is_rmii;
	u8		rev_eth_addr;
};

#endif 
