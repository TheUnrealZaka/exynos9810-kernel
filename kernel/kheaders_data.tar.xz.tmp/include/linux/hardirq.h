#ifndef LINUX_HARDIRQ_H
#define LINUX_HARDIRQ_H

#include <linux/preempt.h>
#include <linux/lockdep.h>
#include <linux/ftrace_irq.h>
#include <linux/vtime.h>
#include <asm/hardirq.h>


extern void synchronize_irq(unsigned int irq);
extern bool synchronize_hardirq(unsigned int irq);

#if defined(CONFIG_TINY_RCU)

static inline void rcu_nmi_enter(void)
{
}

static inline void rcu_nmi_exit(void)
{
}

#else
extern void rcu_nmi_enter(void);
extern void rcu_nmi_exit(void);
#endif


#define __irq_enter()					\
	do {						\
		account_irq_enter_time(current);	\
		preempt_count_add(HARDIRQ_OFFSET);	\
		trace_hardirq_enter();			\
	} while (0)


extern void irq_enter(void);


#define __irq_exit()					\
	do {						\
		trace_hardirq_exit();			\
		account_irq_exit_time(current);		\
		preempt_count_sub(HARDIRQ_OFFSET);	\
	} while (0)


extern void irq_exit(void);

#define nmi_enter()						\
	do {							\
		printk_nmi_enter();				\
		lockdep_off();					\
		ftrace_nmi_enter();				\
		BUG_ON(in_nmi());				\
		preempt_count_add(NMI_OFFSET + HARDIRQ_OFFSET);	\
		rcu_nmi_enter();				\
		trace_hardirq_enter();				\
	} while (0)

#define nmi_exit()						\
	do {							\
		trace_hardirq_exit();				\
		rcu_nmi_exit();					\
		BUG_ON(!in_nmi());				\
		preempt_count_sub(NMI_OFFSET + HARDIRQ_OFFSET);	\
		ftrace_nmi_exit();				\
		lockdep_on();					\
		printk_nmi_exit();				\
	} while (0)

#endif 
