
#ifndef _ARM_PROBES_H
#define _ARM_PROBES_H

#include <asm/opcodes.h>

struct kprobe;
struct arch_specific_insn;

typedef u32 kprobe_opcode_t;
typedef void (kprobes_handler_t) (u32 opcode, long addr, struct pt_regs *);


struct arch_specific_insn {
	kprobe_opcode_t *insn;
	pstate_check_t *pstate_cc;
	kprobes_handler_t *handler;
	
	unsigned long restore;
};

#endif
