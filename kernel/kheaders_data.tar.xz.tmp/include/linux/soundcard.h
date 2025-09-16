
#ifndef SOUNDCARD_H
#define SOUNDCARD_H

#  include <asm/byteorder.h>
#include <uapi/linux/soundcard.h>

#  if defined(__BIG_ENDIAN)
#    define AFMT_S16_NE AFMT_S16_BE
#  elif defined(__LITTLE_ENDIAN)
#    define AFMT_S16_NE AFMT_S16_LE
#  else
#    error "could not determine byte order"
#  endif
#endif
