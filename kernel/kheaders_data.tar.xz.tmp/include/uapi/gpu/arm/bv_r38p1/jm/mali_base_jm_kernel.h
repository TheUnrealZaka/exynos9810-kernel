/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_BASE_JM_KERNEL_H_
#define _UAPI_BASE_JM_KERNEL_H_

#include <linux/types.h>
#include "../mali_base_common_kernel.h"




#define BASE_MEM_RESERVED_BIT_8 ((base_mem_alloc_flags)1 << 8)


#define BASE_MEM_RESERVED_BIT_19 ((base_mem_alloc_flags)1 << 19)


#define BASE_MEM_TILER_ALIGN_TOP ((base_mem_alloc_flags)1 << 20)


#define BASE_MEM_FLAG_MAP_FIXED ((base_mem_alloc_flags)1 << 27)


#define BASEP_MEM_PERFORM_JIT_TRIM ((base_mem_alloc_flags)1 << 29)




#define BASEP_MEM_FLAGS_KERNEL_ONLY \
	(BASEP_MEM_PERMANENT_KERNEL_MAPPING | BASEP_MEM_NO_USER_FREE | \
	 BASE_MEM_FLAG_MAP_FIXED | BASEP_MEM_PERFORM_JIT_TRIM)


#define BASE_MEM_FLAGS_RESERVED \
	(BASE_MEM_RESERVED_BIT_8 | BASE_MEM_RESERVED_BIT_19)



#define BASE_JIT_ALLOC_MEM_TILER_ALIGN_TOP  (1 << 0)


#define BASE_JIT_ALLOC_HEAP_INFO_IS_SIZE  (1 << 1)


#define BASE_JIT_ALLOC_VALID_FLAGS \
	(BASE_JIT_ALLOC_MEM_TILER_ALIGN_TOP | BASE_JIT_ALLOC_HEAP_INFO_IS_SIZE)


#define BASEP_CONTEXT_CREATE_ALLOWED_FLAGS \
	(BASE_CONTEXT_CCTX_EMBEDDED | BASEP_CONTEXT_CREATE_KERNEL_FLAGS)




#define BASEP_CONTEXT_FLAG_JOB_DUMP_DISABLED \
	((base_context_create_flags)(1 << 31))


#define BASE_TLSTREAM_FLAGS_MASK (BASE_TLSTREAM_ENABLE_LATENCY_TRACEPOINTS | \
		BASE_TLSTREAM_JOB_DUMPING_ENABLED)

#define BASE_JD_ATOM_COUNT              256


#define BASE_JD_RP_COUNT (256)


#define BASE_JD_SOFT_EVENT_SET             ((unsigned char)1)
#define BASE_JD_SOFT_EVENT_RESET           ((unsigned char)0)


struct base_jd_udata {
	__u64 blob[2];
};


typedef __u8 base_jd_dep_type;

#define BASE_JD_DEP_TYPE_INVALID  (0)       
#define BASE_JD_DEP_TYPE_DATA     (1U << 0) 
#define BASE_JD_DEP_TYPE_ORDER    (1U << 1) 


typedef __u32 base_jd_core_req;




#define BASE_JD_REQ_DEP ((base_jd_core_req)0)


#define BASE_JD_REQ_FS  ((base_jd_core_req)1 << 0)


#define BASE_JD_REQ_CS ((base_jd_core_req)1 << 1)


#define BASE_JD_REQ_T  ((base_jd_core_req)1 << 2)


#define BASE_JD_REQ_CF ((base_jd_core_req)1 << 3)


#define BASE_JD_REQ_V  ((base_jd_core_req)1 << 4)




#define BASE_JD_REQ_FS_AFBC  ((base_jd_core_req)1 << 13)


#define BASE_JD_REQ_EVENT_COALESCE ((base_jd_core_req)1 << 5)


#define BASE_JD_REQ_COHERENT_GROUP  ((base_jd_core_req)1 << 6)


#define BASE_JD_REQ_PERMON               ((base_jd_core_req)1 << 7)


#define BASE_JD_REQ_EXTERNAL_RESOURCES   ((base_jd_core_req)1 << 8)


#define BASE_JD_REQ_SOFT_JOB        ((base_jd_core_req)1 << 9)

