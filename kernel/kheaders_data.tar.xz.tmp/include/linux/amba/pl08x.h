

#ifndef AMBA_PL08X_H
#define AMBA_PL08X_H


#include <linux/dmaengine.h>
#include <linux/interrupt.h>

struct pl08x_driver_data;
struct pl08x_phy_chan;
struct pl08x_txd;


enum {
	PL08X_AHB1 = (1 << 0),
	PL08X_AHB2 = (1 << 1)
};


struct pl08x_channel_data {
	const char *bus_id;
	int min_signal;
	int max_signal;
	u32 muxval;
	u32 cctl_memcpy;
	dma_addr_t addr;
	bool single;
	u8 periph_buses;
};


struct pl08x_platform_data {
	struct pl08x_channel_data *slave_channels;
	unsigned int num_slave_channels;
	struct pl08x_channel_data memcpy_channel;
	int (*get_xfer_signal)(const struct pl08x_channel_data *);
	void (*put_xfer_signal)(const struct pl08x_channel_data *, int);
	u8 lli_buses;
	u8 mem_buses;
};

#ifdef CONFIG_AMBA_PL08X
bool pl08x_filter_id(struct dma_chan *chan, void *chan_id);
#else
static inline bool pl08x_filter_id(struct dma_chan *chan, void *chan_id)
{
	return false;
}
#endif

#endif	
