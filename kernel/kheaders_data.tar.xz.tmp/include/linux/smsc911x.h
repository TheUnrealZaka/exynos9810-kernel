
#ifndef __LINUX_SMSC911X_H__
#define __LINUX_SMSC911X_H__

#include <linux/phy.h>
#include <linux/if_ether.h>


struct smsc911x_platform_config {
	unsigned int irq_polarity;
	unsigned int irq_type;
	unsigned int flags;
	unsigned int shift;
	phy_interface_t phy_interface;
	unsigned char mac[ETH_ALEN];
};


#define SMSC911X_IRQ_POLARITY_ACTIVE_LOW	0
#define SMSC911X_IRQ_POLARITY_ACTIVE_HIGH	1


#define SMSC911X_IRQ_TYPE_OPEN_DRAIN		0
#define SMSC911X_IRQ_TYPE_PUSH_PULL		1


#define SMSC911X_USE_16BIT 			(BIT(0))
#define SMSC911X_USE_32BIT 			(BIT(1))
#define SMSC911X_FORCE_INTERNAL_PHY		(BIT(2))
#define SMSC911X_FORCE_EXTERNAL_PHY 		(BIT(3))
#define SMSC911X_SAVE_MAC_ADDRESS		(BIT(4))


#define SMSC911X_SWAP_FIFO			(BIT(5))

#endif 
