/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_BASE_CSF_KERNEL_H_
#define _UAPI_BASE_CSF_KERNEL_H_

#include <linux/types.h>
#include "../mali_base_common_kernel.h"




#define BASE_MEM_FIXED ((base_mem_alloc_flags)1 << 8)


#define BASE_MEM_CSF_EVENT ((base_mem_alloc_flags)1 << 19)

#define BASE_MEM_RESERVED_BIT_20 ((base_mem_alloc_flags)1 << 20)



#define BASE_MEM_FIXABLE ((base_mem_alloc_flags)1 << 29)




#define BASEP_MEM_FLAGS_KERNEL_ONLY \
	(BASEP_MEM_PERMANENT_KERNEL_MAPPING | BASEP_MEM_NO_USER_FREE)


#define BASE_MEM_FLAGS_RESERVED BASE_MEM_RESERVED_BIT_20


#define BASEP_MEM_CSF_USER_REG_PAGE_HANDLE (47ul << LOCAL_PAGE_SHIFT)
#define BASEP_MEM_CSF_USER_IO_PAGES_HANDLE (48ul << LOCAL_PAGE_SHIFT)

#define KBASE_CSF_NUM_USER_IO_PAGES_HANDLE \
	((BASE_MEM_COOKIE_BASE - BASEP_MEM_CSF_USER_IO_PAGES_HANDLE) >> \
	 LOCAL_PAGE_SHIFT)


#define BASE_JIT_ALLOC_VALID_FLAGS ((__u8)0)




#define BASE_CONTEXT_CSF_EVENT_THREAD ((base_context_create_flags)1 << 2)


#define BASEP_CONTEXT_CREATE_ALLOWED_FLAGS \
	(BASE_CONTEXT_CCTX_EMBEDDED | \
	 BASE_CONTEXT_CSF_EVENT_THREAD | \
	 BASEP_CONTEXT_CREATE_KERNEL_FLAGS)




#define BASE_TLSTREAM_ENABLE_CSF_TRACEPOINTS (1 << 2)


#define BASE_TLSTREAM_ENABLE_CSFFW_TRACEPOINTS (1 << 3)

#define BASE_TLSTREAM_FLAGS_MASK (BASE_TLSTREAM_ENABLE_LATENCY_TRACEPOINTS | \
		BASE_TLSTREAM_JOB_DUMPING_ENABLED | \
		BASE_TLSTREAM_ENABLE_CSF_TRACEPOINTS | \
		BASE_TLSTREAM_ENABLE_CSFFW_TRACEPOINTS)


#define BASEP_QUEUE_NR_MMAP_USER_PAGES ((size_t)3)

#define BASE_QUEUE_MAX_PRIORITY (15U)


#define BASEP_EVENT_VAL_INDEX (0U)
#define BASEP_EVENT_ERR_INDEX (1U)


#define BASEP_KCPU_CQS_MAX_NUM_OBJS ((size_t)32)


#define BASE_CSF_TILER_OOM_EXCEPTION_FLAG (1u << 0)
#define BASE_CSF_EXCEPTION_HANDLER_FLAGS_MASK (BASE_CSF_TILER_OOM_EXCEPTION_FLAG)


enum base_kcpu_command_type {
	BASE_KCPU_COMMAND_TYPE_FENCE_SIGNAL,
	BASE_KCPU_COMMAND_TYPE_FENCE_WAIT,
	BASE_KCPU_COMMAND_TYPE_CQS_WAIT,
	BASE_KCPU_COMMAND_TYPE_CQS_SET,
	BASE_KCPU_COMMAND_TYPE_CQS_WAIT_OPERATION,
	BASE_KCPU_COMMAND_TYPE_CQS_SET_OPERATION,
	BASE_KCPU_COMMAND_TYPE_MAP_IMPORT,
	BASE_KCPU_COMMAND_TYPE_UNMAP_IMPORT,
	BASE_KCPU_COMMAND_TYPE_UNMAP_IMPORT_FORCE,
	BASE_KCPU_COMMAND_TYPE_JIT_ALLOC,
	BASE_KCPU_COMMAND_TYPE_JIT_FREE,
	BASE_KCPU_COMMAND_TYPE_GROUP_SUSPEND,
	BASE_KCPU_COMMAND_TYPE_ERROR_BARRIER
};


enum base_queue_group_priority {
	BASE_QUEUE_GROUP_PRIORITY_HIGH = 0,
	BASE_QUEUE_GROUP_PRIORITY_MEDIUM,
	BASE_QUEUE_GROUP_PRIORITY_LOW,
	BASE_QUEUE_GROUP_PRIORITY_REALTIME,
	BASE_QUEUE_GROUP_PRIORITY_COUNT
};

struct base_kcpu_command_fence_info {
	__u64 fence;
};