#define BASE_JD_REQ_SOFT_DUMP_CPU_GPU_TIME      (BASE_JD_REQ_SOFT_JOB | 0x1)
#define BASE_JD_REQ_SOFT_FENCE_TRIGGER          (BASE_JD_REQ_SOFT_JOB | 0x2)
#define BASE_JD_REQ_SOFT_FENCE_WAIT             (BASE_JD_REQ_SOFT_JOB | 0x3)




#define BASE_JD_REQ_SOFT_EVENT_WAIT             (BASE_JD_REQ_SOFT_JOB | 0x5)
#define BASE_JD_REQ_SOFT_EVENT_SET              (BASE_JD_REQ_SOFT_JOB | 0x6)
#define BASE_JD_REQ_SOFT_EVENT_RESET            (BASE_JD_REQ_SOFT_JOB | 0x7)

#define BASE_JD_REQ_SOFT_DEBUG_COPY             (BASE_JD_REQ_SOFT_JOB | 0x8)


#define BASE_JD_REQ_SOFT_JIT_ALLOC              (BASE_JD_REQ_SOFT_JOB | 0x9)


#define BASE_JD_REQ_SOFT_JIT_FREE               (BASE_JD_REQ_SOFT_JOB | 0xa)


#define BASE_JD_REQ_SOFT_EXT_RES_MAP            (BASE_JD_REQ_SOFT_JOB | 0xb)


#define BASE_JD_REQ_SOFT_EXT_RES_UNMAP          (BASE_JD_REQ_SOFT_JOB | 0xc)


#define BASE_JD_REQ_ONLY_COMPUTE    ((base_jd_core_req)1 << 10)


#define BASE_JD_REQ_SPECIFIC_COHERENT_GROUP ((base_jd_core_req)1 << 11)


#define BASE_JD_REQ_EVENT_ONLY_ON_FAILURE   ((base_jd_core_req)1 << 12)


#define BASEP_JD_REQ_EVENT_NEVER ((base_jd_core_req)1 << 14)


#define BASE_JD_REQ_SKIP_CACHE_START ((base_jd_core_req)1 << 15)


#define BASE_JD_REQ_SKIP_CACHE_END ((base_jd_core_req)1 << 16)


#define BASE_JD_REQ_JOB_SLOT ((base_jd_core_req)1 << 17)


#define BASE_JD_REQ_START_RENDERPASS ((base_jd_core_req)1 << 18)


#define BASE_JD_REQ_END_RENDERPASS ((base_jd_core_req)1 << 19)


#define BASE_JD_REQ_LIMITED_CORE_MASK ((base_jd_core_req)1 << 20)


#define BASEP_JD_REQ_RESERVED \
	(~(BASE_JD_REQ_ATOM_TYPE | BASE_JD_REQ_EXTERNAL_RESOURCES | \
	BASE_JD_REQ_EVENT_ONLY_ON_FAILURE | BASEP_JD_REQ_EVENT_NEVER | \
	BASE_JD_REQ_EVENT_COALESCE | \
	BASE_JD_REQ_COHERENT_GROUP | BASE_JD_REQ_SPECIFIC_COHERENT_GROUP | \
	BASE_JD_REQ_FS_AFBC | BASE_JD_REQ_PERMON | \
	BASE_JD_REQ_SKIP_CACHE_START | BASE_JD_REQ_SKIP_CACHE_END | \
	BASE_JD_REQ_JOB_SLOT | BASE_JD_REQ_START_RENDERPASS | \
	BASE_JD_REQ_END_RENDERPASS | BASE_JD_REQ_LIMITED_CORE_MASK))


#define BASE_JD_REQ_ATOM_TYPE \
	(BASE_JD_REQ_FS | BASE_JD_REQ_CS | BASE_JD_REQ_T | BASE_JD_REQ_CF | \
	BASE_JD_REQ_V | BASE_JD_REQ_SOFT_JOB | BASE_JD_REQ_ONLY_COMPUTE)


#define BASE_JD_REQ_SOFT_JOB_TYPE (BASE_JD_REQ_SOFT_JOB | 0x1f)


#define BASE_JD_REQ_SOFT_JOB_OR_DEP(core_req) \
	(((core_req) & BASE_JD_REQ_SOFT_JOB) || \
	((core_req) & BASE_JD_REQ_ATOM_TYPE) == BASE_JD_REQ_DEP)


