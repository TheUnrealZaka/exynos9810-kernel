#ifndef __LINUX_PREEMPT_H
#define __LINUX_PREEMPT_H



#include <linux/linkage.h>
#include <linux/list.h>


#define PREEMPT_BITS	8
#define SOFTIRQ_BITS	8
#define HARDIRQ_BITS	4
#define NMI_BITS	1

#define PREEMPT_SHIFT	0
#define SOFTIRQ_SHIFT	(PREEMPT_SHIFT + PREEMPT_BITS)
#define HARDIRQ_SHIFT	(SOFTIRQ_SHIFT + SOFTIRQ_BITS)
#define NMI_SHIFT	(HARDIRQ_SHIFT + HARDIRQ_BITS)

#define __IRQ_MASK(x)	((1UL << (x))-1)

#define PREEMPT_MASK	(__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
#define SOFTIRQ_MASK	(__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
#define HARDIRQ_MASK	(__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)
#define NMI_MASK	(__IRQ_MASK(NMI_BITS)     << NMI_SHIFT)

#define PREEMPT_OFFSET	(1UL << PREEMPT_SHIFT)
#define SOFTIRQ_OFFSET	(1UL << SOFTIRQ_SHIFT)
#define HARDIRQ_OFFSET	(1UL << HARDIRQ_SHIFT)
#define NMI_OFFSET	(1UL << NMI_SHIFT)

#define SOFTIRQ_DISABLE_OFFSET	(2 * SOFTIRQ_OFFSET)


#define PREEMPT_NEED_RESCHED	0x80000000


#include <asm/preempt.h>

#define hardirq_count()	(preempt_count() & HARDIRQ_MASK)
#define softirq_count()	(preempt_count() & SOFTIRQ_MASK)
#define irq_count()	(preempt_count() & (HARDIRQ_MASK | SOFTIRQ_MASK \
				 | NMI_MASK))


#define in_irq()		(hardirq_count())
#define in_softirq()		(softirq_count())
#define in_interrupt()		(irq_count())
#define in_serving_softirq()	(softirq_count() & SOFTIRQ_OFFSET)
#define in_nmi()		(preempt_count() & NMI_MASK)
#define in_task()		(!(preempt_count() & \
				   (NMI_MASK | HARDIRQ_MASK | SOFTIRQ_OFFSET)))


#if defined(CONFIG_PREEMPT_COUNT)
# define PREEMPT_DISABLE_OFFSET	PREEMPT_OFFSET
#else
# define PREEMPT_DISABLE_OFFSET	0
#endif


#define PREEMPT_LOCK_OFFSET	PREEMPT_DISABLE_OFFSET


#define SOFTIRQ_LOCK_OFFSET (SOFTIRQ_DISABLE_OFFSET + PREEMPT_LOCK_OFFSET)


#define in_atomic()	(preempt_count() != 0)


#define in_atomic_preempt_off() (preempt_count() != PREEMPT_DISABLE_OFFSET)

#if defined(CONFIG_DEBUG_PREEMPT) || defined(CONFIG_PREEMPT_TRACER)
extern void preempt_count_add(int val);
extern void preempt_count_sub(int val);
#define preempt_count_dec_and_test() \
	({ preempt_count_sub(1); should_resched(0); })
#else
#define preempt_count_add(val)	__preempt_count_add(val)
#define preempt_count_sub(val)	__preempt_count_sub(val)
#define preempt_count_dec_and_test() __preempt_count_dec_and_test()
#endif

#define __preempt_count_inc() __preempt_count_add(1)
#define __preempt_count_dec() __preempt_count_sub(1)

#define preempt_count_inc() preempt_count_add(1)
#define preempt_count_dec() preempt_count_sub(1)

#ifdef CONFIG_PREEMPT_COUNT

#define preempt_disable() \
do { \
	preempt_count_inc(); \
	barrier(); \
} while (0)

#define sched_preempt_enable_no_resched() \
do { \
	barrier(); \
	preempt_count_dec(); \
} while (0)

#define preempt_enable_no_resched() sched_preempt_enable_no_resched()

#define preemptible()	(preempt_count() == 0 && !irqs_disabled())

#ifdef CONFIG_PREEMPT
#define preempt_enable() \
do { \
	barrier(); \
	if (unlikely(preempt_count_dec_and_test())) \
		__preempt_schedule(); \
} while (0)

#define preempt_enable_notrace() \
do { \
	barrier(); \
	if (unlikely(__preempt_count_dec_and_test())) \
		__preempt_schedule_notrace(); \
} while (0)

#define preempt_check_resched() \
do { \
	if (should_resched(0)) \
		__preempt_schedule(); \
} while (0)

#else 
#define preempt_enable() \
do { \
	barrier(); \
	preempt_count_dec(); \
} while (0)

#define preempt_enable_notrace() \
do { \
	barrier(); \
	__preempt_count_dec(); \
} while (0)

#define preempt_check_resched() do { } while (0)
#endif 

#define preempt_disable_notrace() \
do { \
	__preempt_count_inc(); \
	barrier(); \
} while (0)

#define preempt_enable_no_resched_notrace() \
do { \
	barrier(); \
	__preempt_count_dec(); \
} while (0)

#else 


#define preempt_disable()			barrier()
#define sched_preempt_enable_no_resched()	barrier()
#define preempt_enable_no_resched()		barrier()
#define preempt_enable()			barrier()
#define preempt_check_resched()			do { } while (0)

#define preempt_disable_notrace()		barrier()
#define preempt_enable_no_resched_notrace()	barrier()
#define preempt_enable_notrace()		barrier()
#define preemptible()				0

#endif 

#ifdef MODULE

#undef sched_preempt_enable_no_resched
#undef preempt_enable_no_resched
#undef preempt_enable_no_resched_notrace
#undef preempt_check_resched
#endif

#define preempt_set_need_resched() \
do { \
	set_preempt_need_resched(); \
} while (0)
#define preempt_fold_need_resched() \
do { \
	if (tif_need_resched()) \
		set_preempt_need_resched(); \
} while (0)

#ifdef CONFIG_PREEMPT_NOTIFIERS

struct preempt_notifier;


struct preempt_ops {
	void (*sched_in)(struct preempt_notifier *notifier, int cpu);
	void (*sched_out)(struct preempt_notifier *notifier,
			  struct task_struct *next);
};


struct preempt_notifier {
	struct hlist_node link;
	struct preempt_ops *ops;
};

void preempt_notifier_inc(void);
void preempt_notifier_dec(void);
void preempt_notifier_register(struct preempt_notifier *notifier);
void preempt_notifier_unregister(struct preempt_notifier *notifier);

static inline void preempt_notifier_init(struct preempt_notifier *notifier,
				     struct preempt_ops *ops)
{
	INIT_HLIST_NODE(&notifier->link);
	notifier->ops = ops;
}

#endif

#endif 
