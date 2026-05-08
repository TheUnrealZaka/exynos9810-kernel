

#ifndef __ATA_SAMSUNG_CF_H
#define __ATA_SAMSUNG_CF_H __FILE__


struct s3c_ide_platdata {
	void (*setup_gpio)(void);
};


extern void s3c_ide_set_platdata(struct s3c_ide_platdata *pdata);


extern void s3c64xx_ide_setup_gpio(void);
extern void s5pv210_ide_setup_gpio(void);

#endif 
