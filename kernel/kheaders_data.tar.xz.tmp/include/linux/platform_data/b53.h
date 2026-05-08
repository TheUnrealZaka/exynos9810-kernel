

#ifndef __B53_H
#define __B53_H

#include <linux/kernel.h>

struct b53_platform_data {
	u32 chip_id;
	u16 enabled_ports;

	
	unsigned big_endian:1;
	void __iomem *regs;
};

#endif
