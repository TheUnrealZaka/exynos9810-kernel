
#ifndef __ASM_SMP_H
#define __ASM_SMP_H



#define CPU_MMU_OFF		(-1)
#define CPU_BOOT_SUCCESS	(0)

#define CPU_KILL_ME		(1)

#define CPU_STUCK_IN_KERNEL	(2)

#define CPU_PANIC_KERNEL	(3)

#ifndef __ASSEMBLY__

#include <asm/percpu.h>

#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/thread_info.h>

DECLARE_PER_CPU_READ_MOSTLY(int, cpu_number);


#define raw_smp_processor_id() (*raw_cpu_ptr(&cpu_number))

struct seq_file;


extern void show_ipi_list(struct seq_file *p, int prec);


extern void handle_IPI(int ipinr, struct pt_regs *regs);


extern void smp_init_cpus(void);


extern void set_smp_cross_call(void (*)(const struct cpumask *, unsigned int));

extern void (*__smp_cross_call)(const struct cpumask *, unsigned int);


asmlinkage void secondary_start_kernel(void);


struct secondary_data {
	void *stack;
	struct task_struct *task;
	long status;
};

extern struct secondary_data secondary_data;
extern long __early_cpu_boot_status;
extern void secondary_entry(void);

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi_mask(const struct cpumask *mask);

#ifdef CONFIG_ARM64_ACPI_PARKING_PROTOCOL
extern void arch_send_wakeup_ipi_mask(const struct cpumask *mask);
#else
static inline void arch_send_wakeup_ipi_mask(const struct cpumask *mask)
{
	BUILD_BUG();
}
#endif

extern int __cpu_disable(void);

extern void __cpu_die(unsigned int cpu);
extern void cpu_die(void);
extern void cpu_die_early(void);

static inline void cpu_park_loop(void)
{
	for (;;) {
		wfe();
		wfi();
	}
}

static inline void update_cpu_boot_status(int val)
{
	WRITE_ONCE(secondary_data.status, val);
	
	dsb(ishst);
}


static inline void cpu_panic_kernel(void)
{
	update_cpu_boot_status(CPU_PANIC_KERNEL);
	cpu_park_loop();
}


bool cpus_are_stuck_in_kernel(void);

#endif 

#endif 
