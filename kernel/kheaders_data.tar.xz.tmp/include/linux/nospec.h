// SPDX-License-Identifier: GPL-2.0
// Copyright(c) 2018 Linus Torvalds. All rights reserved.
// Copyright(c) 2018 Alexei Starovoitov. All rights reserved.
// Copyright(c) 2018 Intel Corporation. All rights reserved.

#ifndef _LINUX_NOSPEC_H
#define _LINUX_NOSPEC_H
#include <asm/barrier.h>

struct task_struct;


#ifndef array_index_mask_nospec
static inline unsigned long array_index_mask_nospec(unsigned long index,
						    unsigned long size)
{
	
	OPTIMIZER_HIDE_VAR(index);
	return ~(long)(index | (size - 1UL - index)) >> (BITS_PER_LONG - 1);
}
#endif


#define array_index_nospec(index, size)					\
({									\
	typeof(index) _i = (index);					\
	typeof(size) _s = (size);					\
	unsigned long _mask = array_index_mask_nospec(_i, _s);		\
									\
	BUILD_BUG_ON(sizeof(_i) > sizeof(long));			\
	BUILD_BUG_ON(sizeof(_s) > sizeof(long));			\
									\
	(typeof(_i)) (_i & _mask);					\
})


int arch_prctl_spec_ctrl_get(struct task_struct *task, unsigned long which);
int arch_prctl_spec_ctrl_set(struct task_struct *task, unsigned long which,
			     unsigned long ctrl);

void arch_seccomp_spec_mitigate(struct task_struct *task);

#endif 