struct base_cqs_wait_info {
	__u64 addr;
	__u32 val;
	__u32 padding;
};

struct base_kcpu_command_cqs_wait_info {
	__u64 objs;
	__u32 nr_objs;
	__u32 inherit_err_flags;
};

struct base_cqs_set {
	__u64 addr;
};

struct base_kcpu_command_cqs_set_info {
	__u64 objs;
	__u32 nr_objs;
	__u32 padding;
};


typedef enum PACKED {
	BASEP_CQS_DATA_TYPE_U32 = 0,
	BASEP_CQS_DATA_TYPE_U64 = 1,
} basep_cqs_data_type;


typedef enum {
	BASEP_CQS_WAIT_OPERATION_LE = 0,
	BASEP_CQS_WAIT_OPERATION_GT = 1,
} basep_cqs_wait_operation_op;

struct base_cqs_wait_operation_info {
	__u64 addr;
	__u64 val;
	__u8 operation;
	__u8 data_type;
	__u8 padding[6];
};


struct base_kcpu_command_cqs_wait_operation_info {
	__u64 objs;
	__u32 nr_objs;
	__u32 inherit_err_flags;
};


typedef enum {
	BASEP_CQS_SET_OPERATION_ADD = 0,
	BASEP_CQS_SET_OPERATION_SET = 1,
} basep_cqs_set_operation_op;

struct base_cqs_set_operation_info {
	__u64 addr;
	__u64 val;
	__u8 operation;
	__u8 data_type;
	__u8 padding[6];
};


struct base_kcpu_command_cqs_set_operation_info {
	__u64 objs;
	__u32 nr_objs;
	__u32 padding;
};


struct base_kcpu_command_import_info {
	__u64 handle;
};


struct base_kcpu_command_jit_alloc_info {
	__u64 info;
	__u8 count;
	__u8 padding[7];
};


struct base_kcpu_command_jit_free_info {
	__u64 ids;
	__u8 count;
	__u8 padding[7];
};


struct base_kcpu_command_group_suspend_info {
	__u64 buffer;
	__u32 size;
	__u8 group_handle;
	__u8 padding[3];
};



struct base_kcpu_command {
	__u8 type;
	__u8 padding[sizeof(__u64) - sizeof(__u8)];
	union {
		struct base_kcpu_command_fence_info fence;
		struct base_kcpu_command_cqs_wait_info cqs_wait;
		struct base_kcpu_command_cqs_set_info cqs_set;
		struct base_kcpu_command_cqs_wait_operation_info cqs_wait_operation;
		struct base_kcpu_command_cqs_set_operation_info cqs_set_operation;
		struct base_kcpu_command_import_info import;
		struct base_kcpu_command_jit_alloc_info jit_alloc;
		struct base_kcpu_command_jit_free_info jit_free;
		struct base_kcpu_command_group_suspend_info suspend_buf_copy;
		__u64 padding[2]; 
	} info;
};


struct basep_cs_stream_control {
	__u32 features;
	__u32 padding;
};


struct basep_cs_group_control {
	__u32 features;
	__u32 stream_num;
	__u32 suspend_size;
	__u32 padding;
};


struct base_gpu_queue_group_error_fatal_payload {
	__u64 sideband;
	__u32 status;
	__u32 padding;
};


struct base_gpu_queue_error_fatal_payload {
	__u64 sideband;
	__u32 status;
	__u8 csi_index;
	__u8 padding[3];
};


enum base_gpu_queue_group_error_type {
	BASE_GPU_QUEUE_GROUP_ERROR_FATAL = 0,
	BASE_GPU_QUEUE_GROUP_QUEUE_ERROR_FATAL,
	BASE_GPU_QUEUE_GROUP_ERROR_TIMEOUT,
	BASE_GPU_QUEUE_GROUP_ERROR_TILER_HEAP_OOM,
	BASE_GPU_QUEUE_GROUP_ERROR_FATAL_COUNT
};


struct base_gpu_queue_group_error {
	__u8 error_type;
	__u8 padding[7];
	union {
		struct base_gpu_queue_group_error_fatal_payload fatal_group;
		struct base_gpu_queue_error_fatal_payload fatal_queue;
	} payload;
};


enum base_csf_notification_type {
	BASE_CSF_NOTIFICATION_EVENT = 0,
	BASE_CSF_NOTIFICATION_GPU_QUEUE_GROUP_ERROR,
	BASE_CSF_NOTIFICATION_CPU_QUEUE_DUMP,
	BASE_CSF_NOTIFICATION_COUNT
};


struct base_csf_notification {
	__u8 type;
	__u8 padding[7];
	union {
		struct {
			__u8 handle;
			__u8 padding[7];
			struct base_gpu_queue_group_error error;
		} csg_error;

		__u8 align[56];
	} payload;
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
};

#endif 
