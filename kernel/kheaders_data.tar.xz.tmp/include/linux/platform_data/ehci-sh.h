

#ifndef __USB_EHCI_SH_H
#define __USB_EHCI_SH_H

struct ehci_sh_platdata {
	void (*phy_init)(void); 
};

#endif 
