

#ifndef __MV_PLATFORM_USB_H
#define __MV_PLATFORM_USB_H

enum pxa_ehci_type {
	EHCI_UNDEFINED = 0,
	PXA_U2OEHCI,	
	PXA_SPH,	
	MMP3_HSIC,	
	MMP3_FSIC,	
};

enum {
	MV_USB_MODE_OTG,
	MV_USB_MODE_HOST,
};

enum {
	VBUS_LOW	= 0,
	VBUS_HIGH	= 1 << 0,
};

struct mv_usb_addon_irq {
	unsigned int	irq;
	int		(*poll)(void);
};

struct mv_usb_platform_data {
	struct mv_usb_addon_irq	*id;	
	struct mv_usb_addon_irq	*vbus;	

	
	unsigned int		mode;

	
	unsigned int    disable_otg_clock_gating:1;
	
	 unsigned int    otg_force_a_bus_req:1;

	int	(*phy_init)(void __iomem *regbase);
	void	(*phy_deinit)(void __iomem *regbase);
	int	(*set_vbus)(unsigned int vbus);
	int     (*private_init)(void __iomem *opregs, void __iomem *phyregs);
};
#endif
