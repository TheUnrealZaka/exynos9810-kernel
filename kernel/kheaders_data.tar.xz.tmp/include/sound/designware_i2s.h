

#ifndef __SOUND_DESIGNWARE_I2S_H
#define __SOUND_DESIGNWARE_I2S_H

#include <linux/dmaengine.h>
#include <linux/types.h>


struct i2s_clk_config_data {
	int chan_nr;
	u32 data_width;
	u32 sample_rate;
};

struct i2s_platform_data {
	#define DWC_I2S_PLAY	(1 << 0)
	#define DWC_I2S_RECORD	(1 << 1)
	#define DW_I2S_SLAVE	(1 << 2)
	#define DW_I2S_MASTER	(1 << 3)
	unsigned int cap;
	int channel;
	u32 snd_fmts;
	u32 snd_rates;

	#define DW_I2S_QUIRK_COMP_REG_OFFSET	(1 << 0)
	#define DW_I2S_QUIRK_COMP_PARAM1	(1 << 1)
	unsigned int quirks;
	unsigned int i2s_reg_comp1;
	unsigned int i2s_reg_comp2;

	void *play_dma_data;
	void *capture_dma_data;
	bool (*filter)(struct dma_chan *chan, void *slave);
	int (*i2s_clk_cfg)(struct i2s_clk_config_data *config);
};

struct i2s_dma_data {
	void *data;
	dma_addr_t addr;
	u32 max_burst;
	enum dma_slave_buswidth addr_width;
	bool (*filter)(struct dma_chan *chan, void *slave);
};


#define I2S_RXDMA		0x01C0
#define I2S_TXDMA		0x01C8

#define TWO_CHANNEL_SUPPORT	2	
#define FOUR_CHANNEL_SUPPORT	4	
#define SIX_CHANNEL_SUPPORT	6	
#define EIGHT_CHANNEL_SUPPORT	8	

#endif 
