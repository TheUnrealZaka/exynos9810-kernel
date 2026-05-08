
#ifndef __ASSEMBLY__
#error "Only include this from assembly code"
#endif

#ifndef __ASM_ASSEMBLER_H
#define __ASM_ASSEMBLER_H

#include <asm/asm-offsets.h>
#include <asm/cpufeature.h>
#include <asm/cputype.h>
#include <asm/page.h>
#include <asm/pgtable-hwdef.h>
#include <asm/ptrace.h>
#include <asm/thread_info.h>


	.macro	disable_irq
	msr	daifset, #2
	.endm

	.macro	enable_irq
	msr	daifclr, #2
	.endm

	.macro	save_and_disable_irq, flags
	mrs	\flags, daif
	msr	daifset, #2
	.endm

	.macro	restore_irq, flags
	msr	daif, \flags
	.endm


	.macro	save_and_disable_irqs, olddaif
	mrs	\olddaif, daif
	disable_irq
	.endm

	.macro	restore_irqs, olddaif
	msr	daif, \olddaif
	.endm


	.macro	disable_dbg
	msr	daifset, #8
	.endm

	.macro	enable_dbg
	msr	daifclr, #8
	.endm

	.macro	disable_step_tsk, flgs, tmp
	tbz	\flgs, #TIF_SINGLESTEP, 9990f
	mrs	\tmp, mdscr_el1
	bic	\tmp, \tmp, #1
	msr	mdscr_el1, \tmp
	isb	// Synchronise with enable_dbg
9990:
	.endm

	.macro	enable_step_tsk, flgs, tmp
	tbz	\flgs, #TIF_SINGLESTEP, 9990f
	disable_dbg
	mrs	\tmp, mdscr_el1
	orr	\tmp, \tmp, #1
	msr	mdscr_el1, \tmp
9990:
	.endm


	.macro	enable_dbg_and_irq
	msr	daifclr, #(8 | 2)
	.endm


	.macro	smp_dmb, opt
	dmb	\opt
	.endm


	.macro	csdb
	hint	#20
	.endm


	.macro clearbhb
	hint	#22
	.endm


	.macro	mask_nospec64, idx, limit, tmp
	sub	\tmp, \idx, \limit
	bic	\tmp, \tmp, \idx
	and	\idx, \idx, \tmp, asr #63
	csdb
	.endm


	.macro	nops, num
	.rept	\num
	nop
	.endr
	.endm


	.macro		_asm_extable, from, to
	.pushsection	__ex_table, "a"
	.align		3
	.long		(\from - .), (\to - .)
	.popsection
	.endm

#define USER(l, x...)				\
9999:	x;					\
	_asm_extable	9999b, l


lr	.req	x30		// link register


	 .macro	ventry	label
	.align	7
	b	\label
	.endm


#ifdef CONFIG_CPU_BIG_ENDIAN
#define CPU_BE(code...) code
#else
#define CPU_BE(code...)
#endif


#ifdef CONFIG_CPU_BIG_ENDIAN
#define CPU_LE(code...)
#else
#define CPU_LE(code...) code
#endif


#ifndef CONFIG_CPU_BIG_ENDIAN
	.macro	regs_to_64, rd, lbits, hbits
#else
	.macro	regs_to_64, rd, hbits, lbits
#endif
	orr	\rd, \lbits, \hbits, lsl #32
	.endm


	
	.macro	adr_l, dst, sym
#ifndef MODULE
	adrp	\dst, \sym
	add	\dst, \dst, :lo12:\sym
#else
	movz	\dst, #:abs_g3:\sym
	movk	\dst, #:abs_g2_nc:\sym
	movk	\dst, #:abs_g1_nc:\sym
	movk	\dst, #:abs_g0_nc:\sym
#endif
	.endm

	
	.macro	ldr_l, dst, sym, tmp=
#ifndef MODULE
	.ifb	\tmp
	adrp	\dst, \sym
	ldr	\dst, [\dst, :lo12:\sym]
	.else
	adrp	\tmp, \sym
	ldr	\dst, [\tmp, :lo12:\sym]
	.endif