enum kbase_jd_atom_state {
	KBASE_JD_ATOM_STATE_UNUSED,
	KBASE_JD_ATOM_STATE_QUEUED,
	KBASE_JD_ATOM_STATE_IN_JS,
	KBASE_JD_ATOM_STATE_HW_COMPLETED,
	KBASE_JD_ATOM_STATE_COMPLETED
};


typedef __u8 base_atom_id;


struct base_dependency {
	base_atom_id atom_id;
	base_jd_dep_type dependency_type;
};


struct base_jd_fragment {
	__u64 norm_read_norm_write;
	__u64 norm_read_forced_write;
	__u64 forced_read_forced_write;
	__u64 forced_read_norm_write;
};


typedef __u8 base_jd_prio;


#define BASE_JD_PRIO_MEDIUM  ((base_jd_prio)0)

#define BASE_JD_PRIO_HIGH    ((base_jd_prio)1)

#define BASE_JD_PRIO_LOW     ((base_jd_prio)2)

#define BASE_JD_PRIO_REALTIME    ((base_jd_prio)3)


#define BASE_JD_PRIO_INVALID ((base_jd_prio)255)


#define BASE_JD_NR_PRIO_LEVELS 4


struct base_jd_atom_v2 {
	__u64 jc;
	struct base_jd_udata udata;
	__u64 extres_list;
	__u16 nr_extres;
	__u8 jit_id[2];
	struct base_dependency pre_dep[2];
	base_atom_id atom_number;
	base_jd_prio prio;
	__u8 device_nr;
	__u8 jobslot;
	base_jd_core_req core_req;
	__u8 renderpass_id;
	__u8 padding[7];
};


typedef struct base_jd_atom {
	__u64 seq_nr;
	__u64 jc;
	struct base_jd_udata udata;
	__u64 extres_list;
	__u16 nr_extres;
	__u8 jit_id[2];
	struct base_dependency pre_dep[2];
	base_atom_id atom_number;
	base_jd_prio prio;
	__u8 device_nr;
	__u8 jobslot;
	base_jd_core_req core_req;
	__u8 renderpass_id;
	__u8 padding[7];
} base_jd_atom;


enum {
	BASE_JD_SW_EVENT_KERNEL = (1u << 15), 
	BASE_JD_SW_EVENT = (1u << 14), 
	
	BASE_JD_SW_EVENT_SUCCESS = (1u << 13),
	BASE_JD_SW_EVENT_JOB = (0u << 11), 
	BASE_JD_SW_EVENT_BAG = (1u << 11), 
	BASE_JD_SW_EVENT_INFO = (2u << 11), 
	BASE_JD_SW_EVENT_RESERVED = (3u << 11),	
	
	BASE_JD_SW_EVENT_TYPE_MASK = (3u << 11)
};


enum base_jd_event_code {
	
	BASE_JD_EVENT_RANGE_HW_NONFAULT_START = 0,

	
	BASE_JD_EVENT_NOT_STARTED = 0x00,
	BASE_JD_EVENT_DONE = 0x01,
	BASE_JD_EVENT_STOPPED = 0x03,
	BASE_JD_EVENT_TERMINATED = 0x04,
	BASE_JD_EVENT_ACTIVE = 0x08,

