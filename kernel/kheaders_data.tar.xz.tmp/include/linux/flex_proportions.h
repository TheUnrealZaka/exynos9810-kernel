

#ifndef _LINUX_FLEX_PROPORTIONS_H
#define _LINUX_FLEX_PROPORTIONS_H

#include <linux/percpu_counter.h>
#include <linux/spinlock.h>
#include <linux/seqlock.h>
#include <linux/gfp.h>


#define FPROP_FRAC_SHIFT 10
#define FPROP_FRAC_BASE (1UL << FPROP_FRAC_SHIFT)


struct fprop_global {
	
	struct percpu_counter events;
	
	unsigned int period;
	
	seqcount_t sequence;
};

int fprop_global_init(struct fprop_global *p, gfp_t gfp);
void fprop_global_destroy(struct fprop_global *p);
bool fprop_new_period(struct fprop_global *p, int periods);


struct fprop_local_single {
	
	unsigned long events;
	
	unsigned int period;
	raw_spinlock_t lock;	
};

#define INIT_FPROP_LOCAL_SINGLE(name)			\
{	.lock = __RAW_SPIN_LOCK_UNLOCKED(name.lock),	\
}

int fprop_local_init_single(struct fprop_local_single *pl);
void fprop_local_destroy_single(struct fprop_local_single *pl);
void __fprop_inc_single(struct fprop_global *p, struct fprop_local_single *pl);
void fprop_fraction_single(struct fprop_global *p,
	struct fprop_local_single *pl, unsigned long *numerator,
	unsigned long *denominator);

static inline
void fprop_inc_single(struct fprop_global *p, struct fprop_local_single *pl)
{
	unsigned long flags;

	local_irq_save(flags);
	__fprop_inc_single(p, pl);
	local_irq_restore(flags);
}


struct fprop_local_percpu {
	
	struct percpu_counter events;
	
	unsigned int period;
	raw_spinlock_t lock;	
};

int fprop_local_init_percpu(struct fprop_local_percpu *pl, gfp_t gfp);
void fprop_local_destroy_percpu(struct fprop_local_percpu *pl);
void __fprop_inc_percpu(struct fprop_global *p, struct fprop_local_percpu *pl);
void __fprop_inc_percpu_max(struct fprop_global *p, struct fprop_local_percpu *pl,
			    int max_frac);
void fprop_fraction_percpu(struct fprop_global *p,
	struct fprop_local_percpu *pl, unsigned long *numerator,
	unsigned long *denominator);

static inline
void fprop_inc_percpu(struct fprop_global *p, struct fprop_local_percpu *pl)
{
	unsigned long flags;

	local_irq_save(flags);
	__fprop_inc_percpu(p, pl);
	local_irq_restore(flags);
}

#endif
