/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_GPU_REGMAP_JM_H_
#define _UAPI_KBASE_GPU_REGMAP_JM_H_

#if MALI_USE_CSF && defined(__KERNEL__)
#error "Cannot be compiled with CSF"
#endif


#define AS_MEMATTR_AARCH64_OUTER_IMPL_DEF 0x88ull

#define AS_MEMATTR_AARCH64_OUTER_WA       0x8Dull

#define AS_MEMATTR_AARCH64_NON_CACHEABLE  0x4Cull


#define AS_MEMATTR_INDEX_DEFAULT               0
#define AS_MEMATTR_INDEX_DEFAULT_ACE           3


#define AS_MEMATTR_INDEX_IMPL_DEF_CACHE_POLICY 0

#define AS_MEMATTR_INDEX_FORCE_TO_CACHE_ALL    1

#define AS_MEMATTR_INDEX_WRITE_ALLOC           2

#define AS_MEMATTR_INDEX_OUTER_IMPL_DEF        3

#define AS_MEMATTR_INDEX_OUTER_WA              4

#define AS_MEMATTR_INDEX_NON_CACHEABLE         5



#define CORE_FEATURES           0x008   
#define JS_PRESENT              0x01C   
#define LATEST_FLUSH            0x038   

#define PRFCNT_BASE_LO   0x060  
#define PRFCNT_BASE_HI   0x064  
#define PRFCNT_CONFIG    0x068  
#define PRFCNT_JM_EN     0x06C  
#define PRFCNT_SHADER_EN 0x070  
#define PRFCNT_TILER_EN  0x074  
#define PRFCNT_MMU_L2_EN 0x07C  

#define JS0_FEATURES            0x0C0   
#define JS1_FEATURES            0x0C4   
#define JS2_FEATURES            0x0C8   
#define JS3_FEATURES            0x0CC   
#define JS4_FEATURES            0x0D0   
#define JS5_FEATURES            0x0D4   
#define JS6_FEATURES            0x0D8   
#define JS7_FEATURES            0x0DC   
#define JS8_FEATURES            0x0E0   
#define JS9_FEATURES            0x0E4   
#define JS10_FEATURES           0x0E8   
#define JS11_FEATURES           0x0EC   
#define JS12_FEATURES           0x0F0   
#define JS13_FEATURES           0x0F4   
#define JS14_FEATURES           0x0F8   
#define JS15_FEATURES           0x0FC   

#define JS_FEATURES_REG(n)      GPU_CONTROL_REG(JS0_FEATURES + ((n) << 2))

#define JM_CONFIG               0xF00   



#define JOB_IRQ_JS_STATE        0x010   
#define JOB_IRQ_THROTTLE        0x014   

#define JOB_SLOT0               0x800   
#define JOB_SLOT1               0x880   
#define JOB_SLOT2               0x900   
#define JOB_SLOT3               0x980   
#define JOB_SLOT4               0xA00   
#define JOB_SLOT5               0xA80   
#define JOB_SLOT6               0xB00   
#define JOB_SLOT7               0xB80   
#define JOB_SLOT8               0xC00   
#define JOB_SLOT9               0xC80   
#define JOB_SLOT10              0xD00   
#define JOB_SLOT11              0xD80   
#define JOB_SLOT12              0xE00   
#define JOB_SLOT13              0xE80   
#define JOB_SLOT14              0xF00   
#define JOB_SLOT15              0xF80   

#define JOB_SLOT_REG(n, r)      (JOB_CONTROL_REG(JOB_SLOT0 + ((n) << 7)) + (r))

#define JS_HEAD_LO             0x00	
#define JS_HEAD_HI             0x04	
#define JS_TAIL_LO             0x08	
#define JS_TAIL_HI             0x0C	
#define JS_AFFINITY_LO         0x10	
#define JS_AFFINITY_HI         0x14	
#define JS_CONFIG              0x18	

#define JS_XAFFINITY           0x1C

