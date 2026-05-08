

#ifndef _LINUX_JOYSTICK_H
#define _LINUX_JOYSTICK_H

#include <uapi/linux/joystick.h>

#if BITS_PER_LONG == 64
#define JS_DATA_SAVE_TYPE JS_DATA_SAVE_TYPE_64
#elif BITS_PER_LONG == 32
#define JS_DATA_SAVE_TYPE JS_DATA_SAVE_TYPE_32
#else
#error Unexpected BITS_PER_LONG
#endif
#endif 
