
#ifndef _UAPI__ASM_SIGCONTEXT_H
#define _UAPI__ASM_SIGCONTEXT_H

#ifdef CONFIG_64BIT
#include <linux/types.h>


struct sigcontext {
	__u64 fault_address;
	
	__u64 regs[31];
	__u64 sp;
	__u64 pc;
	__u64 pstate;
	
	__u8 __reserved[4096] __attribute__((__aligned__(16)));
};


struct _aarch64_ctx {
	__u32 magic;
	__u32 size;
};

#define FPSIMD_MAGIC	0x46508001

struct fpsimd_context {
	struct _aarch64_ctx head;
	__u32 fpsr;
	__u32 fpcr;
	__uint128_t vregs[32];
};


#define ESR_MAGIC	0x45535201

struct esr_context {
	struct _aarch64_ctx head;
	__u64 esr;
};

#else 


struct sigcontext {
	unsigned long trap_no;
	unsigned long error_code;
	unsigned long oldmask;
	unsigned long arm_r0;
	unsigned long arm_r1;
	unsigned long arm_r2;
	unsigned long arm_r3;
	unsigned long arm_r4;
	unsigned long arm_r5;
	unsigned long arm_r6;
	unsigned long arm_r7;
	unsigned long arm_r8;
	unsigned long arm_r9;
	unsigned long arm_r10;
	unsigned long arm_fp;
	unsigned long arm_ip;
	unsigned long arm_sp;
	unsigned long arm_lr;
	unsigned long arm_pc;
	unsigned long arm_cpsr;
	unsigned long fault_address;
};
#endif 

#endif 
