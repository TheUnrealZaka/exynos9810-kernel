
#ifndef __ASM_GENERIC_SWITCH_TO_H
#define __ASM_GENERIC_SWITCH_TO_H

#include <linux/thread_info.h>


extern struct task_struct *__switch_to(struct task_struct *,
				       struct task_struct *);

#define switch_to(prev, next, last)					\
	do {								\
		((last) = __switch_to((prev), (next)));			\
	} while (0)

#endif 
