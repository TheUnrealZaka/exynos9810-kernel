
#ifndef __ASM_FP_H
#define __ASM_FP_H

#include <asm/ptrace.h>

#ifndef __ASSEMBLY__


struct fpsimd_state {
	union {
		struct user_fpsimd_state user_fpsimd;
		struct {
			__uint128_t vregs[32];
			u32 fpsr;
			u32 fpcr;
		};
	};
	
	unsigned int cpu;
};

struct fpsimd_kernel_state {
	__uint128_t vregs[32];
	u32 fpsr;
	u32 fpcr;
	unsigned int cpu;
	
	atomic_t depth;
};


struct fpsimd_partial_state {
	u32		fpsr;
	u32		fpcr;
	u32		num_regs;
	__uint128_t	vregs[32];
};


#if defined(__KERNEL__) && defined(CONFIG_COMPAT)

#define VFP_FPSCR_STAT_MASK	0xf800009f
#define VFP_FPSCR_CTRL_MASK	0x07f79f00

#define VFP_STATE_SIZE		((32 * 8) + 4)
#endif

struct task_struct;

extern void fpsimd_save_state(struct fpsimd_state *state);
extern void fpsimd_load_state(struct fpsimd_state *state);

extern void fpsimd_thread_switch(struct task_struct *next);
extern void fpsimd_flush_thread(void);

extern void fpsimd_preserve_current_state(void);
extern void fpsimd_restore_current_state(void);
extern void fpsimd_update_current_state(struct fpsimd_state *state);

extern void fpsimd_flush_task_state(struct task_struct *target);

extern void fpsimd_save_partial_state(struct fpsimd_partial_state *state,
				      u32 num_regs);
extern void fpsimd_load_partial_state(struct fpsimd_partial_state *state);

void fpsimd_set_task_using(struct task_struct *t);
void fpsimd_clr_task_using(struct task_struct *t);

void fpsimd_get(void);
void fpsimd_put(void);

#endif

#endif
