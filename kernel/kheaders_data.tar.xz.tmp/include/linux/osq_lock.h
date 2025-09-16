#ifndef __LINUX_OSQ_LOCK_H
#define __LINUX_OSQ_LOCK_H


struct optimistic_spin_node {
	struct optimistic_spin_node *next, *prev;
	int locked; 
	int cpu; 
};

struct optimistic_spin_queue {
	
	atomic_t tail;
};

#define OSQ_UNLOCKED_VAL (0)


#define OSQ_LOCK_UNLOCKED { ATOMIC_INIT(OSQ_UNLOCKED_VAL) }

static inline void osq_lock_init(struct optimistic_spin_queue *lock)
{
	atomic_set(&lock->tail, OSQ_UNLOCKED_VAL);
}

extern bool osq_lock(struct optimistic_spin_queue *lock);
extern void osq_unlock(struct optimistic_spin_queue *lock);

static inline bool osq_is_locked(struct optimistic_spin_queue *lock)
{
	return atomic_read(&lock->tail) != OSQ_UNLOCKED_VAL;
}

#endif
