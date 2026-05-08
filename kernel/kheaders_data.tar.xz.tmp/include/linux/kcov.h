#ifndef _LINUX_KCOV_H
#define _LINUX_KCOV_H

#include <uapi/linux/kcov.h>

struct task_struct;

#ifdef CONFIG_KCOV

enum kcov_mode {
	
	KCOV_MODE_DISABLED = 0,
	
	KCOV_MODE_INIT = 1,
	
	KCOV_MODE_TRACE_PC = 2,
	
	KCOV_MODE_TRACE_CMP = 3,
};

void kcov_task_init(struct task_struct *t);
void kcov_task_exit(struct task_struct *t);

#else

static inline void kcov_task_init(struct task_struct *t) {}
static inline void kcov_task_exit(struct task_struct *t) {}

#endif 
#endif 
