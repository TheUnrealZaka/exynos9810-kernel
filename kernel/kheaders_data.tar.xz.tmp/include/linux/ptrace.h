#ifndef _LINUX_PTRACE_H
#define _LINUX_PTRACE_H

#include <linux/compiler.h>		
#include <linux/sched.h>		
#include <linux/err.h>			
#include <linux/bug.h>			
#include <linux/pid_namespace.h>	
#include <uapi/linux/ptrace.h>

extern int ptrace_access_vm(struct task_struct *tsk, unsigned long addr,
			    void *buf, int len, unsigned int gup_flags);



#define PT_SEIZED	0x00010000	
#define PT_PTRACED	0x00000001
#define PT_DTRACE	0x00000002	

#define PT_OPT_FLAG_SHIFT	3

#define PT_EVENT_FLAG(event)	(1 << (PT_OPT_FLAG_SHIFT + (event)))
#define PT_TRACESYSGOOD		PT_EVENT_FLAG(0)
#define PT_TRACE_FORK		PT_EVENT_FLAG(PTRACE_EVENT_FORK)
#define PT_TRACE_VFORK		PT_EVENT_FLAG(PTRACE_EVENT_VFORK)
#define PT_TRACE_CLONE		PT_EVENT_FLAG(PTRACE_EVENT_CLONE)
#define PT_TRACE_EXEC		PT_EVENT_FLAG(PTRACE_EVENT_EXEC)
#define PT_TRACE_VFORK_DONE	PT_EVENT_FLAG(PTRACE_EVENT_VFORK_DONE)
#define PT_TRACE_EXIT		PT_EVENT_FLAG(PTRACE_EVENT_EXIT)
#define PT_TRACE_SECCOMP	PT_EVENT_FLAG(PTRACE_EVENT_SECCOMP)

#define PT_EXITKILL		(PTRACE_O_EXITKILL << PT_OPT_FLAG_SHIFT)
#define PT_SUSPEND_SECCOMP	(PTRACE_O_SUSPEND_SECCOMP << PT_OPT_FLAG_SHIFT)

extern long arch_ptrace(struct task_struct *child, long request,
			unsigned long addr, unsigned long data);
extern int ptrace_readdata(struct task_struct *tsk, unsigned long src, char __user *dst, int len);
extern int ptrace_writedata(struct task_struct *tsk, char __user *src, unsigned long dst, int len);
extern void ptrace_disable(struct task_struct *);
extern int ptrace_request(struct task_struct *child, long request,
			  unsigned long addr, unsigned long data);
extern void ptrace_notify(int exit_code);
extern void __ptrace_link(struct task_struct *child,
			  struct task_struct *new_parent,
			  const struct cred *ptracer_cred);
extern void __ptrace_unlink(struct task_struct *child);
extern void exit_ptrace(struct task_struct *tracer, struct list_head *dead);
#define PTRACE_MODE_READ	0x01
#define PTRACE_MODE_ATTACH	0x02
#define PTRACE_MODE_NOAUDIT	0x04
#define PTRACE_MODE_FSCREDS	0x08
#define PTRACE_MODE_REALCREDS	0x10
#define PTRACE_MODE_SCHED	0x20
#define PTRACE_MODE_IBPB	0x40


#define PTRACE_MODE_READ_FSCREDS (PTRACE_MODE_READ | PTRACE_MODE_FSCREDS)
#define PTRACE_MODE_READ_REALCREDS (PTRACE_MODE_READ | PTRACE_MODE_REALCREDS)
#define PTRACE_MODE_ATTACH_FSCREDS (PTRACE_MODE_ATTACH | PTRACE_MODE_FSCREDS)
#define PTRACE_MODE_ATTACH_REALCREDS (PTRACE_MODE_ATTACH | PTRACE_MODE_REALCREDS)
#define PTRACE_MODE_SPEC_IBPB (PTRACE_MODE_ATTACH_REALCREDS | PTRACE_MODE_IBPB)


extern bool ptrace_may_access(struct task_struct *task, unsigned int mode);


extern bool ptrace_may_access_sched(struct task_struct *task, unsigned int mode);

static inline int ptrace_reparented(struct task_struct *child)
{
	return !same_thread_group(child->real_parent, child->parent);
}

static inline void ptrace_unlink(struct task_struct *child)
{
	if (unlikely(child->ptrace))
		__ptrace_unlink(child);
}

int generic_ptrace_peekdata(struct task_struct *tsk, unsigned long addr,
			    unsigned long data);
