
#ifndef __SOUND_AESS_H__
#define __SOUND_AESS_H__

#include <linux/kernel.h>
#include <linux/io.h>


#define AESS_AUTO_GATING_ENABLE_OFFSET			0x07c


#define AESS_AUTO_GATING_ENABLE_SHIFT			0


static inline void aess_enable_autogating(void __iomem *base)
{
	u32 v;

	
	v = 1 << AESS_AUTO_GATING_ENABLE_SHIFT;
	writel(v, base + AESS_AUTO_GATING_ENABLE_OFFSET);
}

#endif 
