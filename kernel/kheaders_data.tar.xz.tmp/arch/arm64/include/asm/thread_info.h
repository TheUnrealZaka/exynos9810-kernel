
#ifndef __ASM_THREAD_INFO_H
#define __ASM_THREAD_INFO_H

#ifdef __KERNEL__

#include <linux/compiler.h>

#ifdef CONFIG_ARM64_4K_PAGES
#define THREAD_SIZE_ORDER	2
#elif defined(CONFIG_ARM64_16K_PAGES)
#define THREAD_SIZE_ORDER	0
#endif

#define THREAD_SIZE		16384
#define THREAD_START_SP		(THREAD_SIZE - 16)

#ifndef __ASSEMBLY__

struct task_struct;

#include <asm/stack_pointer.h>
#include <asm/types.h>

typedef unsigned long mm_segment_t;


struct thread_info {
	unsigned long		flags;		
	mm_segment_t		addr_limit;	
#ifdef CONFIG_ARM64_SW_TTBR0_PAN
	u64			ttbr0;		
#endif
	int			preempt_count;	
};

#define INIT_THREAD_INFO(tsk)						\
{									\
	.preempt_count	= INIT_PREEMPT_COUNT,				\
	.addr_limit	= KERNEL_DS,					\
}

#define init_stack		(init_thread_union.stack)

#define thread_saved_pc(tsk)	\
	((unsigned long)(tsk->thread.cpu_context.pc))
#define thread_saved_sp(tsk)	\
	((unsigned long)(tsk->thread.cpu_context.sp))
#define thread_saved_fp(tsk)	\
	((unsigned long)(tsk->thread.cpu_context.fp))

#endif


#define TIF_SIGPENDING		0
#define TIF_NEED_RESCHED	1
#define TIF_NOTIFY_RESUME	2	
#define TIF_FOREIGN_FPSTATE	3	
#define TIF_FSCHECK		4	
#define TIF_NOHZ		7
#define TIF_SYSCALL_TRACE	8
#define TIF_SYSCALL_AUDIT	9
#define TIF_SYSCALL_TRACEPOINT	10
#define TIF_SECCOMP		11
#define TIF_MEMDIE		18	
#define TIF_FREEZE		19
#define TIF_RESTORE_SIGMASK	20
#define TIF_SINGLESTEP		21
#define TIF_32BIT		22	
#define TIF_SSBD		23	
#define TIF_MEMALLOC		29	

#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
#define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
#define _TIF_FOREIGN_FPSTATE	(1 << TIF_FOREIGN_FPSTATE)
#define _TIF_NOHZ		(1 << TIF_NOHZ)
#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_SYSCALL_TRACEPOINT	(1 << TIF_SYSCALL_TRACEPOINT)
#define _TIF_SECCOMP		(1 << TIF_SECCOMP)
#define _TIF_FSCHECK		(1 << TIF_FSCHECK)
#define _TIF_32BIT		(1 << TIF_32BIT)

#define _TIF_WORK_MASK		(_TIF_NEED_RESCHED | _TIF_SIGPENDING | \
				 _TIF_NOTIFY_RESUME | _TIF_FOREIGN_FPSTATE | \
				 _TIF_FSCHECK)

#define _TIF_SYSCALL_WORK	(_TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT | \
				 _TIF_SYSCALL_TRACEPOINT | _TIF_SECCOMP | \
				 _TIF_NOHZ)

#endif 
#endif 
