

#ifndef _LINUX_PERCPU_REFCOUNT_H
#define _LINUX_PERCPU_REFCOUNT_H

#include <linux/atomic.h>
#include <linux/kernel.h>
#include <linux/percpu.h>
#include <linux/rcupdate.h>
#include <linux/gfp.h>

struct percpu_ref;
typedef void (percpu_ref_func_t)(struct percpu_ref *);


enum {
	__PERCPU_REF_ATOMIC	= 1LU << 0,	
	__PERCPU_REF_DEAD	= 1LU << 1,	
	__PERCPU_REF_ATOMIC_DEAD = __PERCPU_REF_ATOMIC | __PERCPU_REF_DEAD,

	__PERCPU_REF_FLAG_BITS	= 2,
};


enum {
	
	PERCPU_REF_INIT_ATOMIC	= 1 << 0,

	
	PERCPU_REF_INIT_DEAD	= 1 << 1,
};

struct percpu_ref {
	atomic_long_t		count;
	
	unsigned long		percpu_count_ptr;
	percpu_ref_func_t	*release;
	percpu_ref_func_t	*confirm_switch;
	bool			force_atomic:1;
	struct rcu_head		rcu;
};

int __must_check percpu_ref_init(struct percpu_ref *ref,
				 percpu_ref_func_t *release, unsigned int flags,
				 gfp_t gfp);
void percpu_ref_exit(struct percpu_ref *ref);
void percpu_ref_switch_to_atomic(struct percpu_ref *ref,
				 percpu_ref_func_t *confirm_switch);
void percpu_ref_switch_to_percpu(struct percpu_ref *ref);
void percpu_ref_kill_and_confirm(struct percpu_ref *ref,
				 percpu_ref_func_t *confirm_kill);
void percpu_ref_reinit(struct percpu_ref *ref);


static inline void percpu_ref_kill(struct percpu_ref *ref)
{
	percpu_ref_kill_and_confirm(ref, NULL);
}


static inline bool __ref_is_percpu(struct percpu_ref *ref,
					  unsigned long __percpu **percpu_countp)
{
	unsigned long percpu_ptr;

	
	percpu_ptr = READ_ONCE(ref->percpu_count_ptr);

	
	smp_read_barrier_depends();

	
	if (unlikely(percpu_ptr & __PERCPU_REF_ATOMIC_DEAD))
		return false;

	*percpu_countp = (unsigned long __percpu *)percpu_ptr;
	return true;
}


static inline void percpu_ref_get_many(struct percpu_ref *ref, unsigned long nr)
{
	unsigned long __percpu *percpu_count;

	rcu_read_lock_sched();

	if (__ref_is_percpu(ref, &percpu_count))
		this_cpu_add(*percpu_count, nr);
	else
		atomic_long_add(nr, &ref->count);

	rcu_read_unlock_sched();
}


static inline void percpu_ref_get(struct percpu_ref *ref)
{
	percpu_ref_get_many(ref, 1);
}


static inline bool percpu_ref_tryget(struct percpu_ref *ref)
{
	unsigned long __percpu *percpu_count;
	bool ret;

	rcu_read_lock_sched();

	if (__ref_is_percpu(ref, &percpu_count)) {
		this_cpu_inc(*percpu_count);
		ret = true;
	} else {
		ret = atomic_long_inc_not_zero(&ref->count);
	}

	rcu_read_unlock_sched();

	return ret;
}


static inline bool percpu_ref_tryget_live(struct percpu_ref *ref)
{
	unsigned long __percpu *percpu_count;
	bool ret = false;

	rcu_read_lock_sched();

	if (__ref_is_percpu(ref, &percpu_count)) {
		this_cpu_inc(*percpu_count);
		ret = true;
	} else if (!(ref->percpu_count_ptr & __PERCPU_REF_DEAD)) {
		ret = atomic_long_inc_not_zero(&ref->count);
	}

	rcu_read_unlock_sched();

	return ret;
}


static inline void percpu_ref_put_many(struct percpu_ref *ref, unsigned long nr)
{
	unsigned long __percpu *percpu_count;

	rcu_read_lock_sched();

	if (__ref_is_percpu(ref, &percpu_count))
		this_cpu_sub(*percpu_count, nr);
	else if (unlikely(atomic_long_sub_and_test(nr, &ref->count)))
		ref->release(ref);

	rcu_read_unlock_sched();
}


static inline void percpu_ref_put(struct percpu_ref *ref)
{
	percpu_ref_put_many(ref, 1);
}


static inline bool percpu_ref_is_dying(struct percpu_ref *ref)
{
	return ref->percpu_count_ptr & __PERCPU_REF_DEAD;
}


static inline bool percpu_ref_is_zero(struct percpu_ref *ref)
{
	unsigned long __percpu *percpu_count;

	if (__ref_is_percpu(ref, &percpu_count))
		return false;
	return !atomic_long_read(&ref->count);
}

#endif
