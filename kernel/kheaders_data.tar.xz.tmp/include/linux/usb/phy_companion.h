

#ifndef __DRIVERS_PHY_COMPANION_H
#define __DRIVERS_PHY_COMPANION_H

#include <linux/usb/otg.h>


struct phy_companion {

	
	int	(*set_vbus)(struct phy_companion *x, bool enabled);

	
	int	(*start_srp)(struct phy_companion *x);
};

#endif 
