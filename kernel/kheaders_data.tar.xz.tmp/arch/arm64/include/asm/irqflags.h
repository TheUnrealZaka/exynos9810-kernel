
#ifndef __ASM_IRQFLAGS_H
#define __ASM_IRQFLAGS_H

#ifdef __KERNEL__

#include <asm/ptrace.h>
#include <linux/exynos-ss.h>


static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags;
	asm volatile(
		"mrs	%0, daif		// arch_local_irq_save\n"
		"msr	daifset, #2"
		: "=r" (flags)
		:
		: "memory");
	exynos_ss_irqs_disabled(0);
	return flags;
}

static inline void arch_local_irq_enable(void)
{
	exynos_ss_irqs_disabled(1);
	asm volatile(
		"msr	daifclr, #2		// arch_local_irq_enable"
		:
		:
		: "memory");
}

static inline void arch_local_irq_disable(void)
{
	asm volatile(
		"msr	daifset, #2		// arch_local_irq_disable"
		:
		:
		: "memory");
	exynos_ss_irqs_disabled(0);
}

#define local_fiq_enable()	asm("msr	daifclr, #1" : : : "memory")
#define local_fiq_disable()	asm("msr	daifset, #1" : : : "memory")

#define local_async_enable()	asm("msr	daifclr, #4" : : : "memory")
#define local_async_disable()	asm("msr	daifset, #4" : : : "memory")


static inline unsigned long arch_local_save_flags(void)
{
	unsigned long flags;
	asm volatile(
		"mrs	%0, daif		// arch_local_save_flags"
		: "=r" (flags)
		:
		: "memory");
	return flags;
}


static inline void arch_local_irq_restore(unsigned long flags)
{
	exynos_ss_irqs_disabled(flags & PSR_I_BIT);
	asm volatile(
		"msr	daif, %0		// arch_local_irq_restore"
	:
	: "r" (flags)
	: "memory");
}

static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return flags & PSR_I_BIT;
}


#define local_dbg_save(flags)						\
	do {								\
		typecheck(unsigned long, flags);			\
		asm volatile(						\
		"mrs    %0, daif		// local_dbg_save\n"	\
		"msr    daifset, #8"					\
		: "=r" (flags) : : "memory");				\
	} while (0)

#define local_dbg_restore(flags)					\
	do {								\
		typecheck(unsigned long, flags);			\
		asm volatile(						\
		"msr    daif, %0		// local_dbg_restore\n"	\
		: : "r" (flags) : "memory");				\
	} while (0)

#endif
#endif
