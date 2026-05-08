/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_GPU_REGMAP_H_
#define _UAPI_KBASE_GPU_REGMAP_H_

#include "mali_kbase_gpu_coherency.h"
#include "mali_kbase_gpu_id.h"
#if MALI_USE_CSF
#include "backend/mali_kbase_gpu_regmap_csf.h"
#else
#include "backend/mali_kbase_gpu_regmap_jm.h"
#endif




#define GPU_CONTROL_BASE        0x0000
#define GPU_CONTROL_REG(r)      (GPU_CONTROL_BASE + (r))
#define GPU_ID                  0x000   
#define L2_FEATURES             0x004   
#define TILER_FEATURES          0x00C   
#define MEM_FEATURES            0x010   
#define MMU_FEATURES            0x014   
#define AS_PRESENT              0x018   
#define GPU_IRQ_RAWSTAT         0x020   
#define GPU_IRQ_CLEAR           0x024   
#define GPU_IRQ_MASK            0x028   
#define GPU_IRQ_STATUS          0x02C   

#define GPU_COMMAND             0x030   
#define GPU_STATUS              0x034   

#define GPU_DBGEN               (1 << 8)    

#define GPU_FAULTSTATUS         0x03C   
#define GPU_FAULTADDRESS_LO     0x040   
#define GPU_FAULTADDRESS_HI     0x044   

#define L2_CONFIG               0x048   

#define GROUPS_L2_COHERENT      (1 << 0) 
#define SUPER_L2_COHERENT       (1 << 1) 

#define PWR_KEY                 0x050   
#define PWR_OVERRIDE0           0x054   
#define PWR_OVERRIDE1           0x058   
#define GPU_FEATURES_LO         0x060   
#define GPU_FEATURES_HI         0x064   
#define PRFCNT_FEATURES         0x068   
#define TIMESTAMP_OFFSET_LO     0x088   
#define TIMESTAMP_OFFSET_HI     0x08C   
#define CYCLE_COUNT_LO          0x090   
#define CYCLE_COUNT_HI          0x094   
#define TIMESTAMP_LO            0x098   
#define TIMESTAMP_HI            0x09C   

#define THREAD_MAX_THREADS      0x0A0   
#define THREAD_MAX_WORKGROUP_SIZE 0x0A4 
#define THREAD_MAX_BARRIER_SIZE 0x0A8   
#define THREAD_FEATURES         0x0AC   
#define THREAD_TLS_ALLOC        0x310   

#define TEXTURE_FEATURES_0      0x0B0   
#define TEXTURE_FEATURES_1      0x0B4   
#define TEXTURE_FEATURES_2      0x0B8   
#define TEXTURE_FEATURES_3      0x0BC   

#define TEXTURE_FEATURES_REG(n) GPU_CONTROL_REG(TEXTURE_FEATURES_0 + ((n) << 2))

#define SHADER_PRESENT_LO       0x100   
#define SHADER_PRESENT_HI       0x104   

#define TILER_PRESENT_LO        0x110   
#define TILER_PRESENT_HI        0x114   

#define L2_PRESENT_LO           0x120   
#define L2_PRESENT_HI           0x124   

#define STACK_PRESENT_LO        0xE00   
#define STACK_PRESENT_HI        0xE04   

#define SHADER_READY_LO         0x140   
#define SHADER_READY_HI         0x144   

#define TILER_READY_LO          0x150   
#define TILER_READY_HI          0x154   

#define L2_READY_LO             0x160   
#define L2_READY_HI             0x164   

#define STACK_READY_LO          0xE10   
#define STACK_READY_HI          0xE14   

#define SHADER_PWRON_LO         0x180   
#define SHADER_PWRON_HI         0x184   

#define TILER_PWRON_LO          0x190   
#define TILER_PWRON_HI          0x194   

#define L2_PWRON_LO             0x1A0   
#define L2_PWRON_HI             0x1A4   

#define STACK_PWRON_LO          0xE20   
#define STACK_PWRON_HI          0xE24   

#define SHADER_PWROFF_LO        0x1C0   
#define SHADER_PWROFF_HI        0x1C4   