	BASE_JD_EVENT_RANGE_HW_NONFAULT_END = 0x40,
	BASE_JD_EVENT_RANGE_HW_FAULT_OR_SW_ERROR_START = 0x40,

	
	BASE_JD_EVENT_JOB_CONFIG_FAULT = 0x40,
	BASE_JD_EVENT_JOB_POWER_FAULT = 0x41,
	BASE_JD_EVENT_JOB_READ_FAULT = 0x42,
	BASE_JD_EVENT_JOB_WRITE_FAULT = 0x43,
	BASE_JD_EVENT_JOB_AFFINITY_FAULT = 0x44,
	BASE_JD_EVENT_JOB_BUS_FAULT = 0x48,
	BASE_JD_EVENT_INSTR_INVALID_PC = 0x50,
	BASE_JD_EVENT_INSTR_INVALID_ENC = 0x51,
	BASE_JD_EVENT_INSTR_TYPE_MISMATCH = 0x52,
	BASE_JD_EVENT_INSTR_OPERAND_FAULT = 0x53,
	BASE_JD_EVENT_INSTR_TLS_FAULT = 0x54,
	BASE_JD_EVENT_INSTR_BARRIER_FAULT = 0x55,
	BASE_JD_EVENT_INSTR_ALIGN_FAULT = 0x56,
	BASE_JD_EVENT_DATA_INVALID_FAULT = 0x58,
	BASE_JD_EVENT_TILE_RANGE_FAULT = 0x59,
	BASE_JD_EVENT_STATE_FAULT = 0x5A,
	BASE_JD_EVENT_OUT_OF_MEMORY = 0x60,
	BASE_JD_EVENT_UNKNOWN = 0x7F,

	
	BASE_JD_EVENT_DELAYED_BUS_FAULT = 0x80,
	BASE_JD_EVENT_SHAREABILITY_FAULT = 0x88,

	
	BASE_JD_EVENT_TRANSLATION_FAULT_LEVEL1 = 0xC1,
	BASE_JD_EVENT_TRANSLATION_FAULT_LEVEL2 = 0xC2,
	BASE_JD_EVENT_TRANSLATION_FAULT_LEVEL3 = 0xC3,
	BASE_JD_EVENT_TRANSLATION_FAULT_LEVEL4 = 0xC4,
	BASE_JD_EVENT_PERMISSION_FAULT = 0xC8,
	BASE_JD_EVENT_TRANSTAB_BUS_FAULT_LEVEL1 = 0xD1,
	BASE_JD_EVENT_TRANSTAB_BUS_FAULT_LEVEL2 = 0xD2,
	BASE_JD_EVENT_TRANSTAB_BUS_FAULT_LEVEL3 = 0xD3,
	BASE_JD_EVENT_TRANSTAB_BUS_FAULT_LEVEL4 = 0xD4,
	BASE_JD_EVENT_ACCESS_FLAG = 0xD8,

	
	BASE_JD_EVENT_MEM_GROWTH_FAILED =
		BASE_JD_SW_EVENT | BASE_JD_SW_EVENT_JOB | 0x000,
	BASE_JD_EVENT_JOB_CANCELLED =
		BASE_JD_SW_EVENT | BASE_JD_SW_EVENT_JOB | 0x002,
	BASE_JD_EVENT_JOB_INVALID =
		BASE_JD_SW_EVENT | BASE_JD_SW_EVENT_JOB | 0x003,

	BASE_JD_EVENT_RANGE_HW_FAULT_OR_SW_ERROR_END = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_RESERVED | 0x3FF,

	BASE_JD_EVENT_RANGE_SW_SUCCESS_START = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_SUCCESS | 0x000,

	BASE_JD_EVENT_DRV_TERMINATED = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_SUCCESS | BASE_JD_SW_EVENT_INFO | 0x000,

	BASE_JD_EVENT_RANGE_SW_SUCCESS_END = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_SUCCESS | BASE_JD_SW_EVENT_RESERVED | 0x3FF,

	BASE_JD_EVENT_RANGE_KERNEL_ONLY_START = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_KERNEL | 0x000,
	BASE_JD_EVENT_REMOVED_FROM_NEXT = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_KERNEL | BASE_JD_SW_EVENT_JOB | 0x000,
	BASE_JD_EVENT_END_RP_DONE = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_KERNEL | BASE_JD_SW_EVENT_JOB | 0x001,

	BASE_JD_EVENT_RANGE_KERNEL_ONLY_END = BASE_JD_SW_EVENT |
		BASE_JD_SW_EVENT_KERNEL | BASE_JD_SW_EVENT_RESERVED | 0x3FF
};


struct base_jd_event_v2 {
	__u32 event_code;
	base_atom_id atom_number;
	__u8 padding[3];
	struct base_jd_udata udata;
};



struct base_dump_cpu_gpu_counters {
	__u64 system_time;
	__u64 cycle_counter;
	__u64 sec;
	__u32 usec;
	__u8 padding[36];
};


struct mali_base_gpu_core_props {
	__u32 product_id;
	__u16 version_status;
	__u16 minor_revision;
	__u16 major_revision;
	__u16 padding;
	__u32 gpu_freq_khz_max;
	__u32 log2_program_counter_size;
	__u32 texture_features[BASE_GPU_NUM_TEXTURE_FEATURES_REGISTERS];
	__u64 gpu_available_memory_size;
	__u8 num_exec_engines;
};

#endif 
