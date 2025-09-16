
#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

#include <asm/lse.h>
#include <asm/spinlock_types.h>
#include <asm/processor.h>


static inline void arch_spin_unlock_wait(arch_spinlock_t *lock)
{
	unsigned int tmp;
	arch_spinlock_t lockval;
	u32 owner;

	
	smp_mb();
	owner = READ_ONCE(lock->owner) << 16;

	asm volatile(
"	sevl\n"
"1:	wfe\n"
"2:	ldaxr	%w0, %2\n"
	
"	eor	%w1, %w0, %w0, ror #16\n"
"	cbz	%w1, 3f\n"
	
"	eor	%w1, %w3, %w0, lsl #16\n"
"	cbz	%w1, 1b\n"
	
"	b	4f\n"
"3:\n"
	
	ARM64_LSE_ATOMIC_INSN(
	
"	stxr	%w1, %w0, %2\n"
	__nops(2),
	
"	mov	%w1, %w0\n"
"	cas	%w0, %w0, %2\n"
"	eor	%w1, %w1, %w0\n")
	
"	cbnz	%w1, 2b\n"
"4:"
	: "=&r" (lockval), "=&r" (tmp), "+Q" (*lock)
	: "r" (owner)
	: "memory");
}

#define arch_spin_lock_flags(lock, flags) arch_spin_lock(lock)

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
	unsigned int tmp, tmp2;
	arch_spinlock_t lockval, newval;

	asm volatile(
	
	ARM64_LSE_ATOMIC_INSN(
	
"	mov	%w4, #0x1\n"
"	lsl	%w4, %w4, #31\n"
"	prfm	pstl1strm, %3\n"
"1:	ldaxr	%w0, %3\n"
"	add	%w1, %w0, %w6\n"
"	stxr	%w2, %w1, %3\n"
"	cbz	%w2, 4f\n"
"	add	%w4, %w4, #0x1\n"
"	b	1b\n",
	
"	mov	%w2, %w6\n"
"	ldadda	%w2, %w0, %3\n"
	__nops(7)
	)

	
"4:	eor	%w1, %w0, %w0, ror #16\n"
"	cbz	%w1, 3f\n"
	
"	sevl\n"
"2:	wfe\n"
"	ldaxrh	%w2, %5\n"
"	eor	%w1, %w2, %w0, lsr #16\n"
"	cbnz	%w1, 2b\n"
	
"3:"
	: "=&r" (lockval), "=&r" (newval), "=&r" (tmp), "+Q" (*lock), "=&r" (tmp2)
	: "Q" (lock->owner), "I" (1 << TICKET_SHIFT)
	: "memory");
}

static inline int arch_spin_trylock(arch_spinlock_t *lock)
{
	unsigned int tmp;
	arch_spinlock_t lockval;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"	prfm	pstl1strm, %2\n"
	"1:	ldaxr	%w0, %2\n"
	"	eor	%w1, %w0, %w0, ror #16\n"
	"	cbnz	%w1, 2f\n"
	"	add	%w0, %w0, %3\n"
	"	stxr	%w1, %w0, %2\n"
	"	cbnz	%w1, 1b\n"
	"2:",
	
	"	ldr	%w0, %2\n"
	"	eor	%w1, %w0, %w0, ror #16\n"
	"	cbnz	%w1, 1f\n"
	"	add	%w1, %w0, %3\n"
	"	casa	%w0, %w1, %2\n"
	"	sub	%w1, %w1, %3\n"
	"	eor	%w1, %w1, %w0\n"
	"1:")
	: "=&r" (lockval), "=&r" (tmp), "+Q" (*lock)
	: "I" (1 << TICKET_SHIFT)
	: "memory");

	return !tmp;
}

static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	unsigned long tmp;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"	ldrh	%w1, %0\n"
	"	add	%w1, %w1, #1\n"
	"	stlrh	%w1, %0",
	
	"	mov	%w1, #1\n"
	"	staddlh	%w1, %0\n"
	__nops(1))
	: "=Q" (lock->owner), "=&r" (tmp)
	:
	: "memory");
}

static inline int arch_spin_value_unlocked(arch_spinlock_t lock)
{
	return lock.owner == lock.next;
}

static inline int arch_spin_is_locked(arch_spinlock_t *lock)
{
	smp_mb(); 
	return !arch_spin_value_unlocked(READ_ONCE(*lock));
}

static inline int arch_spin_is_contended(arch_spinlock_t *lock)
{
	arch_spinlock_t lockval = READ_ONCE(*lock);
	return (lockval.next - lockval.owner) > 1;
}
#define arch_spin_is_contended	arch_spin_is_contended



