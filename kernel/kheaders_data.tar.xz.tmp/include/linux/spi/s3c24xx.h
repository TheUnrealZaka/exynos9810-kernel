

#ifndef __LINUX_SPI_S3C24XX_H
#define __LINUX_SPI_S3C24XX_H __FILE__

struct s3c2410_spi_info {
	int			 pin_cs;	
	unsigned int		 num_cs;	
	int			 bus_num;       

	unsigned int		 use_fiq:1;	

	void (*gpio_setup)(struct s3c2410_spi_info *spi, int enable);
	void (*set_cs)(struct s3c2410_spi_info *spi, int cs, int pol);
};

extern int s3c24xx_set_fiq(unsigned int irq, bool on);

#endif 
