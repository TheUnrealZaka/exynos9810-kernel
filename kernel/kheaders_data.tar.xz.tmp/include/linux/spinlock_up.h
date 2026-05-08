#ifndef __LINUX_SPINLOCK_UP_H
#define __LINUX_SPINLOCK_UP_H

#ifndef __LINUX_SPINLOCK_H
# error "please don't include this file directly"
#endif

#include <asm/processor.h>	
#include <asm/barrier.h>



#ifdef CONFIG_DEBUG_SPINLOCK
#define arch_spin_is_locked(x)		((x)->slock == 0)

static inline void arch_spin_unlock_wait(arch_spinlock_t *lock)
{
	smp_cond_load_acquire(&lock->slock, VAL);
}

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
	lock->slock = 0;
	barrier();
}

static inline void
arch_spin_lock_flags(arch_spinlock_t *lock, unsigned long flags)
{
	local_irq_save(flags);
	lock->slock = 0;
	barrier();
}

static inline int arch_spin_trylock(arch_spinlock_t *lock)
{
	char oldval = lock->slock;

	lock->slock = 0;
	barrier();

	return oldval > 0;
}

static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	barrier();
	lock->slock = 1;
}


#define arch_read_lock(lock)		do { barrier(); (void)(lock); } while (0)
#define arch_write_lock(lock)		do { barrier(); (void)(lock); } while (0)
#define arch_read_trylock(lock)	({ barrier(); (void)(lock); 1; })
#define arch_write_trylock(lock)	({ barrier(); (void)(lock); 1; })
#define arch_read_unlock(lock)		do { barrier(); (void)(lock); } while (0)
#define arch_write_unlock(lock)	do { barrier(); (void)(lock); } while (0)

#else 
#define arch_spin_is_locked(lock)	((void)(lock), 0)
#define arch_spin_unlock_wait(lock)	do { barrier(); (void)(lock); } while (0)

# define arch_spin_lock(lock)		do { barrier(); (void)(lock); } while (0)
# define arch_spin_lock_flags(lock, flags)	do { barrier(); (void)(lock); } while (0)
# define arch_spin_unlock(lock)	do { barrier(); (void)(lock); } while (0)
# define arch_spin_trylock(lock)	({ barrier(); (void)(lock); 1; })
#endif 

#define arch_spin_is_contended(lock)	(((void)(lock), 0))

#define arch_read_can_lock(lock)	(((void)(lock), 1))
#define arch_write_can_lock(lock)	(((void)(lock), 1))

#endif 
