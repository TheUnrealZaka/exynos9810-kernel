
#ifndef _LINUX_FLAT_H
#define _LINUX_FLAT_H

#include <asm/flat.h>
#include <uapi/linux/flat.h>



#include <asm/byteorder.h>

#define	OLD_FLAT_VERSION			0x00000002L
#define OLD_FLAT_RELOC_TYPE_TEXT	0
#define OLD_FLAT_RELOC_TYPE_DATA	1
#define OLD_FLAT_RELOC_TYPE_BSS		2

typedef union {
	unsigned long	value;
	struct {
# if defined(mc68000) && !defined(CONFIG_COLDFIRE)
		signed long offset : 30;
		unsigned long type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 
# elif defined(__BIG_ENDIAN_BITFIELD)
		unsigned long type : 2;
		signed long offset : 30;
#   	define OLD_FLAT_FLAG_RAM    0x1 
# elif defined(__LITTLE_ENDIAN_BITFIELD)
		signed long offset : 30;
		unsigned long type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 
# else
#   	error "Unknown bitfield order for flat files."
# endif
	} reloc;
} flat_v2_reloc_t;

#endif 
