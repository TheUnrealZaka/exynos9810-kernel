
#ifndef IRQBYPASS_H
#define IRQBYPASS_H

#include <linux/list.h>

struct irq_bypass_consumer;




struct irq_bypass_producer {
	struct list_head node;
	void *token;
	int irq;
	int (*add_consumer)(struct irq_bypass_producer *,
			    struct irq_bypass_consumer *);
	void (*del_consumer)(struct irq_bypass_producer *,
			     struct irq_bypass_consumer *);
	void (*stop)(struct irq_bypass_producer *);
	void (*start)(struct irq_bypass_producer *);
};


struct irq_bypass_consumer {
	struct list_head node;
	void *token;
	int (*add_producer)(struct irq_bypass_consumer *,
			    struct irq_bypass_producer *);
	void (*del_producer)(struct irq_bypass_consumer *,
			     struct irq_bypass_producer *);
	void (*stop)(struct irq_bypass_consumer *);
	void (*start)(struct irq_bypass_consumer *);
};

int irq_bypass_register_producer(struct irq_bypass_producer *);
void irq_bypass_unregister_producer(struct irq_bypass_producer *);
int irq_bypass_register_consumer(struct irq_bypass_consumer *);
void irq_bypass_unregister_consumer(struct irq_bypass_consumer *);

#endif 
