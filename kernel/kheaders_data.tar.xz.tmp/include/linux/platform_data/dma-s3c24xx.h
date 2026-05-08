


#define S3C24XX_DMA_CHANREQ(src, chan)	((BIT(3) | src) << chan * 4)

enum s3c24xx_dma_bus {
	S3C24XX_DMA_APB,
	S3C24XX_DMA_AHB,
};


struct s3c24xx_dma_channel {
	enum s3c24xx_dma_bus bus;
	bool handshake;
	u16 chansel;
};

struct dma_slave_map;


struct s3c24xx_dma_platdata {
	int num_phy_channels;
	struct s3c24xx_dma_channel *channels;
	int num_channels;
	const struct dma_slave_map *slave_map;
	int slavecnt;
};

struct dma_chan;
bool s3c24xx_dma_filter(struct dma_chan *chan, void *param);
