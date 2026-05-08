#ifndef __ASM_LSE_H
#define __ASM_LSE_H

#if defined(CONFIG_AS_LSE) && defined(CONFIG_ARM64_LSE_ATOMICS)

#define __LSE_PREAMBLE	".arch armv8-a+lse\n"

#include <linux/stringify.h>
#include <asm/alternative.h>

#ifdef __ASSEMBLER__

.arch_extension	lse

.macro alt_lse, llsc, lse
	alternative_insn "\llsc", "\lse", ARM64_HAS_LSE_ATOMICS
.endm

#else	

#ifdef CONFIG_LTO_CLANG
#define __LSE_PREAMBLE	".arch armv8-a+lse\n"
#else
__asm__(".arch_extension	lse");
#define __LSE_PREAMBLE
#endif


#define __LL_SC_INLINE
#define __LL_SC_PREFIX(x)	__ll_sc_##x
#define __LL_SC_EXPORT(x)	EXPORT_SYMBOL(__LL_SC_PREFIX(x))


#define __LL_SC_CALL(op)	"bl\t" __stringify(__LL_SC_PREFIX(op)) "\n"
#define __LL_SC_CLOBBERS	"x16", "x17", "x30"


#define ARM64_LSE_ATOMIC_INSN(llsc, lse)				\
	ALTERNATIVE(llsc, __LSE_PREAMBLE lse, ARM64_HAS_LSE_ATOMICS)

#endif	
#else	

#ifdef __ASSEMBLER__

.macro alt_lse, llsc, lse
	\llsc
.endm

#else	

#define __LL_SC_INLINE		static inline
#define __LL_SC_PREFIX(x)	x
#define __LL_SC_EXPORT(x)

#define ARM64_LSE_ATOMIC_INSN(llsc, lse)	llsc

#endif	
#endif	
#endif	
