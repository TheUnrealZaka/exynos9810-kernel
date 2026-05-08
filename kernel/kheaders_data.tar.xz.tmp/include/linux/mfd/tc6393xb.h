

#ifndef MFD_TC6393XB_H
#define MFD_TC6393XB_H

#include <linux/fb.h>


struct tc6393xb_platform_data {
	u16	scr_pll2cr;	
	u16	scr_gper;	

	int	(*enable)(struct platform_device *dev);
	int	(*disable)(struct platform_device *dev);
	int	(*suspend)(struct platform_device *dev);
	int	(*resume)(struct platform_device *dev);

	int	irq_base;	
	int	gpio_base;
	int	(*setup)(struct platform_device *dev);
	void	(*teardown)(struct platform_device *dev);

	struct tmio_nand_data	*nand_data;
	struct tmio_fb_data	*fb_data;

	unsigned resume_restore : 1; 
};

extern int tc6393xb_lcd_mode(struct platform_device *fb,
			     const struct fb_videomode *mode);
extern int tc6393xb_lcd_set_power(struct platform_device *fb, bool on);


#define	IRQ_TC6393_NAND		0
#define	IRQ_TC6393_MMC		1
#define	IRQ_TC6393_OHCI		2
#define	IRQ_TC6393_FB		4

#define	TC6393XB_NR_IRQS	8

#endif
