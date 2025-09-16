

#ifndef __LINUX_USB_S3C_HSOTG_H
#define __LINUX_USB_S3C_HSOTG_H

struct platform_device;

enum dwc2_hsotg_dmamode {
	S3C_HSOTG_DMA_NONE,	
	S3C_HSOTG_DMA_ONLY,	
	S3C_HSOTG_DMA_DRV,	
};


struct dwc2_hsotg_plat {
	enum dwc2_hsotg_dmamode	dma;
	unsigned int		is_osc:1;
	int                     phy_type;

	int (*phy_init)(struct platform_device *pdev, int type);
	int (*phy_exit)(struct platform_device *pdev, int type);
};

extern void dwc2_hsotg_set_platdata(struct dwc2_hsotg_plat *pd);

#endif 
