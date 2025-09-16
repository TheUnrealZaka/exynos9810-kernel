
#ifndef _LIBERTAS_SPI_H_
#define _LIBERTAS_SPI_H_

struct spi_device;

struct libertas_spi_platform_data {
	
	u16 use_dummy_writes;

	
	int (*setup)(struct spi_device *spi);
	int (*teardown)(struct spi_device *spi);
};
#endif
