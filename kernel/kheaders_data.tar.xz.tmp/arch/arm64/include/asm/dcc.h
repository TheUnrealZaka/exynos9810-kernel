
#ifndef __ASM_DCC_H
#define __ASM_DCC_H

#include <asm/barrier.h>
#include <asm/sysreg.h>

static inline u32 __dcc_getstatus(void)
{
	return read_sysreg(mdccsr_el0);
}

static inline char __dcc_getchar(void)
{
	char c = read_sysreg(dbgdtrrx_el0);
	isb();

	return c;
}

static inline void __dcc_putchar(char c)
{
	
	write_sysreg((unsigned char)c, dbgdtrtx_el0);
	isb();
}

#endif
