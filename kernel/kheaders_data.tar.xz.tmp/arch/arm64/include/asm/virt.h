

#ifndef __ASM__VIRT_H
#define __ASM__VIRT_H




#define HVC_GET_VECTORS 0


#define HVC_SET_VECTORS 1


#define HVC_SOFT_RESTART 2

#define BOOT_CPU_MODE_EL1	(0xe11)
#define BOOT_CPU_MODE_EL2	(0xe12)

#ifndef __ASSEMBLY__

#include <asm/ptrace.h>
#include <asm/sections.h>
#include <asm/sysreg.h>


extern u32 __boot_cpu_mode[2];

void __hyp_set_vectors(phys_addr_t phys_vector_base);
phys_addr_t __hyp_get_vectors(void);


static inline bool is_hyp_mode_available(void)
{
	return (__boot_cpu_mode[0] == BOOT_CPU_MODE_EL2 &&
		__boot_cpu_mode[1] == BOOT_CPU_MODE_EL2);
}


static inline bool is_hyp_mode_mismatched(void)
{
	return __boot_cpu_mode[0] != __boot_cpu_mode[1];
}

static inline bool is_kernel_in_hyp_mode(void)
{
	return read_sysreg(CurrentEL) == CurrentEL_EL2;
}

#ifdef CONFIG_ARM64_VHE
extern void verify_cpu_run_el(void);
#else
static inline void verify_cpu_run_el(void) {}
#endif

#endif 

#endif 
