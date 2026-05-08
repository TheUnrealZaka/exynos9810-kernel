#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__


#define TSK_PID 1336 
#define TSK_ACTIVE_MM 1168 

#define TSK_TI_FLAGS 0 
#define TSK_TI_PREEMPT 16 
#define TSK_TI_ADDR_LIMIT 8 
#define TSK_STACK 32 

#define THREAD_CPU_CONTEXT 2656 

#define S_X0 0 
#define S_X1 8 
#define S_X2 16 
#define S_X3 24 
#define S_X4 32 
#define S_X5 40 
#define S_X6 48 
#define S_X7 56 
#define S_X8 64 
#define S_X10 80 
#define S_X12 96 
#define S_X14 112 
#define S_X16 128 
#define S_X18 144 
#define S_X20 160 
#define S_X22 176 
#define S_X24 192 
#define S_X26 208 
#define S_X28 224 
#define S_LR 240 
#define S_SP 248 
#define S_COMPAT_SP 104 
#define S_PSTATE 264 
#define S_PC 256 
#define S_ORIG_X0 272 
#define S_SYSCALLNO 280 
#define S_ORIG_ADDR_LIMIT 288 
#define S_FRAME_SIZE 304 

#define COMPAT_SIGFRAME_REGS_OFFSET 32 
#define COMPAT_RT_SIGFRAME_REGS_OFFSET 160 

#define MM_CONTEXT_ID 720 

#define VMA_VM_MM 64 
#define VMA_VM_FLAGS 80 

#define VM_EXEC 4 

#define PRESERVE_STACK_SIZE 256 
#define PAGE_SZ 4096 

#define DMA_BIDIRECTIONAL 0 
#define DMA_TO_DEVICE 1 
#define DMA_FROM_DEVICE 2 

#define CPU_BOOT_STACK 0 
#define CPU_BOOT_TASK 8 

#define CPU_SUSPEND_SZ 112 
#define CPU_CTX_SP 96 
#define MPIDR_HASH_MASK 0 
#define MPIDR_HASH_SHIFTS 8 
#define SLEEP_STACK_DATA_SYSTEM_REGS 0 
#define SLEEP_STACK_DATA_CALLEE_REGS 112 
#define ARM_SMCCC_RES_X0_OFFS 0 
#define ARM_SMCCC_RES_X2_OFFS 16 
#define ARM_SMCCC_QUIRK_ID_OFFS 0 
#define ARM_SMCCC_QUIRK_STATE_OFFS 8 

#define HIBERN_PBE_ORIG 8 
#define HIBERN_PBE_ADDR 0 
#define HIBERN_PBE_NEXT 16 
#define ARM64_FTR_SYSVAL 16 


#endif