#define TILER_PWROFF_LO         0x1D0   
#define TILER_PWROFF_HI         0x1D4   

#define L2_PWROFF_LO            0x1E0   
#define L2_PWROFF_HI            0x1E4   

#define STACK_PWROFF_LO         0xE30   
#define STACK_PWROFF_HI         0xE34   

#define SHADER_PWRTRANS_LO      0x200   
#define SHADER_PWRTRANS_HI      0x204   

#define TILER_PWRTRANS_LO       0x210   
#define TILER_PWRTRANS_HI       0x214   

#define L2_PWRTRANS_LO          0x220   
#define L2_PWRTRANS_HI          0x224   

#define ASN_HASH_0              0x02C0 
#define ASN_HASH(n)             (ASN_HASH_0 + (n)*4)
#define ASN_HASH_COUNT          3

#define STACK_PWRTRANS_LO       0xE40   
#define STACK_PWRTRANS_HI       0xE44   

#define SHADER_PWRACTIVE_LO     0x240   
#define SHADER_PWRACTIVE_HI     0x244   

#define TILER_PWRACTIVE_LO      0x250   
#define TILER_PWRACTIVE_HI      0x254   

#define L2_PWRACTIVE_LO         0x260   
#define L2_PWRACTIVE_HI         0x264   

#define COHERENCY_FEATURES      0x300   
#define COHERENCY_ENABLE        0x304   

#define SHADER_CONFIG           0xF04   
#define TILER_CONFIG            0xF08   
#define L2_MMU_CONFIG           0xF0C   



#define JOB_CONTROL_BASE        0x1000

#define JOB_CONTROL_REG(r)      (JOB_CONTROL_BASE + (r))

#define JOB_IRQ_RAWSTAT         0x000   
#define JOB_IRQ_CLEAR           0x004   
#define JOB_IRQ_MASK            0x008   
#define JOB_IRQ_STATUS          0x00C   



#define MEMORY_MANAGEMENT_BASE  0x2000
#define MMU_REG(r)              (MEMORY_MANAGEMENT_BASE + (r))

#define MMU_IRQ_RAWSTAT         0x000   
#define MMU_IRQ_CLEAR           0x004   
#define MMU_IRQ_MASK            0x008   
#define MMU_IRQ_STATUS          0x00C   

#define MMU_AS0                 0x400   
#define MMU_AS1                 0x440   
#define MMU_AS2                 0x480   
#define MMU_AS3                 0x4C0   
#define MMU_AS4                 0x500   
#define MMU_AS5                 0x540   
#define MMU_AS6                 0x580   
#define MMU_AS7                 0x5C0   
#define MMU_AS8                 0x600   
#define MMU_AS9                 0x640   
#define MMU_AS10                0x680   
#define MMU_AS11                0x6C0   
#define MMU_AS12                0x700   
#define MMU_AS13                0x740   
#define MMU_AS14                0x780   
#define MMU_AS15                0x7C0   



#define MMU_AS_REG(n, r)        (MMU_REG(MMU_AS0 + ((n) << 6)) + (r))

#define AS_TRANSTAB_LO         0x00	
#define AS_TRANSTAB_HI         0x04	
#define AS_MEMATTR_LO          0x08	
#define AS_MEMATTR_HI          0x0C	
#define AS_LOCKADDR_LO         0x10	
#define AS_LOCKADDR_HI         0x14	
#define AS_COMMAND             0x18	
#define AS_FAULTSTATUS         0x1C	
#define AS_FAULTADDRESS_LO     0x20	
#define AS_FAULTADDRESS_HI     0x24	
#define AS_STATUS              0x28	


#define AS_TRANSCFG_LO         0x30

#define AS_TRANSCFG_HI         0x34

#define AS_FAULTEXTRA_LO       0x38

#define AS_FAULTEXTRA_HI       0x3C



#define GPU_IRQ_REG_ALL (GPU_IRQ_REG_COMMON)



#define MMU_PAGE_FAULT_FLAGS    16


