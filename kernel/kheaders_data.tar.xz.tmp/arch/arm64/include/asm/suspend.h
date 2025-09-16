#ifndef __ASM_SUSPEND_H
#define __ASM_SUSPEND_H

#define NR_CTX_REGS 12
#define NR_CALLEE_SAVED_REGS 12


struct cpu_suspend_ctx {
	
	u64 ctx_regs[NR_CTX_REGS];
	u64 sp;
} __aligned(16);


struct sleep_stack_data {
	struct cpu_suspend_ctx	system_regs;
	unsigned long		callee_saved_regs[NR_CALLEE_SAVED_REGS];
};

extern unsigned long *sleep_save_stash;

extern int cpu_suspend(unsigned long arg, int (*fn)(unsigned long));
extern void cpu_resume(void);
int __cpu_suspend_enter(struct sleep_stack_data *state);
void __cpu_suspend_exit(void);
void _cpu_resume(void);

int swsusp_arch_suspend(void);
int swsusp_arch_resume(void);
int arch_hibernation_header_save(void *addr, unsigned int max_size);
int arch_hibernation_header_restore(void *addr);


int hibernate_resume_nonboot_cpu_disable(void);

#endif
