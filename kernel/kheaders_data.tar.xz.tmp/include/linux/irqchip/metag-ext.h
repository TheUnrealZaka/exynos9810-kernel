

#ifndef _LINUX_IRQCHIP_METAG_EXT_H_
#define _LINUX_IRQCHIP_METAG_EXT_H_

struct irq_data;
struct platform_device;


int init_external_IRQ(void);


void meta_intc_no_mask(void);



extern struct irq_chip meta_intc_edge_chip;
extern struct irq_chip meta_intc_level_chip;


void meta_intc_mask_irq_simple(struct irq_data *data);

void meta_intc_unmask_irq_simple(struct irq_data *data);

#endif 
