
#ifndef __ASM_BYTEORDER_H
#define __ASM_BYTEORDER_H

#ifdef __AARCH64EB__
#include <linux/byteorder/big_endian.h>
#else
#include <linux/byteorder/little_endian.h>
#endif

#endif	
