

#ifndef __I2C_PNX_H__
#define __I2C_PNX_H__

struct platform_device;
struct clk;

struct i2c_pnx_mif {
	int			ret;		
	int			mode;		
	struct completion	complete;	
	struct timer_list	timer;		
	u8 *			buf;		
	int			len;		
	int			order;		
};

struct i2c_pnx_algo_data {
	void __iomem		*ioaddr;
	struct i2c_pnx_mif	mif;
	int			last;
	struct clk		*clk;
	struct i2c_adapter	adapter;
	int			irq;
	u32			timeout;
};

#endif 
