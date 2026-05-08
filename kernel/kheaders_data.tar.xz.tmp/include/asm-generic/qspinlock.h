
#ifndef __ASM_GENERIC_QSPINLOCK_H
#define __ASM_GENERIC_QSPINLOCK_H

#include <asm-generic/qspinlock_types.h>


#ifndef queued_spin_unlock_wait
extern void queued_spin_unlock_wait(struct qspinlock *lock);
#endif


#ifndef queued_spin_is_locked
static __always_inline int queued_spin_is_locked(struct qspinlock *lock)
{
	
	return atomic_read(&lock->val);
}
#endif


static __always_inline int queued_spin_value_unlocked(struct qspinlock lock)
{
	return !atomic_read(&lock.val);
}


static __always_inline int queued_spin_is_contended(struct qspinlock *lock)
{
	return atomic_read(&lock->val) & ~_Q_LOCKED_MASK;
}

static __always_inline int queued_spin_trylock(struct qspinlock *lock)
{
	if (!atomic_read(&lock->val) &&
	   (atomic_cmpxchg_acquire(&lock->val, 0, _Q_LOCKED_VAL) == 0))
		return 1;
	return 0;
}

extern void queued_spin_lock_slowpath(struct qspinlock *lock, u32 val);


static __always_inline void queued_spin_lock(struct qspinlock *lock)
{
	u32 val;

	val = atomic_cmpxchg_acquire(&lock->val, 0, _Q_LOCKED_VAL);
	if (likely(val == 0))
		return;
	queued_spin_lock_slowpath(lock, val);
}

#ifndef queued_spin_unlock

static __always_inline void queued_spin_unlock(struct qspinlock *lock)
{
	
	(void)atomic_sub_return_release(_Q_LOCKED_VAL, &lock->val);
}
#endif

#ifndef virt_spin_lock
static __always_inline bool virt_spin_lock(struct qspinlock *lock)
{
	return false;
}
#endif


#define arch_spin_is_locked(l)		queued_spin_is_locked(l)
#define arch_spin_is_contended(l)	queued_spin_is_contended(l)
#define arch_spin_value_unlocked(l)	queued_spin_value_unlocked(l)
#define arch_spin_lock(l)		queued_spin_lock(l)
#define arch_spin_trylock(l)		queued_spin_trylock(l)
#define arch_spin_unlock(l)		queued_spin_unlock(l)
#define arch_spin_lock_flags(l, f)	queued_spin_lock(l)
#define arch_spin_unlock_wait(l)	queued_spin_unlock_wait(l)

#endif 
