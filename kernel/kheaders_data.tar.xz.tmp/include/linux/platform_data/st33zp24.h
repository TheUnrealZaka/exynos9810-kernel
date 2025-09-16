
#ifndef __ST33ZP24_H__
#define __ST33ZP24_H__

#define TPM_ST33_I2C			"st33zp24-i2c"
#define TPM_ST33_SPI			"st33zp24-spi"

struct st33zp24_platform_data {
	int io_lpcpd;
};

#endif 
