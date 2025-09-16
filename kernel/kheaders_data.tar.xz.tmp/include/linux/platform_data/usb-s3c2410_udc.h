

#ifndef __ASM_ARM_ARCH_UDC_H
#define __ASM_ARM_ARCH_UDC_H

enum s3c2410_udc_cmd_e {
	S3C2410_UDC_P_ENABLE	= 1,	
	S3C2410_UDC_P_DISABLE	= 2,	
	S3C2410_UDC_P_RESET	= 3,	
};

struct s3c2410_udc_mach_info {
	void	(*udc_command)(enum s3c2410_udc_cmd_e);
	void	(*vbus_draw)(unsigned int ma);

	unsigned int pullup_pin;
	unsigned int pullup_pin_inverted;

	unsigned int vbus_pin;
	unsigned char vbus_pin_inverted;
};

extern void __init s3c24xx_udc_set_platdata(struct s3c2410_udc_mach_info *);

struct s3c24xx_hsudc_platdata;

extern void __init s3c24xx_hsudc_set_platdata(struct s3c24xx_hsudc_platdata *pd);

#endif 
