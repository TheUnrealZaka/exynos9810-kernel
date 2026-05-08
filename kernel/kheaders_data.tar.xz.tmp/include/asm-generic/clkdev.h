
#ifndef __ASM_CLKDEV_H
#define __ASM_CLKDEV_H

#include <linux/slab.h>

#ifndef CONFIG_COMMON_CLK
struct clk;

static inline int __clk_get(struct clk *clk) { return 1; }
static inline void __clk_put(struct clk *clk) { }
#endif

static inline struct clk_lookup_alloc *__clkdev_alloc(size_t size)
{
	return kzalloc(size, GFP_KERNEL);
}

#endif
