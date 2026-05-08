
#ifndef __ASM_GENERIC_QRWLOCK_H
#define __ASM_GENERIC_QRWLOCK_H

#include <linux/atomic.h>
#include <asm/barrier.h>
#include <asm/processor.h>

#include <asm-generic/qrwlock_types.h>


#define	_QW_WAITING	1		
#define	_QW_LOCKED	0xff		
#define	_QW_WMASK	0xff		
#define	_QR_SHIFT	8		
#define _QR_BIAS	(1U << _QR_SHIFT)


extern void queued_read_lock_slowpath(struct qrwlock *lock, u32 cnts);
extern void queued_write_lock_slowpath(struct qrwlock *lock);


static inline int queued_read_can_lock(struct qrwlock *lock)
{
	return !(atomic_read(&lock->cnts) & _QW_WMASK);
}


static inline int queued_write_can_lock(struct qrwlock *lock)
{
	return !atomic_read(&lock->cnts);
}


static inline int queued_read_trylock(struct qrwlock *lock)
{
	u32 cnts;

	cnts = atomic_read(&lock->cnts);
	if (likely(!(cnts & _QW_WMASK))) {
		cnts = (u32)atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
		if (likely(!(cnts & _QW_WMASK)))
			return 1;
		atomic_sub(_QR_BIAS, &lock->cnts);
	}
	return 0;
}


static inline int queued_write_trylock(struct qrwlock *lock)
{
	u32 cnts;

	cnts = atomic_read(&lock->cnts);
	if (unlikely(cnts))
		return 0;

	return likely(atomic_cmpxchg_acquire(&lock->cnts,
					     cnts, cnts | _QW_LOCKED) == cnts);
}

static inline void queued_read_lock(struct qrwlock *lock)
{
	u32 cnts;

	cnts = atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
	if (likely(!(cnts & _QW_WMASK)))
		return;

	
	queued_read_lock_slowpath(lock, cnts);
}


static inline void queued_write_lock(struct qrwlock *lock)
{
	
	if (atomic_cmpxchg_acquire(&lock->cnts, 0, _QW_LOCKED) == 0)
		return;

	queued_write_lock_slowpath(lock);
}


static inline void queued_read_unlock(struct qrwlock *lock)
{
	
	(void)atomic_sub_return_release(_QR_BIAS, &lock->cnts);
}


static inline u8 *__qrwlock_write_byte(struct qrwlock *lock)
{
	return (u8 *)lock + 3 * IS_BUILTIN(CONFIG_CPU_BIG_ENDIAN);
}


static inline void queued_write_unlock(struct qrwlock *lock)
{
	smp_store_release(__qrwlock_write_byte(lock), 0);
}


#define arch_read_can_lock(l)	queued_read_can_lock(l)
#define arch_write_can_lock(l)	queued_write_can_lock(l)
#define arch_read_lock(l)	queued_read_lock(l)
#define arch_write_lock(l)	queued_write_lock(l)
#define arch_read_trylock(l)	queued_read_trylock(l)
#define arch_write_trylock(l)	queued_write_trylock(l)
#define arch_read_unlock(l)	queued_read_unlock(l)
#define arch_write_unlock(l)	queued_write_unlock(l)

#endif 