#define MMU_PAGE_FAULT(n)       (1UL << (n))
#define MMU_BUS_ERROR(n)        (1UL << ((n) + MMU_PAGE_FAULT_FLAGS))


#define MMU_HW_OUTA_BITS 40
#define AS_TRANSTAB_BASE_MASK ((1ULL << MMU_HW_OUTA_BITS) - (1ULL << 4))


#define AS_STATUS_AS_ACTIVE 0x01

#define AS_FAULTSTATUS_EXCEPTION_CODE_MASK                      (0x7<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_TRANSLATION_FAULT         (0x0<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_PERMISSION_FAULT          (0x1<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_TRANSTAB_BUS_FAULT        (0x2<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_ACCESS_FLAG               (0x3<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_ADDRESS_SIZE_FAULT        (0x4<<3)
#define AS_FAULTSTATUS_EXCEPTION_CODE_MEMORY_ATTRIBUTES_FAULT   (0x5<<3)

#define AS_FAULTSTATUS_EXCEPTION_TYPE_SHIFT 0
#define AS_FAULTSTATUS_EXCEPTION_TYPE_MASK (0xFF << AS_FAULTSTATUS_EXCEPTION_TYPE_SHIFT)
#define AS_FAULTSTATUS_EXCEPTION_TYPE_GET(reg_val) \
	(((reg_val)&AS_FAULTSTATUS_EXCEPTION_TYPE_MASK) >> AS_FAULTSTATUS_EXCEPTION_TYPE_SHIFT)
#define AS_FAULTSTATUS_EXCEPTION_TYPE_TRANSLATION_FAULT_0 0xC0

#define AS_FAULTSTATUS_ACCESS_TYPE_SHIFT 8
#define AS_FAULTSTATUS_ACCESS_TYPE_MASK (0x3 << AS_FAULTSTATUS_ACCESS_TYPE_SHIFT)
#define AS_FAULTSTATUS_ACCESS_TYPE_GET(reg_val) \
	(((reg_val)&AS_FAULTSTATUS_ACCESS_TYPE_MASK) >> AS_FAULTSTATUS_ACCESS_TYPE_SHIFT)

#define AS_FAULTSTATUS_ACCESS_TYPE_ATOMIC       (0x0)
#define AS_FAULTSTATUS_ACCESS_TYPE_EX           (0x1)
#define AS_FAULTSTATUS_ACCESS_TYPE_READ         (0x2)
#define AS_FAULTSTATUS_ACCESS_TYPE_WRITE        (0x3)

#define AS_FAULTSTATUS_SOURCE_ID_SHIFT 16
#define AS_FAULTSTATUS_SOURCE_ID_MASK (0xFFFF << AS_FAULTSTATUS_SOURCE_ID_SHIFT)
#define AS_FAULTSTATUS_SOURCE_ID_GET(reg_val) \
	(((reg_val)&AS_FAULTSTATUS_SOURCE_ID_MASK) >> AS_FAULTSTATUS_SOURCE_ID_SHIFT)

#define PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_SHIFT (0)
#define PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_MASK                                \
	((0xFF) << PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_SHIFT)
#define PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_GET(reg_val)                        \
	(((reg_val)&PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_MASK) >>                \
	 PRFCNT_FEATURES_COUNTER_BLOCK_SIZE_SHIFT)


#define AS_TRANSCFG_ADRMODE_LEGACY      0
#define AS_TRANSCFG_ADRMODE_UNMAPPED    1
#define AS_TRANSCFG_ADRMODE_IDENTITY    2
#define AS_TRANSCFG_ADRMODE_AARCH64_4K  6
#define AS_TRANSCFG_ADRMODE_AARCH64_64K 8

#define AS_TRANSCFG_ADRMODE_MASK        0xF


#define AS_TRANSCFG_PTW_MEMATTR_MASK (3ull << 24)
#define AS_TRANSCFG_PTW_MEMATTR_NON_CACHEABLE (1ull << 24)
#define AS_TRANSCFG_PTW_MEMATTR_WRITE_BACK (2ull << 24)

