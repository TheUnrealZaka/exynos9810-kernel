/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */




#ifndef _UAPI_BASE_KERNEL_H_
#define _UAPI_BASE_KERNEL_H_

#include <linux/types.h>
#include "mali_base_mem_priv.h"
#include "gpu/mali_kbase_gpu_id.h"
#include "gpu/mali_kbase_gpu_coherency.h"

#define BASE_MAX_COHERENT_GROUPS 16

#if defined(PAGE_MASK) && defined(PAGE_SHIFT)
#define LOCAL_PAGE_SHIFT PAGE_SHIFT
#define LOCAL_PAGE_LSB ~PAGE_MASK
#else
#ifndef OSU_CONFIG_CPU_PAGE_SIZE_LOG2
#define OSU_CONFIG_CPU_PAGE_SIZE_LOG2 12
#endif

#if defined(OSU_CONFIG_CPU_PAGE_SIZE_LOG2)
#define LOCAL_PAGE_SHIFT OSU_CONFIG_CPU_PAGE_SIZE_LOG2
#define LOCAL_PAGE_LSB ((1ul << OSU_CONFIG_CPU_PAGE_SIZE_LOG2) - 1)
#else
#error Failed to find page size
#endif
#endif


#define BASE_MEM_GROUP_DEFAULT (0)


#define BASE_MEM_GROUP_COUNT (16)


typedef __u32 base_mem_alloc_flags;


#define BASE_MEM_FLAGS_MODIFIABLE \
	(BASE_MEM_DONT_NEED | BASE_MEM_COHERENT_SYSTEM | \
	 BASE_MEM_COHERENT_LOCAL)


#define BASE_MEM_FLAGS_QUERYABLE \
	(BASE_MEM_FLAGS_INPUT_MASK & ~(BASE_MEM_SAME_VA | \
		BASE_MEM_COHERENT_SYSTEM_REQUIRED | BASE_MEM_DONT_NEED | \
		BASE_MEM_IMPORT_SHARED | BASE_MEM_FLAGS_RESERVED | \
		BASEP_MEM_FLAGS_KERNEL_ONLY))


enum base_mem_import_type {
	BASE_MEM_IMPORT_TYPE_INVALID = 0,
	
	BASE_MEM_IMPORT_TYPE_UMM = 2,
	BASE_MEM_IMPORT_TYPE_USER_BUFFER = 3
};



struct base_mem_import_user_buffer {
	__u64 ptr;
	__u64 length;
};


#define BASE_MEM_MASK_4GB  0xfffff000UL

#define BASE_MEM_PFN_MASK_4GB  (BASE_MEM_MASK_4GB >> LOCAL_PAGE_SHIFT)


#define BASE_MEM_TILER_ALIGN_TOP_EXTENSION_MAX_PAGES_LOG2                      \
	(21u - (LOCAL_PAGE_SHIFT))
#define BASE_MEM_TILER_ALIGN_TOP_EXTENSION_MAX_PAGES                           \
	(1ull << (BASE_MEM_TILER_ALIGN_TOP_EXTENSION_MAX_PAGES_LOG2))


#define KBASE_COOKIE_MASK  ~1UL 


#define KBASE_MEM_ALLOC_MAX_SIZE ((8ull << 30) >> PAGE_SHIFT) 


struct base_fence {
	struct {
		int fd;
		int stream_fd;
	} basep;
};


struct base_mem_aliasing_info {
	struct base_mem_handle handle;
	__u64 offset;
	__u64 length;
};


#define BASE_JIT_MAX_TRIM_LEVEL (100)


#define BASE_JIT_ALLOC_COUNT (255)


struct base_jit_alloc_info_10_2 {
	__u64 gpu_alloc_addr;
	__u64 va_pages;
	__u64 commit_pages;
	__u64 extension;
	__u8 id;
};


struct base_jit_alloc_info_11_5 {
	__u64 gpu_alloc_addr;
	__u64 va_pages;
	__u64 commit_pages;
	__u64 extension;
	__u8 id;
	__u8 bin_id;
	__u8 max_allocations;
	__u8 flags;
	__u8 padding[2];
	__u16 usage_id;
};


struct base_jit_alloc_info {
	__u64 gpu_alloc_addr;
	__u64 va_pages;
	__u64 commit_pages;
	__u64 extension;
	__u8 id;
	__u8 bin_id;
	__u8 max_allocations;
	__u8 flags;
	__u8 padding[2];
	__u16 usage_id;
	__u64 heap_info_gpu_addr;
};

enum base_external_resource_access {
	BASE_EXT_RES_ACCESS_SHARED,
	BASE_EXT_RES_ACCESS_EXCLUSIVE
};

struct base_external_resource {
	__u64 ext_resource;
};


#define BASE_EXT_RES_COUNT_MAX 10


struct base_external_resource_list {
	__u64 count;
	struct base_external_resource ext_res[1];
};

