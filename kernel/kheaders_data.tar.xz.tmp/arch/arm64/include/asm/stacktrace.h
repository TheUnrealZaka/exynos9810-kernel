
#ifndef __ASM_STACKTRACE_H
#define __ASM_STACKTRACE_H

struct task_struct;

struct stackframe {
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	int graph;
#endif
#ifdef CONFIG_SEC_DEBUG_BRANCH_VERIFIER
	unsigned int pc_from_irq;
#endif
};

extern int unwind_frame(struct task_struct *tsk, struct stackframe *frame);
extern void walk_stackframe(struct task_struct *tsk, struct stackframe *frame,
			    int (*fn)(struct stackframe *, void *), void *data);

#endif	
