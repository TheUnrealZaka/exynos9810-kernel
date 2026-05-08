

#ifndef __LINUX_PLATFORM_DATA_ATMEL_MXT_TS_H
#define __LINUX_PLATFORM_DATA_ATMEL_MXT_TS_H

#include <linux/types.h>

enum mxt_suspend_mode {
	MXT_SUSPEND_DEEP_SLEEP	= 0,
	MXT_SUSPEND_T9_CTRL	= 1,
};


struct mxt_platform_data {
	unsigned long irqflags;
	u8 t19_num_keys;
	const unsigned int *t19_keymap;
	enum mxt_suspend_mode suspend_mode;
};

#endif 