static inline void arch_write_lock(arch_rwlock_t *rw)
{
	unsigned int tmp;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"	sevl\n"
	"1:	wfe\n"
	"2:	ldaxr	%w0, %1\n"
	"	cbnz	%w0, 1b\n"
	"	stxr	%w0, %w2, %1\n"
	"	cbnz	%w0, 2b\n"
	__nops(1),
	
	"1:	mov	%w0, wzr\n"
	"2:	casa	%w0, %w2, %1\n"
	"	cbz	%w0, 3f\n"
	"	ldxr	%w0, %1\n"
	"	cbz	%w0, 2b\n"
	"	wfe\n"
	"	b	1b\n"
	"3:")
	: "=&r" (tmp), "+Q" (rw->lock)
	: "r" (0x80000000)
	: "memory");
}

static inline int arch_write_trylock(arch_rwlock_t *rw)
{
	unsigned int tmp;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"1:	ldaxr	%w0, %1\n"
	"	cbnz	%w0, 2f\n"
	"	stxr	%w0, %w2, %1\n"
	"	cbnz	%w0, 1b\n"
	"2:",
	
	"	mov	%w0, wzr\n"
	"	casa	%w0, %w2, %1\n"
	__nops(2))
	: "=&r" (tmp), "+Q" (rw->lock)
	: "r" (0x80000000)
	: "memory");

	return !tmp;
}

static inline void arch_write_unlock(arch_rwlock_t *rw)
{
	asm volatile(ARM64_LSE_ATOMIC_INSN(
	"	stlr	wzr, %0",
	"	swpl	wzr, wzr, %0")
	: "=Q" (rw->lock) :: "memory");
}


#define arch_write_can_lock(x)		((x)->lock == 0)


static inline void arch_read_lock(arch_rwlock_t *rw)
{
	unsigned int tmp, tmp2;

	asm volatile(
	"	sevl\n"
	ARM64_LSE_ATOMIC_INSN(
	
	"1:	wfe\n"
	"2:	ldaxr	%w0, %2\n"
	"	add	%w0, %w0, #1\n"
	"	tbnz	%w0, #31, 1b\n"
	"	stxr	%w1, %w0, %2\n"
	"	cbnz	%w1, 2b\n"
	__nops(1),
	
	"1:	wfe\n"
	"2:	ldxr	%w0, %2\n"
	"	adds	%w1, %w0, #1\n"
	"	tbnz	%w1, #31, 1b\n"
	"	casa	%w0, %w1, %2\n"
	"	sbc	%w0, %w1, %w0\n"
	"	cbnz	%w0, 2b")
	: "=&r" (tmp), "=&r" (tmp2), "+Q" (rw->lock)
	:
	: "cc", "memory");
}

static inline void arch_read_unlock(arch_rwlock_t *rw)
{
	unsigned int tmp, tmp2;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"1:	ldxr	%w0, %2\n"
	"	sub	%w0, %w0, #1\n"
	"	stlxr	%w1, %w0, %2\n"
	"	cbnz	%w1, 1b",
	
	"	movn	%w0, #0\n"
	"	staddl	%w0, %2\n"
	__nops(2))
	: "=&r" (tmp), "=&r" (tmp2), "+Q" (rw->lock)
	:
	: "memory");
}

static inline int arch_read_trylock(arch_rwlock_t *rw)
{
	unsigned int tmp, tmp2;

	asm volatile(ARM64_LSE_ATOMIC_INSN(
	
	"	mov	%w1, #1\n"
	"1:	ldaxr	%w0, %2\n"
	"	add	%w0, %w0, #1\n"
	"	tbnz	%w0, #31, 2f\n"
	"	stxr	%w1, %w0, %2\n"
	"	cbnz	%w1, 1b\n"
	"2:",
	
	"	ldr	%w0, %2\n"
	"	adds	%w1, %w0, #1\n"
	"	tbnz	%w1, #31, 1f\n"
	"	casa	%w0, %w1, %2\n"
	"	sbc	%w1, %w1, %w0\n"
	__nops(1)
	"1:")
	: "=&r" (tmp), "=&r" (tmp2), "+Q" (rw->lock)
	:
	: "cc", "memory");

	return !tmp2;
}


#define arch_read_can_lock(x)		((x)->lock < 0x80000000)

#define arch_read_lock_flags(lock, flags) arch_read_lock(lock)
#define arch_write_lock_flags(lock, flags) arch_write_lock(lock)

#define arch_spin_relax(lock)	cpu_relax()
#define arch_read_relax(lock)	cpu_relax()
#define arch_write_relax(lock)	cpu_relax()


#define smp_mb__before_spinlock()	smp_mb()

#endif 