int generic_ptrace_pokedata(struct task_struct *tsk, unsigned long addr,
			    unsigned long data);


static inline struct task_struct *ptrace_parent(struct task_struct *task)
{
	if (unlikely(task->ptrace))
		return rcu_dereference(task->parent);
	return NULL;
}


static inline bool ptrace_event_enabled(struct task_struct *task, int event)
{
	return task->ptrace & PT_EVENT_FLAG(event);
}


static inline void ptrace_event(int event, unsigned long message)
{
	if (unlikely(ptrace_event_enabled(current, event))) {
		current->ptrace_message = message;
		ptrace_notify((event << 8) | SIGTRAP);
	} else if (event == PTRACE_EVENT_EXEC) {
		
		if ((current->ptrace & (PT_PTRACED|PT_SEIZED)) == PT_PTRACED)
			send_sig(SIGTRAP, current, 0);
	}
}


static inline void ptrace_event_pid(int event, struct pid *pid)
{
	
	unsigned long message = 0;
	struct pid_namespace *ns;

	rcu_read_lock();
	ns = task_active_pid_ns(rcu_dereference(current->parent));
	if (ns)
		message = pid_nr_ns(pid, ns);
	rcu_read_unlock();

	ptrace_event(event, message);
}


static inline void ptrace_init_task(struct task_struct *child, bool ptrace)
{
	INIT_LIST_HEAD(&child->ptrace_entry);
	INIT_LIST_HEAD(&child->ptraced);
	child->jobctl = 0;
	child->ptrace = 0;
	child->parent = child->real_parent;

	if (unlikely(ptrace) && current->ptrace) {
		child->ptrace = current->ptrace;
		__ptrace_link(child, current->parent, current->ptracer_cred);

		if (child->ptrace & PT_SEIZED)
			task_set_jobctl_pending(child, JOBCTL_TRAP_STOP);
		else
			sigaddset(&child->pending.signal, SIGSTOP);

		set_tsk_thread_flag(child, TIF_SIGPENDING);
	}
	else
		child->ptracer_cred = NULL;
}


static inline void ptrace_release_task(struct task_struct *task)
{
	BUG_ON(!list_empty(&task->ptraced));
	ptrace_unlink(task);
	BUG_ON(!list_empty(&task->ptrace_entry));
}

#ifndef force_successful_syscall_return

#define force_successful_syscall_return() do { } while (0)
#endif

#ifndef is_syscall_success

#define is_syscall_success(regs) (!IS_ERR_VALUE((unsigned long)(regs_return_value(regs))))
#endif



#ifndef arch_has_single_step

#define arch_has_single_step()		(0)


static inline void user_enable_single_step(struct task_struct *task)
{
	BUG();			
}


static inline void user_disable_single_step(struct task_struct *task)
{
}
#else
extern void user_enable_single_step(struct task_struct *);
extern void user_disable_single_step(struct task_struct *);
#endif	

#ifndef arch_has_block_step

#define arch_has_block_step()		(0)


static inline void user_enable_block_step(struct task_struct *task)
{
	BUG();			
}
#else
extern void user_enable_block_step(struct task_struct *);
#endif	

#ifdef ARCH_HAS_USER_SINGLE_STEP_INFO
extern void user_single_step_siginfo(struct task_struct *tsk,
				struct pt_regs *regs, siginfo_t *info);
#else
static inline void user_single_step_siginfo(struct task_struct *tsk,
				struct pt_regs *regs, siginfo_t *info)
{
	memset(info, 0, sizeof(*info));
	info->si_signo = SIGTRAP;
}
#endif

#ifndef arch_ptrace_stop_needed

#define arch_ptrace_stop_needed(code, info)	(0)
#endif

#ifndef arch_ptrace_stop

#define arch_ptrace_stop(code, info)		do { } while (0)
#endif

#ifndef current_pt_regs
#define current_pt_regs() task_pt_regs(current)
#endif

#ifndef ptrace_signal_deliver
#define ptrace_signal_deliver() ((void)0)
#endif


#ifndef signal_pt_regs
#define signal_pt_regs() task_pt_regs(current)
#endif

#ifndef current_user_stack_pointer
#define current_user_stack_pointer() user_stack_pointer(current_pt_regs())
#endif

extern int task_current_syscall(struct task_struct *target, long *callno,
				unsigned long args[6], unsigned int maxargs,
				unsigned long *sp, unsigned long *pc);

#endif
