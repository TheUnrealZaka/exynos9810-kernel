


#define S3C64XX_AC97_GPD  0
#define S3C64XX_AC97_GPE  1

#include <linux/dmaengine.h>

extern void s3c64xx_ac97_setup_gpio(int);

struct samsung_i2s {

#define QUIRK_PRI_6CHAN		(1 << 0)

#define QUIRK_SEC_DAI		(1 << 1)

#define QUIRK_NO_MUXPSR		(1 << 2)
#define QUIRK_NEED_RSTCLR	(1 << 3)
#define QUIRK_SUPPORTS_TDM	(1 << 4)
#define QUIRK_SUPPORTS_IDMA	(1 << 5)
	
	u32 quirks;
	dma_addr_t idma_addr;
};


struct s3c_audio_pdata {
	int (*cfg_gpio)(struct platform_device *);
	dma_filter_fn dma_filter;
	void *dma_playback;
	void *dma_capture;
	void *dma_play_sec;
	void *dma_capture_mic;
	union {
		struct samsung_i2s i2s;
	} type;
};
