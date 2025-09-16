

#ifndef _CLOCKSOURCE_PXA_H
#define _CLOCKSOURCE_PXA_H

extern void pxa_timer_nodt_init(int irq, void __iomem *base,
			   unsigned long clock_tick_rate);

#endif
