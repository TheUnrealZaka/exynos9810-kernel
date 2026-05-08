
#ifndef __ASM_CPU_OPS_H
#define __ASM_CPU_OPS_H

#include <linux/init.h>
#include <linux/threads.h>


struct cpu_operations {
	const char	*name;
	int		(*cpu_init)(unsigned int);
	int		(*cpu_prepare)(unsigned int);
	int		(*cpu_boot)(unsigned int);
	void		(*cpu_postboot)(void);
#ifdef CONFIG_HOTPLUG_CPU
	int		(*cpu_disable)(unsigned int cpu);
	void		(*cpu_die)(unsigned int cpu);
	int		(*cpu_kill)(unsigned int cpu);
#endif
#ifdef CONFIG_CPU_IDLE
	int		(*cpu_init_idle)(unsigned int);
	int		(*cpu_suspend)(unsigned long);
#endif
};

extern const struct cpu_operations *cpu_ops[NR_CPUS];
int __init cpu_read_ops(int cpu);

static inline void __init cpu_read_bootcpu_ops(void)
{
	cpu_read_ops(0);
}

#endif 
