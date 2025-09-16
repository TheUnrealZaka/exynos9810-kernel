

#ifndef __DAVINCI_ASP_H
#define __DAVINCI_ASP_H

#include <linux/genalloc.h>

struct davinci_mcasp_pdata {
	u32 tx_dma_offset;
	u32 rx_dma_offset;
	int asp_chan_q;	
	int ram_chan_q;	
	
	unsigned enable_channel_combine:1;
	unsigned sram_size_playback;
	unsigned sram_size_capture;
	struct gen_pool *sram_pool;

	
	int clk_input_pin;

	
	bool i2s_accurate_sck;

	
	int tdm_slots;
	u8 op_mode;
	u8 num_serializer;
	u8 *serial_dir;
	u8 version;
	u8 txnumevt;
	u8 rxnumevt;
	int tx_dma_channel;
	int rx_dma_channel;
};

#define snd_platform_data davinci_mcasp_pdata

enum {
	MCASP_VERSION_1 = 0,	
	MCASP_VERSION_2,	
	MCASP_VERSION_3,        
	MCASP_VERSION_4,	
};

enum mcbsp_clk_input_pin {
	MCBSP_CLKR = 0,		
	MCBSP_CLKS,
};

#define INACTIVE_MODE	0
#define TX_MODE		1
#define RX_MODE		2

#define DAVINCI_MCASP_IIS_MODE	0
#define DAVINCI_MCASP_DIT_MODE	1

#endif
