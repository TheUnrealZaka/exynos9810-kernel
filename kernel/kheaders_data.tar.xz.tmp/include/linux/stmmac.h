

#ifndef __STMMAC_PLATFORM_DATA
#define __STMMAC_PLATFORM_DATA

#include <linux/platform_device.h>

#define STMMAC_RX_COE_NONE	0
#define STMMAC_RX_COE_TYPE1	1
#define STMMAC_RX_COE_TYPE2	2




#define	STMMAC_CSR_60_100M	0x0	
#define	STMMAC_CSR_100_150M	0x1	
#define	STMMAC_CSR_20_35M	0x2	
#define	STMMAC_CSR_35_60M	0x3	
#define	STMMAC_CSR_150_250M	0x4	
#define	STMMAC_CSR_250_300M	0x5	


#define STMMAC_CSR_I_4		0x8	
#define STMMAC_CSR_I_6		0x9	
#define STMMAC_CSR_I_8		0xA	
#define STMMAC_CSR_I_10		0xB	
#define STMMAC_CSR_I_12		0xC	
#define STMMAC_CSR_I_14		0xD	
#define STMMAC_CSR_I_16		0xE	
#define STMMAC_CSR_I_18		0xF	


#define DMA_AXI_BLEN_4		(1 << 1)
#define DMA_AXI_BLEN_8		(1 << 2)
#define DMA_AXI_BLEN_16		(1 << 3)
#define DMA_AXI_BLEN_32		(1 << 4)
#define DMA_AXI_BLEN_64		(1 << 5)
#define DMA_AXI_BLEN_128	(1 << 6)
#define DMA_AXI_BLEN_256	(1 << 7)
#define DMA_AXI_BLEN_ALL (DMA_AXI_BLEN_4 | DMA_AXI_BLEN_8 | DMA_AXI_BLEN_16 \
			| DMA_AXI_BLEN_32 | DMA_AXI_BLEN_64 \
			| DMA_AXI_BLEN_128 | DMA_AXI_BLEN_256)



struct stmmac_mdio_bus_data {
	int (*phy_reset)(void *priv);
	unsigned int phy_mask;
	int *irqs;
	int probed_phy_irq;
#ifdef CONFIG_OF
	int reset_gpio, active_low;
	u32 delays[3];
#endif
};

struct stmmac_dma_cfg {
	int pbl;
	int fixed_burst;
	int mixed_burst;
	bool aal;
};

#define AXI_BLEN	7
struct stmmac_axi {
	bool axi_lpi_en;
	bool axi_xit_frm;
	u32 axi_wr_osr_lmt;
	u32 axi_rd_osr_lmt;
	bool axi_kbbe;
	bool axi_axi_all;
	u32 axi_blen[AXI_BLEN];
	bool axi_fb;
	bool axi_mb;
	bool axi_rb;
};

struct plat_stmmacenet_data {
	int bus_id;
	int phy_addr;
	int interface;
	struct stmmac_mdio_bus_data *mdio_bus_data;
	struct device_node *phy_node;
	struct device_node *mdio_node;
	struct stmmac_dma_cfg *dma_cfg;
	int clk_csr;
	int has_gmac;
	int enh_desc;
	int tx_coe;
	int rx_coe;
	int bugged_jumbo;
	int pmt;
	int force_sf_dma_mode;
	int force_thresh_dma_mode;
	int riwt_off;
	int max_speed;
	int maxmtu;
	int multicast_filter_bins;
	int unicast_filter_entries;
	int tx_fifo_size;
	int rx_fifo_size;
	void (*fix_mac_speed)(void *priv, unsigned int speed);
	void (*bus_setup)(void __iomem *ioaddr);
	int (*init)(struct platform_device *pdev, void *priv);
	void (*exit)(struct platform_device *pdev, void *priv);
	void (*suspend)(struct platform_device *pdev, void *priv);
	void (*resume)(struct platform_device *pdev, void *priv);
	void *bsp_priv;
	struct stmmac_axi *axi;
	int has_gmac4;
	bool tso_en;
	int mac_port_sel_speed;
};
#endif
