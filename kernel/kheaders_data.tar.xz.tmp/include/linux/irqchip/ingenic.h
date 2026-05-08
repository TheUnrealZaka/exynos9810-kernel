

#ifndef __LINUX_IRQCHIP_INGENIC_H__
#define __LINUX_IRQCHIP_INGENIC_H__

#include <linux/irq.h>

extern void ingenic_intc_irq_suspend(struct irq_data *data);
extern void ingenic_intc_irq_resume(struct irq_data *data);

#endif
