
#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <linux/cache.h>
#include <linux/threads.h>
#include <asm/irq.h>

#define NR_IPI	6

typedef struct {
	unsigned int __softirq_pending;
	unsigned int ipi_irqs[NR_IPI];
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	

#define __inc_irq_stat(cpu, member)	__IRQ_STAT(cpu, member)++
#define __get_irq_stat(cpu, member)	__IRQ_STAT(cpu, member)

u64 smp_irq_stat_cpu(unsigned int cpu);
#define arch_irq_stat_cpu	smp_irq_stat_cpu

#define __ARCH_IRQ_EXIT_IRQS_DISABLED	1

static inline void ack_bad_irq(unsigned int irq)
{
	extern unsigned long irq_err_count;
	irq_err_count++;
}

#endif 