struct base_jd_debug_copy_buffer {
	__u64 address;
	__u64 size;
	struct base_external_resource extres;
};

#define GPU_MAX_JOB_SLOTS 16




struct mali_base_gpu_l2_cache_props {
	__u8 log2_line_size;
	__u8 log2_cache_size;
	__u8 num_l2_slices; 
	__u8 padding[5];
};

struct mali_base_gpu_tiler_props {
	__u32 bin_size_bytes;	
	__u32 max_active_levels;	
};


struct mali_base_gpu_thread_props {
	__u32 max_threads;
	__u32 max_workgroup_size;
	__u32 max_barrier_size;
	__u16 max_registers;
	__u8 max_task_queue;
	__u8 max_thread_group_split;
	__u8 impl_tech;
	__u8  padding[3];
	__u32 tls_alloc;
};


struct mali_base_gpu_coherent_group {
	__u64 core_mask;
	__u16 num_cores;
	__u16 padding[3];
};


struct mali_base_gpu_coherent_group_info {
	__u32 num_groups;
	__u32 num_core_groups;
	__u32 coherency;
	__u32 padding;
	struct mali_base_gpu_coherent_group group[BASE_MAX_COHERENT_GROUPS];
};

#if MALI_USE_CSF
#include "csf/mali_base_csf_kernel.h"
#else
#include "jm/mali_base_jm_kernel.h"
#endif


struct gpu_raw_gpu_props {
	__u64 shader_present;
	__u64 tiler_present;
	__u64 l2_present;
	__u64 stack_present;
	__u32 l2_features;
	__u32 core_features;
	__u32 mem_features;
	__u32 mmu_features;

	__u32 as_present;

	__u32 js_present;
	__u32 js_features[GPU_MAX_JOB_SLOTS];
	__u32 tiler_features;
	__u32 texture_features[BASE_GPU_NUM_TEXTURE_FEATURES_REGISTERS];

	__u32 gpu_id;

	__u32 thread_max_threads;
	__u32 thread_max_workgroup_size;
	__u32 thread_max_barrier_size;
	__u32 thread_features;

	
	__u32 coherency_mode;

	__u32 thread_tls_alloc;
	__u64 gpu_features;
};


struct base_gpu_props {
	struct mali_base_gpu_core_props core_props;
	struct mali_base_gpu_l2_cache_props l2_props;
	__u64 unused_1;
	struct mali_base_gpu_tiler_props tiler_props;
	struct mali_base_gpu_thread_props thread_props;
	struct gpu_raw_gpu_props raw_props;
	struct mali_base_gpu_coherent_group_info coherency_info;
};

#define BASE_MEM_GROUP_ID_GET(flags)                                           \
	((flags & BASE_MEM_GROUP_ID_MASK) >> BASEP_MEM_GROUP_ID_SHIFT)

#define BASE_MEM_GROUP_ID_SET(id)                                              \
	(((base_mem_alloc_flags)((id < 0 || id >= BASE_MEM_GROUP_COUNT) ?      \
					 BASE_MEM_GROUP_DEFAULT :              \
					 id)                                   \
	  << BASEP_MEM_GROUP_ID_SHIFT) &                                       \
	 BASE_MEM_GROUP_ID_MASK)

#define BASE_CONTEXT_MMU_GROUP_ID_SET(group_id)                                \
	(BASEP_CONTEXT_MMU_GROUP_ID_MASK &                                     \
	 ((base_context_create_flags)(group_id)                                \
	  << BASEP_CONTEXT_MMU_GROUP_ID_SHIFT))

#define BASE_CONTEXT_MMU_GROUP_ID_GET(flags)                                   \
	((flags & BASEP_CONTEXT_MMU_GROUP_ID_MASK) >>                          \
	 BASEP_CONTEXT_MMU_GROUP_ID_SHIFT)




#define BASE_TIMEINFO_MONOTONIC_FLAG (1UL << 0)

#define BASE_TIMEINFO_TIMESTAMP_FLAG (1UL << 1)

#define BASE_TIMEINFO_CYCLE_COUNTER_FLAG (1UL << 2)

#define BASE_TIMEINFO_KERNEL_SOURCE_FLAG (1UL << 30)

#define BASE_TIMEINFO_USER_SOURCE_FLAG (1UL << 31)

#define BASE_TIMEREQUEST_ALLOWED_FLAGS (\
		BASE_TIMEINFO_MONOTONIC_FLAG | \
		BASE_TIMEINFO_TIMESTAMP_FLAG | \
		BASE_TIMEINFO_CYCLE_COUNTER_FLAG | \
		BASE_TIMEINFO_KERNEL_SOURCE_FLAG | \
		BASE_TIMEINFO_USER_SOURCE_FLAG)


#define BASE_MEM_ALIAS_MAX_ENTS ((size_t)24576)

#endif 
