
#ifndef __LINUX_PXA168_ETH_H
#define __LINUX_PXA168_ETH_H

#include <linux/phy.h>

struct pxa168_eth_platform_data {
	int	port_number;
	int	phy_addr;

	
	int	speed;		
	int	duplex;		
	phy_interface_t intf;

	
	int	rx_queue_size;
	int	tx_queue_size;

	
	int (*init)(void);
};

#endif 
