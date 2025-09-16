
#ifndef __ASM_MMU_CONTEXT_H
#define __ASM_MMU_CONTEXT_H

#include <linux/compiler.h>
#include <linux/sched.h>

#include <asm/cacheflush.h>
#include <asm/cpufeature.h>
#include <asm/proc-fns.h>
#include <asm-generic/mm_hooks.h>
#include <asm/cputype.h>
#include <asm/pgtable.h>
#include <asm/sysreg.h>
#include <asm/tlbflush.h>

static inline void contextidr_thread_switch(struct task_struct *next)
{
	if (!IS_ENABLED(CONFIG_PID_IN_CONTEXTIDR))
		return;

	write_sysreg(task_pid_nr(next), contextidr_el1);
	isb();
}


static inline void cpu_set_reserved_ttbr0(void)
{
	unsigned long ttbr = __pa_symbol(empty_zero_page);

	write_sysreg(ttbr, ttbr0_el1);
	isb();
}

static inline void cpu_switch_mm(pgd_t *pgd, struct mm_struct *mm)
{
	BUG_ON(pgd == swapper_pg_dir);
	cpu_set_reserved_ttbr0();
	cpu_do_switch_mm(virt_to_phys(pgd),mm);
}


extern u64 idmap_t0sz;

static inline bool __cpu_uses_extended_idmap(void)
{
	return (!IS_ENABLED(CONFIG_ARM64_VA_BITS_48) &&
		unlikely(idmap_t0sz != TCR_T0SZ(VA_BITS)));
}


static inline void __cpu_set_tcr_t0sz(unsigned long t0sz)
{
	unsigned long tcr;

	if (!__cpu_uses_extended_idmap())
		return;

	tcr = read_sysreg(tcr_el1);
	tcr &= ~TCR_T0SZ_MASK;
	tcr |= t0sz << TCR_T0SZ_OFFSET;
	write_sysreg(tcr, tcr_el1);
	isb();
}

#define cpu_set_default_tcr_t0sz()	__cpu_set_tcr_t0sz(TCR_T0SZ(VA_BITS))
#define cpu_set_idmap_tcr_t0sz()	__cpu_set_tcr_t0sz(idmap_t0sz)


static inline void cpu_uninstall_idmap(void)
{
	struct mm_struct *mm = current->active_mm;

	cpu_set_reserved_ttbr0();
	local_flush_tlb_all();
	cpu_set_default_tcr_t0sz();

	if (mm != &init_mm && !system_uses_ttbr0_pan())
		cpu_switch_mm(mm->pgd, mm);
}

static inline void cpu_install_idmap(void)
{
	cpu_set_reserved_ttbr0();
	local_flush_tlb_all();
	cpu_set_idmap_tcr_t0sz();

	cpu_switch_mm(lm_alias(idmap_pg_dir), &init_mm);
}


static inline void __nocfi cpu_replace_ttbr1(pgd_t *pgd)
{
	typedef void (ttbr_replace_func)(phys_addr_t);
	extern ttbr_replace_func idmap_cpu_replace_ttbr1;
	ttbr_replace_func *replace_phys;

	phys_addr_t pgd_phys = virt_to_phys(pgd);

	replace_phys = (void *)__pa_symbol(idmap_cpu_replace_ttbr1);

	cpu_install_idmap();
	replace_phys(pgd_phys);
	cpu_uninstall_idmap();
}


#define destroy_context(mm)		do { } while(0)
void check_and_switch_context(struct mm_struct *mm, unsigned int cpu);

#define init_new_context(tsk,mm)	({ atomic64_set(&(mm)->context.id, 0); 0; })

#ifdef CONFIG_ARM64_SW_TTBR0_PAN
static inline void update_saved_ttbr0(struct task_struct *tsk,
				      struct mm_struct *mm)
{
	u64 ttbr;

	if (!system_uses_ttbr0_pan())
		return;

	if (mm == &init_mm)
		ttbr = __pa_symbol(empty_zero_page);
	else
		ttbr = virt_to_phys(mm->pgd) | ASID(mm) << 48;

	WRITE_ONCE(task_thread_info(tsk)->ttbr0, ttbr);
}
#else
static inline void update_saved_ttbr0(struct task_struct *tsk,
				      struct mm_struct *mm)
{
}
#endif

static inline void
enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
	
	update_saved_ttbr0(tsk, &init_mm);
}

static inline void __switch_mm(struct mm_struct *next)
{
	unsigned int cpu = smp_processor_id();

	
	if (next == &init_mm) {
		cpu_set_reserved_ttbr0();
		return;
	}

	check_and_switch_context(next, cpu);
}

static inline void
switch_mm(struct mm_struct *prev, struct mm_struct *next,
	  struct task_struct *tsk)
{
	if (prev != next)
		__switch_mm(next);

	
	update_saved_ttbr0(tsk, next);
}

#define deactivate_mm(tsk,mm)	do { } while (0)
#define activate_mm(prev,next)	switch_mm(prev, next, current)

void verify_cpu_asid_bits(void);
void post_ttbr_update_workaround(void);

#endif
