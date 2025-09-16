
#ifndef __LINUX_TINY_H
#define __LINUX_TINY_H

#include <linux/cache.h>

static inline unsigned long get_state_synchronize_rcu(void)
{
	return 0;
}

static inline void cond_synchronize_rcu(unsigned long oldstate)
{
	might_sleep();
}

static inline unsigned long get_state_synchronize_sched(void)
{
	return 0;
}

static inline void cond_synchronize_sched(unsigned long oldstate)
{
	might_sleep();
}

static inline void rcu_barrier_bh(void)
{
	wait_rcu_gp(call_rcu_bh);
}

static inline void rcu_barrier_sched(void)
{
	wait_rcu_gp(call_rcu_sched);
}

static inline void synchronize_rcu_expedited(void)
{
	synchronize_sched();	
}

static inline void rcu_barrier(void)
{
	rcu_barrier_sched();  
}

static inline void synchronize_rcu_bh(void)
{
	synchronize_sched();
}

static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched();
}

static inline void synchronize_sched_expedited(void)
{
	synchronize_sched();
}

static inline void kfree_call_rcu(struct rcu_head *head,
				  rcu_callback_t func)
{
	call_rcu(head, func);
}

static inline void rcu_note_context_switch(void)
{
	rcu_sched_qs();
}


static inline void rcu_virt_note_context_switch(int cpu)
{
}


static inline unsigned long rcu_batches_started(void)
{
	return 0;
}


static inline unsigned long rcu_batches_started_bh(void)
{
	return 0;
}


static inline unsigned long rcu_batches_started_sched(void)
{
	return 0;
}


static inline unsigned long rcu_batches_completed(void)
{
	return 0;
}


static inline unsigned long rcu_batches_completed_bh(void)
{
	return 0;
}


static inline unsigned long rcu_batches_completed_sched(void)
{
	return 0;
}


static inline unsigned long rcu_exp_batches_completed(void)
{
	return 0;
}


static inline unsigned long rcu_exp_batches_completed_sched(void)
{
	return 0;
}

static inline void rcu_force_quiescent_state(void)
{
}

static inline void rcu_bh_force_quiescent_state(void)
{
}

static inline void rcu_sched_force_quiescent_state(void)
{
}

static inline void show_rcu_gp_kthreads(void)
{
}

static inline void rcu_cpu_stall_reset(void)
{
}

static inline void rcu_idle_enter(void)
{
}

static inline void rcu_idle_exit(void)
{
}

static inline void rcu_irq_enter(void)
{
}

static inline void rcu_irq_exit_irqson(void)
{
}

static inline void rcu_irq_enter_irqson(void)
{
}

static inline void rcu_irq_exit(void)
{
}

static inline void exit_rcu(void)
{
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern int rcu_scheduler_active __read_mostly;
void rcu_scheduler_starting(void);
#else 
static inline void rcu_scheduler_starting(void)
{
}
#endif 

#if defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE)

static inline bool rcu_is_watching(void)
{
	return __rcu_is_watching();
}

#else 

static inline bool rcu_is_watching(void)
{
	return true;
}

#endif 

static inline void rcu_all_qs(void)
{
	barrier(); 
}


#define rcutree_prepare_cpu      NULL
#define rcutree_online_cpu       NULL
#define rcutree_offline_cpu      NULL
#define rcutree_dead_cpu         NULL
#define rcutree_dying_cpu        NULL

#endif 
