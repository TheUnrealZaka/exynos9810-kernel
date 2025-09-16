
#ifndef __ASM_EXEC_H
#define __ASM_EXEC_H

#include <linux/sched.h>

extern unsigned long arch_align_stack(unsigned long sp);
void uao_thread_switch(struct task_struct *next);

#endif	
