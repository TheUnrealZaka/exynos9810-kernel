

#ifndef __USB_EHCI_ORION_H
#define __USB_EHCI_ORION_H

#include <linux/mbus.h>

enum orion_ehci_phy_ver {
	EHCI_PHY_ORION,
	EHCI_PHY_DD,
	EHCI_PHY_KW,
	EHCI_PHY_NA,
};

struct orion_ehci_data {
	enum orion_ehci_phy_ver phy_version;
};


#endif
