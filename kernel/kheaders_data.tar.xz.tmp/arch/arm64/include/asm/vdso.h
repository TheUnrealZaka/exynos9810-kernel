
#ifndef __ASM_VDSO_H
#define __ASM_VDSO_H

#ifdef __KERNEL__


#define VDSO_LBASE	0x0

#ifndef __ASSEMBLY__

#include <generated/vdso-offsets.h>
#ifdef CONFIG_VDSO32
#include <generated/vdso32-offsets.h>
#endif

#define VDSO_SYMBOL(base, name)						   \
({									   \
	(void *)(vdso_offset_##name - VDSO_LBASE + (unsigned long)(base)); \
})

#endif 

#endif 

#endif 