#else
	.ifb	\tmp
	adr_l	\dst, \sym
	ldr	\dst, [\dst]
	.else
	adr_l	\tmp, \sym
	ldr	\dst, [\tmp]
	.endif
#endif
	.endm

	
	.macro	str_l, src, sym, tmp
#ifndef MODULE
	adrp	\tmp, \sym
	str	\src, [\tmp, :lo12:\sym]
#else
	adr_l	\tmp, \sym
	str	\src, [\tmp]
#endif
	.endm

	
	.macro adr_this_cpu, dst, sym, tmp
	adr_l	\dst, \sym
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	mrs	\tmp, tpidr_el1
alternative_else
	mrs	\tmp, tpidr_el2
alternative_endif
	add	\dst, \dst, \tmp
	.endm

	
	.macro ldr_this_cpu dst, sym, tmp
	adr_l	\dst, \sym
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	mrs	\tmp, tpidr_el1
alternative_else
	mrs	\tmp, tpidr_el2
alternative_endif
	ldr	\dst, [\dst, \tmp]
	.endm


	.macro	vma_vm_mm, rd, rn
	ldr	\rd, [\rn, #VMA_VM_MM]
	.endm


	.macro	mmid, rd, rn
	ldr	\rd, [\rn, #MM_CONTEXT_ID]
	.endm

	.macro	read_ctr, reg
alternative_if_not ARM64_MISMATCHED_CACHE_LINE_SIZE
	mrs	\reg, ctr_el0			// read CTR
	nop
alternative_else
	ldr_l	\reg, arm64_ftr_reg_ctrel0 + ARM64_FTR_SYSVAL
alternative_endif
	.endm



	.macro	raw_dcache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	ubfm	\tmp, \tmp, #16, #19		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
	.endm


	.macro	dcache_line_size, reg, tmp
	read_ctr	\tmp
	ubfm		\tmp, \tmp, #16, #19	// cache line size encoding
	mov		\reg, #4		// bytes per word
	lsl		\reg, \reg, \tmp	// actual cache line size
	.endm


	.macro	raw_icache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	and	\tmp, \tmp, #0xf		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
	.endm


	.macro	icache_line_size, reg, tmp
	read_ctr	\tmp
	and		\tmp, \tmp, #0xf	// cache line size encoding
	mov		\reg, #4		// bytes per word
	lsl		\reg, \reg, \tmp	// actual cache line size
	.endm


	.macro	tcr_set_idmap_t0sz, valreg, tmpreg
#ifndef CONFIG_ARM64_VA_BITS_48
	ldr_l	\tmpreg, idmap_t0sz
	bfi	\valreg, \tmpreg, #TCR_T0SZ_OFFSET, #TCR_TxSZ_WIDTH
#endif
	.endm


	.macro __dcache_op_workaround_clean_cache, op, kaddr
alternative_if_not ARM64_WORKAROUND_CLEAN_CACHE
	dc	\op, \kaddr
alternative_else
	dc	civac, \kaddr
alternative_endif
	.endm

	.macro dcache_by_line_op op, domain, kaddr, size, tmp1, tmp2
	dcache_line_size \tmp1, \tmp2
	add	\size, \kaddr, \size
	sub	\tmp2, \tmp1, #1
	bic	\kaddr, \kaddr, \tmp2
9998:
	.ifc	\op, cvau
	__dcache_op_workaround_clean_cache \op, \kaddr
	.else
	.ifc	\op, cvac
	__dcache_op_workaround_clean_cache \op, \kaddr
	.else
	.ifc	\op, cvap
	sys	3, c7, c12, 1, \kaddr	// dc cvap
	.else
	dc	\op, \kaddr
	.endif
	.endif
	.endif
	add	\kaddr, \kaddr, \tmp1
	cmp	\kaddr, \size
	b.lo	9998b
	dsb	\domain
	.endm


	.macro	reset_pmuserenr_el0, tmpreg
	mrs	\tmpreg, id_aa64dfr0_el1	// Check ID_AA64DFR0_EL1 PMUVer
	sbfx	\tmpreg, \tmpreg, #8, #4
	cmp	\tmpreg, #1			// Skip if no PMU present
	b.lt	9000f
	msr	pmuserenr_el0, xzr		// Disable PMU access from EL0
9000:
	.endm


	.macro copy_page dest:req src:req t1:req t2:req t3:req t4:req t5:req t6:req t7:req t8:req
9998:	ldp	\t1, \t2, [\src]
	ldp	\t3, \t4, [\src, #16]
	ldp	\t5, \t6, [\src, #32]
	ldp	\t7, \t8, [\src, #48]
	add	\src, \src, #64
	stnp	\t1, \t2, [\dest]
	stnp	\t3, \t4, [\dest, #16]
	stnp	\t5, \t6, [\dest, #32]
	stnp	\t7, \t8, [\dest, #48]
	add	\dest, \dest, #64
	tst	\src, #(PAGE_SIZE - 1)
	b.ne	9998b
	.endm


#define ENDPIPROC(x)			\
	.globl	__pi_##x;		\
	.type 	__pi_##x, %function;	\
	.set	__pi_##x, x;		\
	.size	__pi_##x, . - x;	\
	ENDPROC(x)

	
	.macro	le64sym, sym
	.long	\sym\()_lo32
	.long	\sym\()_hi32
	.endm

	
	.macro	mov_q, reg, val
	.if (((\val) >> 31) == 0 || ((\val) >> 31) == 0x1ffffffff)
	movz	\reg, :abs_g1_s:\val
	.else
	.if (((\val) >> 47) == 0 || ((\val) >> 47) == 0x1ffff)
	movz	\reg, :abs_g2_s:\val
	.else
	movz	\reg, :abs_g3:\val
	movk	\reg, :abs_g2_nc:\val
	.endif
	movk	\reg, :abs_g1_nc:\val
	.endif
	movk	\reg, :abs_g0_nc:\val
	.endm


	.macro	get_thread_info, rd
	mrs	\rd, sp_el0
	.endm

	.macro	pte_to_phys, phys, pte
	and	\phys, \pte, #(((1 << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT)
	.endm


	.macro	cpu_midr_match model, rv_min, rv_max, res, tmp1, tmp2, tmp3
	mrs		\res, midr_el1
	mov_q		\tmp1, (MIDR_REVISION_MASK | MIDR_VARIANT_MASK)
	mov_q		\tmp2, MIDR_CPU_MODEL_MASK
	and		\tmp3, \res, \tmp2	// Extract model
	and		\tmp1, \res, \tmp1	// rev & variant
	mov_q		\tmp2, \model
	cmp		\tmp3, \tmp2
	cset		\res, eq
	cbz		\res, .Ldone\@		// Model matches ?

	.if (\rv_min != 0)			// Skip min check if rv_min == 0
	mov_q		\tmp3, \rv_min
	cmp		\tmp1, \tmp3
	cset		\res, ge
	.endif					// \rv_min != 0
	
	.if ((\rv_min != \rv_max) || \rv_min == 0)
	mov_q		\tmp2, \rv_max
	cmp		\tmp1, \tmp2
	cset		\tmp2, le
	and		\res, \res, \tmp2
	.endif
.Ldone\@:
	.endm

	.macro __mitigate_spectre_bhb_loop      tmp
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
alternative_cb  spectre_bhb_patch_loop_iter
	mov	\tmp, #32		// Patched to correct the immediate
alternative_cb_end
.Lspectre_bhb_loop\@:
	b	. + 4
	subs	\tmp, \tmp, #1
	b.ne	.Lspectre_bhb_loop\@
	dsb	nsh
	isb
#endif 
	.endm

	
	.macro __mitigate_spectre_bhb_fw
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
	stp	x0, x1, [sp, #-16]!
	stp	x2, x3, [sp, #-16]!
	mov	w0, #ARM_SMCCC_ARCH_WORKAROUND_3
alternative_cb	arm64_update_smccc_conduit
	nop					// Patched to SMC/HVC #0
alternative_cb_end
	ldp	x2, x3, [sp], #16
	ldp	x0, x1, [sp], #16
#endif 
	.endm
#endif	
