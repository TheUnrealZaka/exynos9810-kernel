

#ifndef _LINUX_SRCU_H
#define _LINUX_SRCU_H

#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>

struct srcu_struct_array {
	unsigned long c[2];
	unsigned long seq[2];
};

struct rcu_batch {
	struct rcu_head *head, **tail;
};

#define RCU_BATCH_INIT(name) { NULL, &(name.head) }

struct srcu_struct {
	unsigned long completed;
	struct srcu_struct_array __percpu *per_cpu_ref;
	spinlock_t queue_lock; 
	bool running;
	
	struct rcu_batch batch_queue;
	
	struct rcu_batch batch_check0;
	
	struct rcu_batch batch_check1;
	struct rcu_batch batch_done;
	struct delayed_work work;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif 
};

#ifdef CONFIG_DEBUG_LOCK_ALLOC

int __init_srcu_struct(struct srcu_struct *sp, const char *name,
		       struct lock_class_key *key);

#define init_srcu_struct(sp) \
({ \
	static struct lock_class_key __srcu_key; \
	\
	__init_srcu_struct((sp), #sp, &__srcu_key); \
})

#define __SRCU_DEP_MAP_INIT(srcu_name)	.dep_map = { .name = #srcu_name },
#else 

int init_srcu_struct(struct srcu_struct *sp);

#define __SRCU_DEP_MAP_INIT(srcu_name)
#endif 

void process_srcu(struct work_struct *work);

#define __SRCU_STRUCT_INIT(name)					\
	{								\
		.completed = -300,					\
		.per_cpu_ref = &name##_srcu_array,			\
		.queue_lock = __SPIN_LOCK_UNLOCKED(name.queue_lock),	\
		.running = false,					\
		.batch_queue = RCU_BATCH_INIT(name.batch_queue),	\
		.batch_check0 = RCU_BATCH_INIT(name.batch_check0),	\
		.batch_check1 = RCU_BATCH_INIT(name.batch_check1),	\
		.batch_done = RCU_BATCH_INIT(name.batch_done),		\
		.work = __DELAYED_WORK_INITIALIZER(name.work, process_srcu, 0),\
		__SRCU_DEP_MAP_INIT(name)				\
	}


#define __DEFINE_SRCU(name, is_static)					\
	static DEFINE_PER_CPU(struct srcu_struct_array, name##_srcu_array);\
	is_static struct srcu_struct name = __SRCU_STRUCT_INIT(name)
#define DEFINE_SRCU(name)		__DEFINE_SRCU(name, )
#define DEFINE_STATIC_SRCU(name)	__DEFINE_SRCU(name, static)


void call_srcu(struct srcu_struct *sp, struct rcu_head *head,
		void (*func)(struct rcu_head *head));

void cleanup_srcu_struct(struct srcu_struct *sp);
int __srcu_read_lock(struct srcu_struct *sp) __acquires(sp);
void __srcu_read_unlock(struct srcu_struct *sp, int idx) __releases(sp);
void synchronize_srcu(struct srcu_struct *sp);
void synchronize_srcu_expedited(struct srcu_struct *sp);
unsigned long srcu_batches_completed(struct srcu_struct *sp);
void srcu_barrier(struct srcu_struct *sp);

#ifdef CONFIG_DEBUG_LOCK_ALLOC


static inline int srcu_read_lock_held(struct srcu_struct *sp)
{
	if (!debug_lockdep_rcu_enabled())
		return 1;
	return lock_is_held(&sp->dep_map);
}

#else 

static inline int srcu_read_lock_held(struct srcu_struct *sp)
{
	return 1;
}

#endif 


#define srcu_dereference_check(p, sp, c) \
	__rcu_dereference_check((p), (c) || srcu_read_lock_held(sp), __rcu)


#define srcu_dereference(p, sp) srcu_dereference_check((p), (sp), 0)


static inline int srcu_read_lock(struct srcu_struct *sp) __acquires(sp)
{
	int retval;

	preempt_disable();
	retval = __srcu_read_lock(sp);
	preempt_enable();
	rcu_lock_acquire(&(sp)->dep_map);
	return retval;
}


static inline void srcu_read_unlock(struct srcu_struct *sp, int idx)
	__releases(sp)
{
	rcu_lock_release(&(sp)->dep_map);
	__srcu_read_unlock(sp, idx);
}


static inline void smp_mb__after_srcu_read_unlock(void)
{
	
}

#endif
