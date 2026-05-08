#ifndef _ASM_GENERIC_SECTIONS_H_
#define _ASM_GENERIC_SECTIONS_H_



#include <linux/compiler.h>
#include <linux/types.h>


extern char _text[], _stext[], _etext[];
extern char _data[], _sdata[], _edata[];
extern char __bss_start[], __bss_stop[];
extern char __init_begin[], __init_end[];
extern char _sinittext[], _einittext[];
extern char __start_data_ro_after_init[], __end_data_ro_after_init[];
extern char _end[];
extern char __per_cpu_load[], __per_cpu_start[], __per_cpu_end[];
extern char __kprobes_text_start[], __kprobes_text_end[];
extern char __entry_text_start[], __entry_text_end[];
extern char __start_rodata[], __end_rodata[];
extern char __irqentry_text_start[], __irqentry_text_end[];
extern char __softirqentry_text_start[], __softirqentry_text_end[];
#ifdef	CONFIG_RKP_KDP
extern char __rkp_ro_start[], __rkp_ro_end[];
#endif 


extern char __ctors_start[], __ctors_end[];

extern __visible const void __nosave_begin, __nosave_end;


#ifndef dereference_function_descriptor
#define dereference_function_descriptor(p) (p)
#endif


#ifndef arch_is_kernel_text
static inline int arch_is_kernel_text(unsigned long addr)
{
	return 0;
}
#endif

#ifndef arch_is_kernel_data
static inline int arch_is_kernel_data(unsigned long addr)
{
	return 0;
}
#endif


static inline bool memory_contains(void *begin, void *end, void *virt,
				   size_t size)
{
	return virt >= begin && virt + size <= end;
}


static inline bool memory_intersects(void *begin, void *end, void *virt,
				     size_t size)
{
	void *vend = virt + size;

	if (virt < end && vend > begin)
		return true;

	return false;
}


static inline bool init_section_contains(void *virt, size_t size)
{
	return memory_contains(__init_begin, __init_end, virt, size);
}


static inline bool init_section_intersects(void *virt, size_t size)
{
	return memory_intersects(__init_begin, __init_end, virt, size);
}

#endif 
