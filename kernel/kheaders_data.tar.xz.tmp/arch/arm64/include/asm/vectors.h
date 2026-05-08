/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ASM_VECTORS_H
#define __ASM_VECTORS_H

#include <linux/bug.h>
#include <linux/percpu.h>

#include <asm/fixmap.h>
#include <asm/mmu.h>

extern char vectors[];
extern char tramp_vectors[];
extern char __bp_harden_el1_vectors[];


enum arm64_bp_harden_el1_vectors {
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
	
	EL1_VECTOR_BHB_LOOP,

	
	EL1_VECTOR_BHB_FW,

	
	EL1_VECTOR_BHB_CLEAR_INSN,
#endif 

	
	EL1_VECTOR_KPTI,
};

#ifndef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
#define EL1_VECTOR_BHB_LOOP		-1
#define EL1_VECTOR_BHB_FW		-1
#define EL1_VECTOR_BHB_CLEAR_INSN	-1
#endif 


DECLARE_PER_CPU_READ_MOSTLY(const char *, this_cpu_vector);

#ifndef CONFIG_UNMAP_KERNEL_AT_EL0
#define TRAMP_VALIAS	0
#endif

static inline const char *
arm64_get_bp_hardening_vector(enum arm64_bp_harden_el1_vectors slot)
{
	if (arm64_kernel_unmapped_at_el0())
		return (char *)TRAMP_VALIAS + SZ_2K * slot;

	WARN_ON_ONCE(slot == EL1_VECTOR_KPTI);

	return __bp_harden_el1_vectors + SZ_2K * slot;
}

#endif 
