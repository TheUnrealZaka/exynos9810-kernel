#ifndef _LINUX_KAISER_H
#define _LINUX_KAISER_H

#ifdef CONFIG_PAGE_TABLE_ISOLATION
#include <asm/kaiser.h>

static inline int kaiser_map_thread_stack(void *stack)
{
	
	return kaiser_add_mapping((unsigned long)stack +
			THREAD_SIZE - PAGE_SIZE, PAGE_SIZE, __PAGE_KERNEL);
}

static inline void kaiser_unmap_thread_stack(void *stack)
{
	
	kaiser_remove_mapping((unsigned long)stack +
			THREAD_SIZE - PAGE_SIZE, PAGE_SIZE);
}
#else



static inline void kaiser_init(void)
{
}
static inline int kaiser_add_mapping(unsigned long addr,
				     unsigned long size, u64 flags)
{
	return 0;
}
static inline void kaiser_remove_mapping(unsigned long start,
					 unsigned long size)
{
}
static inline int kaiser_map_thread_stack(void *stack)
{
	return 0;
}
static inline void kaiser_unmap_thread_stack(void *stack)
{
}

#endif 
#endif 
