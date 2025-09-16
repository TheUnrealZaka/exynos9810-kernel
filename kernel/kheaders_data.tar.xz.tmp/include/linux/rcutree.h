

#ifndef __LINUX_RCUTREE_H
#define __LINUX_RCUTREE_H

void rcu_note_context_switch(void);
int rcu_needs_cpu(u64 basem, u64 *nextevt);
void rcu_cpu_stall_reset(void);


static inline void rcu_virt_note_context_switch(int cpu)
{
	rcu_note_context_switch();
}

void synchronize_rcu_bh(void);
void synchronize_sched_expedited(void);
void synchronize_rcu_expedited(void);

void kfree_call_rcu(struct rcu_head *head, rcu_callback_t func);


static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched_expedited();
}

void rcu_barrier(void);
void rcu_barrier_bh(void);
void rcu_barrier_sched(void);
unsigned long get_state_synchronize_rcu(void);
void cond_synchronize_rcu(unsigned long oldstate);
unsigned long get_state_synchronize_sched(void);
void cond_synchronize_sched(unsigned long oldstate);

extern unsigned long rcutorture_testseq;
extern unsigned long rcutorture_vernum;
unsigned long rcu_batches_started(void);
unsigned long rcu_batches_started_bh(void);
unsigned long rcu_batches_started_sched(void);
unsigned long rcu_batches_completed(void);
unsigned long rcu_batches_completed_bh(void);
unsigned long rcu_batches_completed_sched(void);
unsigned long rcu_exp_batches_completed(void);
unsigned long rcu_exp_batches_completed_sched(void);
void show_rcu_gp_kthreads(void);

void rcu_force_quiescent_state(void);
void rcu_bh_force_quiescent_state(void);
void rcu_sched_force_quiescent_state(void);

void rcu_idle_enter(void);
void rcu_idle_exit(void);
void rcu_irq_enter(void);
void rcu_irq_exit(void);
void rcu_irq_enter_irqson(void);
void rcu_irq_exit_irqson(void);

void exit_rcu(void);

void rcu_scheduler_starting(void);
extern int rcu_scheduler_active __read_mostly;

bool rcu_is_watching(void);

void rcu_all_qs(void);


int rcutree_prepare_cpu(unsigned int cpu);
int rcutree_online_cpu(unsigned int cpu);
int rcutree_offline_cpu(unsigned int cpu);
int rcutree_dead_cpu(unsigned int cpu);
int rcutree_dying_cpu(unsigned int cpu);

#endif 
