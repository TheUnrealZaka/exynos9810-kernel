
#ifndef __ASM_PROCFNS_H
#define __ASM_PROCFNS_H

#ifdef __KERNEL__
#ifndef __ASSEMBLY__

#include <asm/page.h>

struct mm_struct;
struct cpu_suspend_ctx;

extern void cpu_do_idle(void);
extern void cpu_do_switch_mm(unsigned long pgd_phys, struct mm_struct *mm);
extern void cpu_do_suspend(struct cpu_suspend_ctx *ptr);
extern u64 cpu_do_resume(phys_addr_t ptr, u64 idmap_ttbr);

#include <asm/memory.h>

#endif 
#endif 
#endif 
