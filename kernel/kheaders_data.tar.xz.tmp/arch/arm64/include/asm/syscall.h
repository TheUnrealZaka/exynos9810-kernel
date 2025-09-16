
#ifndef __ASM_SYSCALL_H
#define __ASM_SYSCALL_H

#include <uapi/linux/audit.h>
#include <linux/compat.h>
#include <linux/err.h>

extern const void *sys_call_table[];

static inline int syscall_get_nr(struct task_struct *task,
				 struct pt_regs *regs)
{
	return regs->syscallno;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->regs[0] = regs->orig_x0;
}


static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->regs[0];
	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->regs[0];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->regs[0] = (long) error ? error : val;
}

#define SYSCALL_MAX_ARGS 6

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	if (n == 0)
		return;

	if (i + n > SYSCALL_MAX_ARGS) {
		unsigned long *args_bad = args + SYSCALL_MAX_ARGS - i;
		unsigned int n_bad = n + i - SYSCALL_MAX_ARGS;
		pr_warning("%s called with max args %d, handling only %d\n",
			   __func__, i + n, SYSCALL_MAX_ARGS);
		memset(args_bad, 0, n_bad * sizeof(args[0]));
	}

	if (i == 0) {
		args[0] = regs->orig_x0;
		args++;
		i++;
		n--;
	}

	memcpy(args, &regs->regs[i], n * sizeof(args[0]));
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	if (n == 0)
		return;

	if (i + n > SYSCALL_MAX_ARGS) {
		pr_warning("%s called with max args %d, handling only %d\n",
			   __func__, i + n, SYSCALL_MAX_ARGS);
		n = SYSCALL_MAX_ARGS - i;
	}

	if (i == 0) {
		regs->orig_x0 = args[0];
		args++;
		i++;
		n--;
	}

	memcpy(&regs->regs[i], args, n * sizeof(args[0]));
}


static inline int syscall_get_arch(void)
{
	if (is_compat_task())
		return AUDIT_ARCH_ARM;

	return AUDIT_ARCH_AARCH64;
}

#endif	
