

#ifndef __SPI_S3C64XX_H
#define __SPI_S3C64XX_H

#include <linux/dmaengine.h>


enum {
	DMA_MODE = 0,
	CPU_MODE = 1,
};


enum {
	NO_SWAP_MODE = 0,
	SWAP_MODE = 1,
};


enum {
	NONSECURE_MODE = 0,
	SECURE_MODE = 1,
};

enum {
	MANUAL_CS_MODE = 0,
	AUTO_CS_MODE = 1,
};


enum spi_domain {
	DOMAIN_TOP = 0,
	DOMAIN_ISP,
	DOMAIN_CAM1,
};

struct platform_device;


struct s3c64xx_spi_csinfo {
	u8 fb_delay;
	u8 cs_mode;
	unsigned line;
};


struct s3c64xx_spi_info {
	struct list_head node;
	unsigned int need_hw_init;
	int src_clk_nr;
	int num_cs;
	int dma_mode;
	int swap_mode;
	int secure_mode;
	int (*cfg_gpio)(void);
	dma_filter_fn filter;
	enum spi_domain domain;
	unsigned int dbg_mode;
};

struct s3c64xx_spi_dma_data {
	struct dma_chan *ch;
	enum dma_transfer_direction direction;
	unsigned long dmach;
};


struct s3c64xx_spi_driver_data {
	void __iomem                    *regs;
	struct clk                      *clk;
	struct clk                      *src_clk;
	struct platform_device          *pdev;
	struct spi_master               *master;
	struct s3c64xx_spi_info  *cntrlr_info;
	struct spi_device               *tgl_spi;
	struct list_head                queue;
	spinlock_t                      lock;
	unsigned long                   sfr_start;
	struct completion               xfer_completion;
	unsigned                        state;
	unsigned                        cur_mode, cur_bpw;
	unsigned                        cur_speed;
	struct s3c64xx_spi_dma_data	rx_dma;
	struct s3c64xx_spi_dma_data	tx_dma;
	struct samsung_dma_ops		*ops;

	struct s3c64xx_spi_port_config	*port_conf;
	unsigned int			port_id;
	unsigned long			gpios[4];

	struct pinctrl			*pinctrl;
	struct pinctrl_state		*pin_def;
	struct pinctrl_state		*pin_idle;

	int is_probed;
	int spi_clkoff_time;
	int idle_ip_index;
};


extern void s3c64xx_spi0_set_platdata(struct s3c64xx_spi_info *pd,
				      int src_clk_nr, int num_cs);
extern void s3c64xx_spi1_set_platdata(struct s3c64xx_spi_info *pd,
				      int src_clk_nr, int num_cs);
extern void s3c64xx_spi2_set_platdata(struct s3c64xx_spi_info *pd,
				      int src_clk_nr, int num_cs);


extern int s3c64xx_spi0_cfg_gpio(void);
extern int s3c64xx_spi1_cfg_gpio(void);
extern int s3c64xx_spi2_cfg_gpio(void);

extern struct s3c64xx_spi_info s3c64xx_spi0_pdata;
extern struct s3c64xx_spi_info s3c64xx_spi1_pdata;
extern struct s3c64xx_spi_info s3c64xx_spi2_pdata;
#endif 
