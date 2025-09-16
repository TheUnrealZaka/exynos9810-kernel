#ifndef _LINUX_CONTEXT_TRACKING_H
#define _LINUX_CONTEXT_TRACKING_H

#include <linux/sched.h>
#include <linux/vtime.h>
#include <linux/context_tracking_state.h>
#include <asm/ptrace.h>


#ifdef CONFIG_CONTEXT_TRACKING
extern void context_tracking_cpu_set(int cpu);


extern void __context_tracking_enter(enum ctx_state state);
extern void __context_tracking_exit(enum ctx_state state);

extern void context_tracking_enter(enum ctx_state state);
extern void context_tracking_exit(enum ctx_state state);
extern void context_tracking_user_enter(void);
extern void context_tracking_user_exit(void);

static inline void user_enter(void)
{
	if (context_tracking_is_enabled())
		context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit(void)
{
	if (context_tracking_is_enabled())
		context_tracking_exit(CONTEXT_USER);
}


static inline void user_enter_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_exit(CONTEXT_USER);
}

static inline enum ctx_state exception_enter(void)
{
	enum ctx_state prev_ctx;

	if (!context_tracking_is_enabled())
		return 0;

	prev_ctx = this_cpu_read(context_tracking.state);
	if (prev_ctx != CONTEXT_KERNEL)
		context_tracking_exit(prev_ctx);

	return prev_ctx;
}

static inline void exception_exit(enum ctx_state prev_ctx)
{
	if (context_tracking_is_enabled()) {
		if (prev_ctx != CONTEXT_KERNEL)
			context_tracking_enter(prev_ctx);
	}
}



static inline enum ctx_state ct_state(void)
{
	return context_tracking_is_enabled() ?
		this_cpu_read(context_tracking.state) : CONTEXT_DISABLED;
}
#else
static inline void user_enter(void) { }
static inline void user_exit(void) { }
static inline void user_enter_irqoff(void) { }
static inline void user_exit_irqoff(void) { }
static inline enum ctx_state exception_enter(void) { return 0; }
static inline void exception_exit(enum ctx_state prev_ctx) { }
static inline enum ctx_state ct_state(void) { return CONTEXT_DISABLED; }
#endif 

#define CT_WARN_ON(cond) WARN_ON(context_tracking_is_enabled() && (cond))

#ifdef CONFIG_CONTEXT_TRACKING_FORCE
extern void context_tracking_init(void);
#else
static inline void context_tracking_init(void) { }
#endif 


#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN

static inline void guest_enter_irqoff(void)
{
	if (vtime_accounting_cpu_enabled())
		vtime_guest_enter(current);
	else
		current->flags |= PF_VCPU;

	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_GUEST);

	
	if (!context_tracking_cpu_is_enabled())
		rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_exit(CONTEXT_GUEST);

	if (vtime_accounting_cpu_enabled())
		vtime_guest_exit(current);
	else
		current->flags &= ~PF_VCPU;
}

#else
static inline void guest_enter_irqoff(void)
{
	
	vtime_account_system(current);
	current->flags |= PF_VCPU;
	rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	
	vtime_account_system(current);
	current->flags &= ~PF_VCPU;
}
#endif 

static inline void guest_enter(void)
{
	unsigned long flags;

	local_irq_save(flags);
	guest_enter_irqoff();
	local_irq_restore(flags);
}

static inline void guest_exit(void)
{
	unsigned long flags;

	local_irq_save(flags);
	guest_exit_irqoff();
	local_irq_restore(flags);
}

#endif
