

#ifndef __ASM_PERF_EVENT_H
#define __ASM_PERF_EVENT_H

#include <asm/stack_pointer.h>

#define	ARMV8_PMU_MAX_COUNTERS	32
#define	ARMV8_PMU_COUNTER_MASK	(ARMV8_PMU_MAX_COUNTERS - 1)


#define ARMV8_PMU_PMCR_E	(1 << 0) 
#define ARMV8_PMU_PMCR_P	(1 << 1) 
#define ARMV8_PMU_PMCR_C	(1 << 2) 
#define ARMV8_PMU_PMCR_D	(1 << 3) 
#define ARMV8_PMU_PMCR_X	(1 << 4) 
#define ARMV8_PMU_PMCR_DP	(1 << 5) 
#define ARMV8_PMU_PMCR_LC	(1 << 6) 
#define	ARMV8_PMU_PMCR_N_SHIFT	11	 
#define	ARMV8_PMU_PMCR_N_MASK	0x1f
#define	ARMV8_PMU_PMCR_MASK	0x7f	 


#define	ARMV8_PMU_OVSR_MASK		0xffffffff	
#define	ARMV8_PMU_OVERFLOWED_MASK	ARMV8_PMU_OVSR_MASK


#define	ARMV8_PMU_EVTYPE_MASK	0xc800ffff	
#define	ARMV8_PMU_EVTYPE_EVENT	0xffff		


#define ARMV8_PMUV3_PERFCTR_SW_INCR				0x00
#define ARMV8_PMUV3_PERFCTR_L1D_CACHE_REFILL			0x03
#define ARMV8_PMUV3_PERFCTR_L1D_CACHE				0x04
#define ARMV8_PMUV3_PERFCTR_BR_MIS_PRED				0x10
#define ARMV8_PMUV3_PERFCTR_CPU_CYCLES				0x11
#define ARMV8_PMUV3_PERFCTR_BR_PRED				0x12


#define	ARMV8_PMU_EXCLUDE_EL1	(1 << 31)
#define	ARMV8_PMU_EXCLUDE_EL0	(1 << 30)
#define	ARMV8_PMU_INCLUDE_EL2	(1 << 27)


#define ARMV8_PMU_USERENR_MASK	0xf		
#define ARMV8_PMU_USERENR_EN	(1 << 0) 
#define ARMV8_PMU_USERENR_SW	(1 << 1) 
#define ARMV8_PMU_USERENR_CR	(1 << 2) 
#define ARMV8_PMU_USERENR_ER	(1 << 3) 

#ifdef CONFIG_PERF_EVENTS
struct pt_regs;
extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
extern unsigned long perf_misc_flags(struct pt_regs *regs);
#define perf_misc_flags(regs)	perf_misc_flags(regs)
#endif

#define perf_arch_fetch_caller_regs(regs, __ip) { \
	(regs)->pc = (__ip);    \
	(regs)->regs[29] = (unsigned long) __builtin_frame_address(0); \
	(regs)->sp = current_stack_pointer; \
	(regs)->pstate = PSR_MODE_EL1h;	\
}

#endif
