#ifndef __ASM_IRQ_H
#define __ASM_IRQ_H

#define IRQ_STACK_SIZE			THREAD_SIZE
#define IRQ_STACK_START_SP		THREAD_START_SP

#ifndef __ASSEMBLER__

#include <linux/percpu.h>

#include <asm-generic/irq.h>
#include <asm/thread_info.h>

struct pt_regs;

DECLARE_PER_CPU(unsigned long [IRQ_STACK_SIZE/sizeof(long)], irq_stack);


#define IRQ_STACK_PTR(cpu) ((unsigned long)per_cpu(irq_stack, cpu) + IRQ_STACK_START_SP)


#define IRQ_STACK_TO_TASK_STACK(ptr) (*((unsigned long *)((ptr) - 0x08)))

extern void set_handle_irq(void (*handle_irq)(struct pt_regs *));

static inline int nr_legacy_irqs(void)
{
	return 0;
}

static inline bool on_irq_stack(unsigned long sp, int cpu)
{
	
	unsigned long low = (unsigned long)per_cpu(irq_stack, cpu);
	unsigned long high = low + IRQ_STACK_START_SP;

	return (low <= sp && sp <= high);
}

#endif 
#endif
