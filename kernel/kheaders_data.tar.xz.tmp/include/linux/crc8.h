
#ifndef __CRC8_H_
#define __CRC8_H_

#include <linux/types.h>


#define CRC8_INIT_VALUE		0xFF


#define CRC8_GOOD_VALUE(_table)	(_table[0xFF])


#define CRC8_TABLE_SIZE			256


#define DECLARE_CRC8_TABLE(_table) \
	static u8 _table[CRC8_TABLE_SIZE]


void crc8_populate_lsb(u8 table[CRC8_TABLE_SIZE], u8 polynomial);


void crc8_populate_msb(u8 table[CRC8_TABLE_SIZE], u8 polynomial);


u8 crc8(const u8 table[CRC8_TABLE_SIZE], u8 *pdata, size_t nbytes, u8 crc);

#endif 
