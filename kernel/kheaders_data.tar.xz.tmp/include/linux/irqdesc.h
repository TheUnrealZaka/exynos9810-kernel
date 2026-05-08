#ifndef _LINUX_IRQDESC_H
#define _LINUX_IRQDESC_H

#include <linux/rcupdate.h>
#include <linux/kobject.h>



struct irq_affinity_notify;
struct proc_dir_entry;
struct module;
struct irq_desc;
struct irq_domain;
struct pt_regs;


struct irq_desc {
	struct irq_common_data	irq_common_data;
	struct irq_data		irq_data;
	unsigned int __percpu	*kstat_irqs;
	irq_flow_handler_t	handle_irq;
#ifdef CONFIG_IRQ_PREFLOW_FASTEOI
	irq_preflow_handler_t	preflow_handler;
#endif
	struct irqaction	*action;	
	unsigned int		status_use_accessors;
	unsigned int		core_internal_state__do_not_mess_with_it;
	unsigned int		depth;		
	unsigned int		wake_depth;	
	unsigned int		tot_count;
	unsigned int		irq_count;	
	unsigned long		last_unhandled;	
	unsigned int		irqs_unhandled;
	atomic_t		threads_handled;
	int			threads_handled_last;
	raw_spinlock_t		lock;
	struct cpumask		*percpu_enabled;
	const struct cpumask	*percpu_affinity;
#ifdef CONFIG_SMP
	const struct cpumask	*affinity_hint;
	struct irq_affinity_notify *affinity_notify;
#ifdef CONFIG_GENERIC_PENDING_IRQ
	cpumask_var_t		pending_mask;
#endif
#endif
	unsigned long		threads_oneshot;
	atomic_t		threads_active;
	wait_queue_head_t       wait_for_threads;
#ifdef CONFIG_PM_SLEEP
	unsigned int		nr_actions;
	unsigned int		no_suspend_depth;
	unsigned int		cond_suspend_depth;
	unsigned int		force_resume_depth;
#endif
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry	*dir;
#endif
#ifdef CONFIG_SPARSE_IRQ
	struct rcu_head		rcu;
	struct kobject		kobj;
#endif
	int			parent_irq;
	struct module		*owner;
	const char		*name;
} ____cacheline_internodealigned_in_smp;

#ifdef CONFIG_SPARSE_IRQ
extern void irq_lock_sparse(void);
extern void irq_unlock_sparse(void);
#else
static inline void irq_lock_sparse(void) { }
static inline void irq_unlock_sparse(void) { }
extern struct irq_desc irq_desc[NR_IRQS];
#endif

static inline struct irq_desc *irq_data_to_desc(struct irq_data *data)
{
	return container_of(data->common, struct irq_desc, irq_common_data);
}

static inline unsigned int irq_desc_get_irq(struct irq_desc *desc)
{
	return desc->irq_data.irq;
}

static inline struct irq_data *irq_desc_get_irq_data(struct irq_desc *desc)
{
	return &desc->irq_data;
}

static inline struct irq_chip *irq_desc_get_chip(struct irq_desc *desc)
{
	return desc->irq_data.chip;
}

static inline void *irq_desc_get_chip_data(struct irq_desc *desc)
{
	return desc->irq_data.chip_data;
}

static inline void *irq_desc_get_handler_data(struct irq_desc *desc)
{
	return desc->irq_common_data.handler_data;
}

static inline struct msi_desc *irq_desc_get_msi_desc(struct irq_desc *desc)
{
	return desc->irq_common_data.msi_desc;
}


static inline void generic_handle_irq_desc(struct irq_desc *desc)
{
	desc->handle_irq(desc);
}

int generic_handle_irq(unsigned int irq);

#ifdef CONFIG_HANDLE_DOMAIN_IRQ

int __handle_domain_irq(struct irq_domain *domain, unsigned int hwirq,
			bool lookup, struct pt_regs *regs);

static inline int handle_domain_irq(struct irq_domain *domain,
				    unsigned int hwirq, struct pt_regs *regs)
{
	return __handle_domain_irq(domain, hwirq, true, regs);
}
#endif


static inline int irq_desc_has_action(struct irq_desc *desc)
{
	return desc->action != NULL;
}

static inline int irq_has_action(unsigned int irq)
{
	return irq_desc_has_action(irq_to_desc(irq));
}


static inline void irq_set_handler_locked(struct irq_data *data,
					  irq_flow_handler_t handler)
{
	struct irq_desc *desc = irq_data_to_desc(data);

	desc->handle_irq = handler;
}


static inline void
irq_set_chip_handler_name_locked(struct irq_data *data, struct irq_chip *chip,
				 irq_flow_handler_t handler, const char *name)
{
	struct irq_desc *desc = irq_data_to_desc(data);

	desc->handle_irq = handler;
	desc->name = name;
	data->chip = chip;
}

static inline int irq_balancing_disabled(unsigned int irq)
{
	struct irq_desc *desc;

	desc = irq_to_desc(irq);
	return desc->status_use_accessors & IRQ_NO_BALANCING_MASK;
}

static inline int irq_is_percpu(unsigned int irq)
{
	struct irq_desc *desc;

	desc = irq_to_desc(irq);
	return desc->status_use_accessors & IRQ_PER_CPU;
}

static inline void
irq_set_lockdep_class(unsigned int irq, struct lock_class_key *class)
{
	struct irq_desc *desc = irq_to_desc(irq);

	if (desc)
		lockdep_set_class(&desc->lock, class);
}

#ifdef CONFIG_IRQ_PREFLOW_FASTEOI
static inline void
__irq_set_preflow_handler(unsigned int irq, irq_preflow_handler_t handler)
{
	struct irq_desc *desc;

	desc = irq_to_desc(irq);
	desc->preflow_handler = handler;
}
#endif

#endif
