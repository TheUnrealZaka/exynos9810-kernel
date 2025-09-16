

#ifndef __LINUX_MV643XX_ETH_H
#define __LINUX_MV643XX_ETH_H

#include <linux/mbus.h>
#include <linux/if_ether.h>

#define MV643XX_ETH_SHARED_NAME		"mv643xx_eth"
#define MV643XX_ETH_NAME		"mv643xx_eth_port"
#define MV643XX_ETH_SHARED_REGS		0x2000
#define MV643XX_ETH_SHARED_REGS_SIZE	0x2000
#define MV643XX_ETH_BAR_4		0x2220
#define MV643XX_ETH_SIZE_REG_4		0x2224
#define MV643XX_ETH_BASE_ADDR_ENABLE_REG	0x2290

#define MV643XX_TX_CSUM_DEFAULT_LIMIT	0

struct mv643xx_eth_shared_platform_data {
	struct mbus_dram_target_info	*dram;
	
	int			tx_csum_limit;
};

#define MV643XX_ETH_PHY_ADDR_DEFAULT	0
#define MV643XX_ETH_PHY_ADDR(x)		(0x80 | (x))
#define MV643XX_ETH_PHY_NONE		0xff

struct device_node;
struct mv643xx_eth_platform_data {
	
	struct platform_device	*shared;
	int			port_number;

	
	int			phy_addr;
	struct device_node	*phy_node;

	
	u8			mac_addr[ETH_ALEN];

	
	int			speed;
	int			duplex;

	
	int			rx_queue_count;
	int			tx_queue_count;

	
	int			rx_queue_size;
	int			tx_queue_size;

	
	unsigned long		rx_sram_addr;
	int			rx_sram_size;
	unsigned long		tx_sram_addr;
	int			tx_sram_size;
};


#endif
