

#ifndef _TVAUDIO_H
#define _TVAUDIO_H

#include <media/i2c-addr.h>


#define TVAUDIO_INPUT_TUNER  0
#define TVAUDIO_INPUT_RADIO  1
#define TVAUDIO_INPUT_EXTERN 2
#define TVAUDIO_INPUT_INTERN 3

static inline const unsigned short *tvaudio_addrs(void)
{
	static const unsigned short addrs[] = {
		I2C_ADDR_TDA8425   >> 1,
		I2C_ADDR_TEA6300   >> 1,
		I2C_ADDR_TEA6420   >> 1,
		I2C_ADDR_TDA9840   >> 1,
		I2C_ADDR_TDA985x_L >> 1,
		I2C_ADDR_TDA985x_H >> 1,
		I2C_ADDR_TDA9874   >> 1,
		I2C_ADDR_PIC16C54  >> 1,
		I2C_CLIENT_END
	};

	return addrs;
}

#endif
