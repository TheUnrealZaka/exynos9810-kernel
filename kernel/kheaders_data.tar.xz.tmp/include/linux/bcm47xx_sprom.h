

#ifndef __BCM47XX_SPROM_H
#define __BCM47XX_SPROM_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>

#ifdef CONFIG_BCM47XX_SPROM
int bcm47xx_sprom_register_fallbacks(void);
#else
static inline int bcm47xx_sprom_register_fallbacks(void)
{
	return -ENOTSUPP;
};
#endif

#endif 
