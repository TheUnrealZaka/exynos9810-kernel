

#ifndef __ARM_KGDB_H
#define __ARM_KGDB_H

#include <linux/ptrace.h>
#include <asm/debug-monitors.h>

#ifndef	__ASSEMBLY__

static inline void arch_kgdb_breakpoint(void)
{
	asm ("brk %0" : : "I" (KGDB_COMPILED_DBG_BRK_IMM));
}

extern void kgdb_handle_bus_error(void);
extern int kgdb_fault_expected;

#endif 



#define _GP_REGS		33
#define _FP_REGS		32
#define _EXTRA_REGS		3

#define GP_REG_BYTES		(_GP_REGS * 8)
#define DBG_MAX_REG_NUM		(_GP_REGS + _FP_REGS + _EXTRA_REGS)



#define BUFMAX			2048



#define NUMREGBYTES	((_GP_REGS * 8) + (_FP_REGS * 16) + \
			(_EXTRA_REGS * 4))

#endif 