#define AS_TRANSCFG_PTW_SH_MASK ((3ull << 28))
#define AS_TRANSCFG_PTW_SH_OS (2ull << 28)
#define AS_TRANSCFG_PTW_SH_IS (3ull << 28)
#define AS_TRANSCFG_R_ALLOCATE (1ull << 30)




#define AS_COMMAND_NOP         0x00	
#define AS_COMMAND_UPDATE      0x01	
#define AS_COMMAND_LOCK        0x02	
#define AS_COMMAND_UNLOCK      0x03	

#define AS_COMMAND_FLUSH 0x04

#define AS_COMMAND_FLUSH_PT 0x04

#define AS_COMMAND_FLUSH_MEM 0x05


#define GPU_STATUS_PRFCNT_ACTIVE            (1 << 2)    
#define GPU_STATUS_CYCLE_COUNT_ACTIVE       (1 << 6)    
#define GPU_STATUS_PROTECTED_MODE_ACTIVE    (1 << 7)    


#define PRFCNT_CONFIG_MODE_SHIFT        0 
#define PRFCNT_CONFIG_AS_SHIFT          4 
#define PRFCNT_CONFIG_SETSELECT_SHIFT   8 


#define PRFCNT_CONFIG_MODE_OFF          0

#define PRFCNT_CONFIG_MODE_MANUAL       1

#define PRFCNT_CONFIG_MODE_TILE         2



#define AS_MEMATTR_IMPL_DEF_CACHE_POLICY 0x88ull

#define AS_MEMATTR_FORCE_TO_CACHE_ALL    0x8Full

#define AS_MEMATTR_WRITE_ALLOC           0x8Dull


#define AS_MEMATTR_LPAE_IMPL_DEF_CACHE_POLICY 0x48ull

#define AS_MEMATTR_LPAE_FORCE_TO_CACHE_ALL    0x4Full

#define AS_MEMATTR_LPAE_WRITE_ALLOC           0x4Dull

#define AS_MEMATTR_LPAE_OUTER_IMPL_DEF        0x88ull

#define AS_MEMATTR_LPAE_OUTER_WA              0x8Dull

#define AS_MEMATTR_LPAE_NON_CACHEABLE_RESERVED


#define L2_MMU_CONFIG_ALLOW_SNOOP_DISPARITY_SHIFT       (23)
#define L2_MMU_CONFIG_ALLOW_SNOOP_DISPARITY             (0x1 << L2_MMU_CONFIG_ALLOW_SNOOP_DISPARITY_SHIFT)






#define IMPLEMENTATION_UNSPECIFIED  0
#define IMPLEMENTATION_SILICON      1
#define IMPLEMENTATION_FPGA         2
#define IMPLEMENTATION_MODEL        3


#define THREAD_MT_DEFAULT     256
#define THREAD_MWS_DEFAULT    256
#define THREAD_MBS_DEFAULT    256
#define THREAD_MR_DEFAULT     1024
#define THREAD_MTQ_DEFAULT    4
#define THREAD_MTGS_DEFAULT   10




#define SC_LS_ALLOW_ATTR_TYPES      (1ul << 16)
#define SC_TLS_HASH_ENABLE          (1ul << 17)
#define SC_LS_ATTR_CHECK_DISABLE    (1ul << 18)
#define SC_VAR_ALGORITHM            (1ul << 29)



#define TC_CLOCK_GATE_OVERRIDE      (1ul << 0)



#define L2_CONFIG_SIZE_SHIFT        16
#define L2_CONFIG_SIZE_MASK         (0xFFul << L2_CONFIG_SIZE_SHIFT)
#define L2_CONFIG_HASH_SHIFT        24
#define L2_CONFIG_HASH_MASK         (0xFFul << L2_CONFIG_HASH_SHIFT)
#define L2_CONFIG_ASN_HASH_ENABLE_SHIFT        24
#define L2_CONFIG_ASN_HASH_ENABLE_MASK         (1ul << L2_CONFIG_ASN_HASH_ENABLE_SHIFT)



#define IDVS_GROUP_SIZE_SHIFT (16)
#define IDVS_GROUP_MAX_SIZE (0x3F)

#endif 
