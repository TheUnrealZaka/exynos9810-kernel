

#ifndef _LINUX_TRACEHOOK_H
#define _LINUX_TRACEHOOK_H	1

#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/security.h>
#include <linux/task_work.h>
#include <linux/memcontrol.h>
struct linux_binprm;


static inline int ptrace_report_syscall(struct pt_regs *regs)
{
	int ptrace = current->ptrace;

	if (!(ptrace & PT_PTRACED))
		return 0;

	ptrace_notify(SIGTRAP | ((ptrace & PT_TRACESYSGOOD) ? 0x80 : 0));

	
	if (current->exit_code) {
		send_sig(current->exit_code, current, 1);
		current->exit_code = 0;
	}

	return fatal_signal_pending(current);
}


static inline __must_check int tracehook_report_syscall_entry(
	struct pt_regs *regs)
{
	return ptrace_report_syscall(regs);
}


static inline void tracehook_report_syscall_exit(struct pt_regs *regs, int step)
{
	if (step) {
		siginfo_t info;
		user_single_step_siginfo(current, regs, &info);
		force_sig_info(SIGTRAP, &info, current);
		return;
	}

	ptrace_report_syscall(regs);
}


static inline void tracehook_signal_handler(int stepping)
{
	if (stepping)
		ptrace_notify(SIGTRAP);
}


static inline void set_notify_resume(struct task_struct *task)
{
#ifdef TIF_NOTIFY_RESUME
	if (!test_and_set_tsk_thread_flag(task, TIF_NOTIFY_RESUME))
		kick_process(task);
#endif
}


static inline void tracehook_notify_resume(struct pt_regs *regs)
{
	
	smp_mb__after_atomic();
	if (unlikely(current->task_works))
		task_work_run();

	mem_cgroup_handle_over_high();
}

#endif	
