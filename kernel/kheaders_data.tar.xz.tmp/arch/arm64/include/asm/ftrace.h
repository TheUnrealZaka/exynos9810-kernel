
#ifndef __ASM_FTRACE_H
#define __ASM_FTRACE_H

#include <asm/insn.h>

#define MCOUNT_ADDR		((unsigned long)_mcount)
#define MCOUNT_INSN_SIZE	AARCH64_INSN_SIZE

#ifndef __ASSEMBLY__
#include <linux/compat.h>

extern void _mcount(unsigned long);
extern void *return_address(unsigned int);

struct dyn_arch_ftrace {
	
};

extern unsigned long ftrace_graph_call;

extern void return_to_handler(void);

static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	
	return addr;
}

#define ftrace_return_address(n) return_address(n)


#define ARCH_TRACE_IGNORE_COMPAT_SYSCALLS
static inline bool arch_trace_is_compat_syscall(struct pt_regs *regs)
{
	return is_compat_task();
}
#endif 

#endif 