#define JS_COMMAND             0x20	
#define JS_STATUS              0x24	

#define JS_HEAD_NEXT_LO        0x40	
#define JS_HEAD_NEXT_HI        0x44	

#define JS_AFFINITY_NEXT_LO    0x50	
#define JS_AFFINITY_NEXT_HI    0x54	
#define JS_CONFIG_NEXT         0x58	

#define JS_XAFFINITY_NEXT      0x5C

#define JS_COMMAND_NEXT        0x60	

#define JS_FLUSH_ID_NEXT       0x70	





#define JS_COMMAND_NOP         0x00	
#define JS_COMMAND_START       0x01	
#define JS_COMMAND_SOFT_STOP   0x02	
#define JS_COMMAND_HARD_STOP   0x03	
#define JS_COMMAND_SOFT_STOP_0 0x04	
#define JS_COMMAND_HARD_STOP_0 0x05	
#define JS_COMMAND_SOFT_STOP_1 0x06	
#define JS_COMMAND_HARD_STOP_1 0x07	

#define JS_COMMAND_MASK        0x07    


#define JS_CONFIG_START_FLUSH_NO_ACTION        (0u << 0)
#define JS_CONFIG_START_FLUSH_CLEAN            (1u << 8)
#define JS_CONFIG_START_FLUSH_CLEAN_INVALIDATE (3u << 8)
#define JS_CONFIG_START_MMU                    (1u << 10)
#define JS_CONFIG_JOB_CHAIN_FLAG               (1u << 11)
#define JS_CONFIG_END_FLUSH_NO_ACTION          JS_CONFIG_START_FLUSH_NO_ACTION
#define JS_CONFIG_END_FLUSH_CLEAN              (1u << 12)
#define JS_CONFIG_END_FLUSH_CLEAN_INVALIDATE   (3u << 12)
#define JS_CONFIG_ENABLE_FLUSH_REDUCTION       (1u << 14)
#define JS_CONFIG_DISABLE_DESCRIPTOR_WR_BK     (1u << 15)
#define JS_CONFIG_THREAD_PRI(n)                ((n) << 16)


#define JS_XAFFINITY_XAFFINITY_ENABLE (1u << 0)
#define JS_XAFFINITY_TILER_ENABLE     (1u << 8)
#define JS_XAFFINITY_CACHE_ENABLE     (1u << 16)






#define JS_STATUS_NO_EXCEPTION_BASE   0x00
#define JS_STATUS_INTERRUPTED         (JS_STATUS_NO_EXCEPTION_BASE + 0x02)	
#define JS_STATUS_STOPPED             (JS_STATUS_NO_EXCEPTION_BASE + 0x03)	
#define JS_STATUS_TERMINATED          (JS_STATUS_NO_EXCEPTION_BASE + 0x04)	


#define JS_STATUS_FAULT_BASE          0x40
#define JS_STATUS_CONFIG_FAULT        (JS_STATUS_FAULT_BASE)	
#define JS_STATUS_POWER_FAULT         (JS_STATUS_FAULT_BASE + 0x01)	
#define JS_STATUS_READ_FAULT          (JS_STATUS_FAULT_BASE + 0x02)	
#define JS_STATUS_WRITE_FAULT         (JS_STATUS_FAULT_BASE + 0x03)	
#define JS_STATUS_AFFINITY_FAULT      (JS_STATUS_FAULT_BASE + 0x04)	
#define JS_STATUS_BUS_FAULT           (JS_STATUS_FAULT_BASE + 0x08)	


#define JS_STATUS_INSTRUCTION_FAULT_BASE  0x50
#define JS_STATUS_INSTR_INVALID_PC        (JS_STATUS_INSTRUCTION_FAULT_BASE)	
#define JS_STATUS_INSTR_INVALID_ENC       (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x01)	
#define JS_STATUS_INSTR_TYPE_MISMATCH     (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x02)	
#define JS_STATUS_INSTR_OPERAND_FAULT     (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x03)	
#define JS_STATUS_INSTR_TLS_FAULT         (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x04)	
#define JS_STATUS_INSTR_BARRIER_FAULT     (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x05)	
#define JS_STATUS_INSTR_ALIGN_FAULT       (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x06)	

