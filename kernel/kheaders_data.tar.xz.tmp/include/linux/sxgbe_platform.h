
#ifndef __SXGBE_PLATFORM_H__
#define __SXGBE_PLATFORM_H__


#define SXGBE_CSR_100_150M	0x0	
#define SXGBE_CSR_150_250M	0x1	
#define SXGBE_CSR_250_300M	0x2	
#define SXGBE_CSR_300_350M	0x3	
#define SXGBE_CSR_350_400M	0x4	
#define SXGBE_CSR_400_500M	0x5	


struct sxgbe_mdio_bus_data {
	unsigned int phy_mask;
	int *irqs;
	int probed_phy_irq;
};

struct sxgbe_dma_cfg {
	int pbl;
	int fixed_burst;
	int burst_map;
	int adv_addr_mode;
};

struct sxgbe_plat_data {
	char *phy_bus_name;
	int bus_id;
	int phy_addr;
	int interface;
	struct sxgbe_mdio_bus_data *mdio_bus_data;
	struct sxgbe_dma_cfg *dma_cfg;
	int clk_csr;
	int pmt;
	int force_sf_dma_mode;
	int force_thresh_dma_mode;
	int riwt_off;
};

#endif 
