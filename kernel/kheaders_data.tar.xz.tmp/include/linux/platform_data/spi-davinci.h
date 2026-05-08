

#ifndef __ARCH_ARM_DAVINCI_SPI_H
#define __ARCH_ARM_DAVINCI_SPI_H

#include <linux/platform_data/edma.h>

#define SPI_INTERN_CS	0xFF

enum {
	SPI_VERSION_1, 
	SPI_VERSION_2, 
};


struct davinci_spi_platform_data {
	u8			version;
	u8			num_chipselect;
	u8			intr_line;
	u8			*chip_sel;
	u8			prescaler_limit;
	bool			cshold_bug;
	enum dma_event_q	dma_event_q;
};


struct davinci_spi_config {
	u8	wdelay;
	u8	odd_parity;
	u8	parity_enable;
#define SPI_IO_TYPE_INTR	0
#define SPI_IO_TYPE_POLL	1
#define SPI_IO_TYPE_DMA		2
	u8	io_type;
	u8	timer_disable;
	u8	c2tdelay;
	u8	t2cdelay;
	u8	t2edelay;
	u8	c2edelay;
};

#endif	