#define JS_STATUS_DATA_INVALID_FAULT      (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x08)	
#define JS_STATUS_TILE_RANGE_FAULT        (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x09)	
#define JS_STATUS_ADDRESS_RANGE_FAULT     (JS_STATUS_INSTRUCTION_FAULT_BASE + 0x0A)	


#define JS_STATUS_MEMORY_FAULT_BASE   0x60
#define JS_STATUS_OUT_OF_MEMORY       (JS_STATUS_MEMORY_FAULT_BASE)	
#define JS_STATUS_UNKNOWN             0x7F	


#define JS_FEATURE_NULL_JOB              (1u << 1)
#define JS_FEATURE_SET_VALUE_JOB         (1u << 2)
#define JS_FEATURE_CACHE_FLUSH_JOB       (1u << 3)
#define JS_FEATURE_COMPUTE_JOB           (1u << 4)
#define JS_FEATURE_VERTEX_JOB            (1u << 5)
#define JS_FEATURE_GEOMETRY_JOB          (1u << 6)
#define JS_FEATURE_TILER_JOB             (1u << 7)
#define JS_FEATURE_FUSED_JOB             (1u << 8)
#define JS_FEATURE_FRAGMENT_JOB          (1u << 9)


#define JM_TIMESTAMP_OVERRIDE  (1ul << 0)
#define JM_CLOCK_GATE_OVERRIDE (1ul << 1)
#define JM_JOB_THROTTLE_ENABLE (1ul << 2)
#define JM_JOB_THROTTLE_LIMIT_SHIFT (3)
#define JM_MAX_JOB_THROTTLE_LIMIT (0x3F)
#define JM_FORCE_COHERENCY_FEATURES_SHIFT (2)


#define GPU_COMMAND_NOP                0x00 
#define GPU_COMMAND_SOFT_RESET         0x01 
#define GPU_COMMAND_HARD_RESET         0x02 
#define GPU_COMMAND_PRFCNT_CLEAR       0x03 
#define GPU_COMMAND_PRFCNT_SAMPLE      0x04 
#define GPU_COMMAND_CYCLE_COUNT_START  0x05 
#define GPU_COMMAND_CYCLE_COUNT_STOP   0x06 
#define GPU_COMMAND_CLEAN_CACHES       0x07 
#define GPU_COMMAND_CLEAN_INV_CACHES   0x08 
#define GPU_COMMAND_SET_PROTECTED_MODE 0x09 

#define GPU_COMMAND_CACHE_CLN_INV_L2 GPU_COMMAND_CLEAN_INV_CACHES
#define GPU_COMMAND_CACHE_CLN_INV_L2_LSC GPU_COMMAND_CLEAN_INV_CACHES

#define GPU_COMMAND_FLUSH_CACHE_MERGE(cmd1, cmd2)                              \
	((cmd1) > (cmd2) ? (cmd1) : (cmd2))


#define GPU_FAULT               (1 << 0)    
#define MULTIPLE_GPU_FAULTS     (1 << 7)    
#define RESET_COMPLETED         (1 << 8)    
#define POWER_CHANGED_SINGLE    (1 << 9)    
#define POWER_CHANGED_ALL       (1 << 10)   
#define PRFCNT_SAMPLE_COMPLETED (1 << 16)   
#define CLEAN_CACHES_COMPLETED  (1 << 17)   
#define FLUSH_PA_RANGE_COMPLETED                                                                   \
	        (1 << 20) 


#define GPU_IRQ_REG_COMMON (GPU_FAULT | MULTIPLE_GPU_FAULTS | RESET_COMPLETED \
		| POWER_CHANGED_ALL | PRFCNT_SAMPLE_COMPLETED)

#endif 
