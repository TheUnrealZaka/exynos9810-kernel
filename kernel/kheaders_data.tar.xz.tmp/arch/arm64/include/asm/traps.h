
#ifndef __ASM_TRAP_H
#define __ASM_TRAP_H

#include <linux/list.h>
#include <asm/sections.h>

struct pt_regs;

struct undef_hook {
	struct list_head node;
	u32 instr_mask;
	u32 instr_val;
	u64 pstate_mask;
	u64 pstate_val;
	int (*fn)(struct pt_regs *regs, u32 instr);
};

void register_undef_hook(struct undef_hook *hook);
void unregister_undef_hook(struct undef_hook *hook);

void arm64_notify_segfault(struct pt_regs *regs, unsigned long addr);

static inline int __in_irqentry_text(unsigned long ptr)
{
	return ptr >= (unsigned long)&__irqentry_text_start &&
	       ptr < (unsigned long)&__irqentry_text_end;
}

static inline int in_exception_text(unsigned long ptr)
{
	int in;

	in = ptr >= (unsigned long)&__exception_text_start &&
	     ptr < (unsigned long)&__exception_text_end;

	return in ? : __in_irqentry_text(ptr);
}

#endif
