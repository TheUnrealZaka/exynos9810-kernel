
#ifndef __IRQCHIP_CHAINED_IRQ_H
#define __IRQCHIP_CHAINED_IRQ_H

#include <linux/irq.h>


static inline void chained_irq_enter(struct irq_chip *chip,
				     struct irq_desc *desc)
{
	
	if (chip->irq_eoi)
		return;

	if (chip->irq_mask_ack) {
		chip->irq_mask_ack(&desc->irq_data);
	} else {
		chip->irq_mask(&desc->irq_data);
		if (chip->irq_ack)
			chip->irq_ack(&desc->irq_data);
	}
}

static inline void chained_irq_exit(struct irq_chip *chip,
				    struct irq_desc *desc)
{
	if (chip->irq_eoi)
		chip->irq_eoi(&desc->irq_data);
	else
		chip->irq_unmask(&desc->irq_data);
}

#endif 
