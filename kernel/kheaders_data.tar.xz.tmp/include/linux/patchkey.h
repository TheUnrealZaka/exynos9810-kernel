
#ifndef _LINUX_PATCHKEY_H
#define _LINUX_PATCHKEY_H

#  include <asm/byteorder.h>
#include <uapi/linux/patchkey.h>

#  if defined(__BIG_ENDIAN)
#    define _PATCHKEY(id) (0xfd00|id)
#  elif defined(__LITTLE_ENDIAN)
#    define _PATCHKEY(id) ((id<<8)|0x00fd)
#  else
#    error "could not determine byte order"
#  endif
#endif 
