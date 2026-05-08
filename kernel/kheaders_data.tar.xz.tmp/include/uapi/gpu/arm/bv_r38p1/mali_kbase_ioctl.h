/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_IOCTL_H_
#define _UAPI_KBASE_IOCTL_H_

#ifdef __cpluscplus
extern "C" {
#endif

#include <asm-generic/ioctl.h>
#include <linux/types.h>

#if MALI_USE_CSF
#include "csf/mali_kbase_csf_ioctl.h"
#else
#include "jm/mali_kbase_jm_ioctl.h"
#endif 

#define KBASE_IOCTL_TYPE 0x80


struct kbase_ioctl_set_flags {
	__u32 create_flags;
};

#define KBASE_IOCTL_SET_FLAGS \
	_IOW(KBASE_IOCTL_TYPE, 1, struct kbase_ioctl_set_flags)


struct kbase_ioctl_get_gpuprops {
	__u64 buffer;
	__u32 size;
	__u32 flags;
};

#define KBASE_IOCTL_GET_GPUPROPS \
	_IOW(KBASE_IOCTL_TYPE, 3, struct kbase_ioctl_get_gpuprops)


union kbase_ioctl_mem_alloc {
	struct {
		__u64 va_pages;
		__u64 commit_pages;
		__u64 extension;
		__u64 flags;
	} in;
	struct {
		__u64 flags;
		__u64 gpu_va;
	} out;
};

#define KBASE_IOCTL_MEM_ALLOC \
	_IOWR(KBASE_IOCTL_TYPE, 5, union kbase_ioctl_mem_alloc)


union kbase_ioctl_mem_query {
	struct {
		__u64 gpu_addr;
		__u64 query;
	} in;
	struct {
		__u64 value;
	} out;
};

#define KBASE_IOCTL_MEM_QUERY \
	_IOWR(KBASE_IOCTL_TYPE, 6, union kbase_ioctl_mem_query)

#define KBASE_MEM_QUERY_COMMIT_SIZE	((__u64)1)
#define KBASE_MEM_QUERY_VA_SIZE		((__u64)2)
#define KBASE_MEM_QUERY_FLAGS		((__u64)3)


struct kbase_ioctl_mem_free {
	__u64 gpu_addr;
};

#define KBASE_IOCTL_MEM_FREE \
	_IOW(KBASE_IOCTL_TYPE, 7, struct kbase_ioctl_mem_free)


struct kbase_ioctl_hwcnt_reader_setup {
	__u32 buffer_count;
	__u32 fe_bm;
	__u32 shader_bm;
	__u32 tiler_bm;
	__u32 mmu_l2_bm;
};

#define KBASE_IOCTL_HWCNT_READER_SETUP \
	_IOW(KBASE_IOCTL_TYPE, 8, struct kbase_ioctl_hwcnt_reader_setup)


struct kbase_ioctl_hwcnt_values {
	__u64 data;
	__u32 size;
	__u32 padding;
};

#define KBASE_IOCTL_HWCNT_SET \
	_IOW(KBASE_IOCTL_TYPE, 32, struct kbase_ioctl_hwcnt_values)


struct kbase_ioctl_disjoint_query {
	__u32 counter;
};

#define KBASE_IOCTL_DISJOINT_QUERY \
	_IOR(KBASE_IOCTL_TYPE, 12, struct kbase_ioctl_disjoint_query)


struct kbase_ioctl_get_ddk_version {
	__u64 version_buffer;
	__u32 size;
	__u32 padding;
};

#define KBASE_IOCTL_GET_DDK_VERSION \
	_IOW(KBASE_IOCTL_TYPE, 13, struct kbase_ioctl_get_ddk_version)


struct kbase_ioctl_mem_jit_init_10_2 {
	__u64 va_pages;
};

#define KBASE_IOCTL_MEM_JIT_INIT_10_2 \
	_IOW(KBASE_IOCTL_TYPE, 14, struct kbase_ioctl_mem_jit_init_10_2)


struct kbase_ioctl_mem_jit_init_11_5 {
	__u64 va_pages;
	__u8 max_allocations;
	__u8 trim_level;
	__u8 group_id;
	__u8 padding[5];
};

#define KBASE_IOCTL_MEM_JIT_INIT_11_5 \
	_IOW(KBASE_IOCTL_TYPE, 14, struct kbase_ioctl_mem_jit_init_11_5)


struct kbase_ioctl_mem_jit_init {
	__u64 va_pages;
	__u8 max_allocations;
	__u8 trim_level;
	__u8 group_id;
	__u8 padding[5];
	__u64 phys_pages;
};

#define KBASE_IOCTL_MEM_JIT_INIT \
	_IOW(KBASE_IOCTL_TYPE, 14, struct kbase_ioctl_mem_jit_init)


struct kbase_ioctl_mem_sync {
	__u64 handle;
	__u64 user_addr;
	__u64 size;
	__u8 type;
	__u8 padding[7];
};

#define KBASE_IOCTL_MEM_SYNC \
	_IOW(KBASE_IOCTL_TYPE, 15, struct kbase_ioctl_mem_sync)


union kbase_ioctl_mem_find_cpu_offset {
	struct {
		__u64 gpu_addr;
		__u64 cpu_addr;
		__u64 size;
	} in;
	struct {
		__u64 offset;
	} out;
};

#define KBASE_IOCTL_MEM_FIND_CPU_OFFSET \
	_IOWR(KBASE_IOCTL_TYPE, 16, union kbase_ioctl_mem_find_cpu_offset)


struct kbase_ioctl_get_context_id {
	__u32 id;
};

#define KBASE_IOCTL_GET_CONTEXT_ID \
	_IOR(KBASE_IOCTL_TYPE, 17, struct kbase_ioctl_get_context_id)


struct kbase_ioctl_tlstream_acquire {
	__u32 flags;
};

#define KBASE_IOCTL_TLSTREAM_ACQUIRE \
	_IOW(KBASE_IOCTL_TYPE, 18, struct kbase_ioctl_tlstream_acquire)

#define KBASE_IOCTL_TLSTREAM_FLUSH \
	_IO(KBASE_IOCTL_TYPE, 19)


struct kbase_ioctl_mem_commit {
	__u64 gpu_addr;
	__u64 pages;
};

#define KBASE_IOCTL_MEM_COMMIT \
	_IOW(KBASE_IOCTL_TYPE, 20, struct kbase_ioctl_mem_commit)


union kbase_ioctl_mem_alias {
	struct {
		__u64 flags;
		__u64 stride;
		__u64 nents;
		__u64 aliasing_info;
	} in;
	struct {
		__u64 flags;
		__u64 gpu_va;
		__u64 va_pages;
	} out;
};

#define KBASE_IOCTL_MEM_ALIAS \
	_IOWR(KBASE_IOCTL_TYPE, 21, union kbase_ioctl_mem_alias)


union kbase_ioctl_mem_import {
	struct {
		__u64 flags;
		__u64 phandle;
		__u32 type;
		__u32 padding;
	} in;
	struct {
		__u64 flags;
		__u64 gpu_va;
		__u64 va_pages;
	} out;
};

#define KBASE_IOCTL_MEM_IMPORT \
	_IOWR(KBASE_IOCTL_TYPE, 22, union kbase_ioctl_mem_import)


struct kbase_ioctl_mem_flags_change {
	__u64 gpu_va;
	__u64 flags;
	__u64 mask;
};

#define KBASE_IOCTL_MEM_FLAGS_CHANGE \
	_IOW(KBASE_IOCTL_TYPE, 23, struct kbase_ioctl_mem_flags_change)


struct kbase_ioctl_stream_create {
	char name[32];
};

#define KBASE_IOCTL_STREAM_CREATE \
	_IOW(KBASE_IOCTL_TYPE, 24, struct kbase_ioctl_stream_create)


struct kbase_ioctl_fence_validate {
	int fd;
};

#define KBASE_IOCTL_FENCE_VALIDATE \
	_IOW(KBASE_IOCTL_TYPE, 25, struct kbase_ioctl_fence_validate)


struct kbase_ioctl_mem_profile_add {
	__u64 buffer;
	__u32 len;
	__u32 padding;
};

#define KBASE_IOCTL_MEM_PROFILE_ADD \
	_IOW(KBASE_IOCTL_TYPE, 27, struct kbase_ioctl_mem_profile_add)


struct kbase_ioctl_sticky_resource_map {
	__u64 count;
	__u64 address;
};

#define KBASE_IOCTL_STICKY_RESOURCE_MAP \
	_IOW(KBASE_IOCTL_TYPE, 29, struct kbase_ioctl_sticky_resource_map)


struct kbase_ioctl_sticky_resource_unmap {
	__u64 count;
	__u64 address;
};

#define KBASE_IOCTL_STICKY_RESOURCE_UNMAP \
	_IOW(KBASE_IOCTL_TYPE, 30, struct kbase_ioctl_sticky_resource_unmap)


union kbase_ioctl_mem_find_gpu_start_and_offset {
	struct {
		__u64 gpu_addr;
		__u64 size;
	} in;
	struct {
		__u64 start;
		__u64 offset;
	} out;
};

#define KBASE_IOCTL_MEM_FIND_GPU_START_AND_OFFSET \
	_IOWR(KBASE_IOCTL_TYPE, 31, union kbase_ioctl_mem_find_gpu_start_and_offset)

#define KBASE_IOCTL_CINSTR_GWT_START \
	_IO(KBASE_IOCTL_TYPE, 33)

#define KBASE_IOCTL_CINSTR_GWT_STOP \
	_IO(KBASE_IOCTL_TYPE, 34)


union kbase_ioctl_cinstr_gwt_dump {
	struct {
		__u64 addr_buffer;
		__u64 size_buffer;
		__u32 len;
		__u32 padding;

	} in;
	struct {
		__u32 no_of_addr_collected;
		__u8 more_data_available;
		__u8 padding[27];
	} out;
};

#define KBASE_IOCTL_CINSTR_GWT_DUMP \
	_IOWR(KBASE_IOCTL_TYPE, 35, union kbase_ioctl_cinstr_gwt_dump)


struct kbase_ioctl_mem_exec_init {
	__u64 va_pages;
};

#define KBASE_IOCTL_MEM_EXEC_INIT \
	_IOW(KBASE_IOCTL_TYPE, 38, struct kbase_ioctl_mem_exec_init)


union kbase_ioctl_get_cpu_gpu_timeinfo {
	struct {
		__u32 request_flags;
		__u32 paddings[7];
	} in;
	struct {
		__u64 sec;
		__u32 nsec;
		__u32 padding;
		__u64 timestamp;
		__u64 cycle_counter;
	} out;
};

#define KBASE_IOCTL_GET_CPU_GPU_TIMEINFO \
	_IOWR(KBASE_IOCTL_TYPE, 50, union kbase_ioctl_get_cpu_gpu_timeinfo)



struct kbase_ioctl_context_priority_check {
	__u8 priority;
};

#define KBASE_IOCTL_CONTEXT_PRIORITY_CHECK \
	_IOWR(KBASE_IOCTL_TYPE, 54, struct kbase_ioctl_context_priority_check)


struct kbase_ioctl_set_limited_core_count {
	__u8 max_core_count;
};

#define KBASE_IOCTL_SET_LIMITED_CORE_COUNT \
	_IOW(KBASE_IOCTL_TYPE, 55, struct kbase_ioctl_set_limited_core_count)


struct kbase_ioctl_kinstr_prfcnt_enum_info {
	__u32 info_item_size;
	__u32 info_item_count;
	__u64 info_list_ptr;
};

#define KBASE_IOCTL_KINSTR_PRFCNT_ENUM_INFO                                    \
	_IOWR(KBASE_IOCTL_TYPE, 56, struct kbase_ioctl_kinstr_prfcnt_enum_info)


union kbase_ioctl_kinstr_prfcnt_setup {
	struct {
		__u32 request_item_count;
		__u32 request_item_size;
		__u64 requests_ptr;
	} in;
	struct {
		__u32 prfcnt_metadata_item_size;
		__u32 prfcnt_mmap_size_bytes;
	} out;
};

#define KBASE_IOCTL_KINSTR_PRFCNT_SETUP                                        \
	_IOWR(KBASE_IOCTL_TYPE, 57, union kbase_ioctl_kinstr_prfcnt_setup)


#if MALI_UNIT_TEST


#define KBASE_IOCTL_TEST_TYPE (KBASE_IOCTL_TYPE + 1)



struct kbase_ioctl_tlstream_stats {
	__u32 bytes_collected;
	__u32 bytes_generated;
};

#define KBASE_IOCTL_TLSTREAM_STATS \
	_IOR(KBASE_IOCTL_TEST_TYPE, 2, struct kbase_ioctl_tlstream_stats)

#endif 


#define KBASE_IOCTL_EXTRA_TYPE (KBASE_IOCTL_TYPE + 2)





#define KBASE_GPUPROP_VALUE_SIZE_U8	(0x0)
#define KBASE_GPUPROP_VALUE_SIZE_U16	(0x1)
#define KBASE_GPUPROP_VALUE_SIZE_U32	(0x2)
#define KBASE_GPUPROP_VALUE_SIZE_U64	(0x3)

#define KBASE_GPUPROP_PRODUCT_ID			1
#define KBASE_GPUPROP_VERSION_STATUS			2
#define KBASE_GPUPROP_MINOR_REVISION			3
#define KBASE_GPUPROP_MAJOR_REVISION			4

#define KBASE_GPUPROP_GPU_FREQ_KHZ_MAX			6

#define KBASE_GPUPROP_LOG2_PROGRAM_COUNTER_SIZE		8
#define KBASE_GPUPROP_TEXTURE_FEATURES_0		9
#define KBASE_GPUPROP_TEXTURE_FEATURES_1		10
#define KBASE_GPUPROP_TEXTURE_FEATURES_2		11
#define KBASE_GPUPROP_GPU_AVAILABLE_MEMORY_SIZE		12

#define KBASE_GPUPROP_L2_LOG2_LINE_SIZE			13
#define KBASE_GPUPROP_L2_LOG2_CACHE_SIZE		14
#define KBASE_GPUPROP_L2_NUM_L2_SLICES			15

#define KBASE_GPUPROP_TILER_BIN_SIZE_BYTES		16
#define KBASE_GPUPROP_TILER_MAX_ACTIVE_LEVELS		17

#define KBASE_GPUPROP_MAX_THREADS			18
#define KBASE_GPUPROP_MAX_WORKGROUP_SIZE		19
#define KBASE_GPUPROP_MAX_BARRIER_SIZE			20
#define KBASE_GPUPROP_MAX_REGISTERS			21
#define KBASE_GPUPROP_MAX_TASK_QUEUE			22
#define KBASE_GPUPROP_MAX_THREAD_GROUP_SPLIT		23
#define KBASE_GPUPROP_IMPL_TECH				24

#define KBASE_GPUPROP_RAW_SHADER_PRESENT		25
#define KBASE_GPUPROP_RAW_TILER_PRESENT			26
#define KBASE_GPUPROP_RAW_L2_PRESENT			27
#define KBASE_GPUPROP_RAW_STACK_PRESENT			28
#define KBASE_GPUPROP_RAW_L2_FEATURES			29
#define KBASE_GPUPROP_RAW_CORE_FEATURES			30
#define KBASE_GPUPROP_RAW_MEM_FEATURES			31
#define KBASE_GPUPROP_RAW_MMU_FEATURES			32
#define KBASE_GPUPROP_RAW_AS_PRESENT			33
#define KBASE_GPUPROP_RAW_JS_PRESENT			34
#define KBASE_GPUPROP_RAW_JS_FEATURES_0			35
#define KBASE_GPUPROP_RAW_JS_FEATURES_1			36
#define KBASE_GPUPROP_RAW_JS_FEATURES_2			37
#define KBASE_GPUPROP_RAW_JS_FEATURES_3			38
#define KBASE_GPUPROP_RAW_JS_FEATURES_4			39
#define KBASE_GPUPROP_RAW_JS_FEATURES_5			40
#define KBASE_GPUPROP_RAW_JS_FEATURES_6			41
#define KBASE_GPUPROP_RAW_JS_FEATURES_7			42
#define KBASE_GPUPROP_RAW_JS_FEATURES_8			43
#define KBASE_GPUPROP_RAW_JS_FEATURES_9			44
#define KBASE_GPUPROP_RAW_JS_FEATURES_10		45
#define KBASE_GPUPROP_RAW_JS_FEATURES_11		46
#define KBASE_GPUPROP_RAW_JS_FEATURES_12		47
#define KBASE_GPUPROP_RAW_JS_FEATURES_13		48
#define KBASE_GPUPROP_RAW_JS_FEATURES_14		49
#define KBASE_GPUPROP_RAW_JS_FEATURES_15		50
#define KBASE_GPUPROP_RAW_TILER_FEATURES		51
#define KBASE_GPUPROP_RAW_TEXTURE_FEATURES_0		52
#define KBASE_GPUPROP_RAW_TEXTURE_FEATURES_1		53
#define KBASE_GPUPROP_RAW_TEXTURE_FEATURES_2		54
#define KBASE_GPUPROP_RAW_GPU_ID			55
#define KBASE_GPUPROP_RAW_THREAD_MAX_THREADS		56
#define KBASE_GPUPROP_RAW_THREAD_MAX_WORKGROUP_SIZE	57
#define KBASE_GPUPROP_RAW_THREAD_MAX_BARRIER_SIZE	58
#define KBASE_GPUPROP_RAW_THREAD_FEATURES		59
#define KBASE_GPUPROP_RAW_COHERENCY_MODE		60

#define KBASE_GPUPROP_COHERENCY_NUM_GROUPS		61
#define KBASE_GPUPROP_COHERENCY_NUM_CORE_GROUPS		62
#define KBASE_GPUPROP_COHERENCY_COHERENCY		63
#define KBASE_GPUPROP_COHERENCY_GROUP_0			64
#define KBASE_GPUPROP_COHERENCY_GROUP_1			65
#define KBASE_GPUPROP_COHERENCY_GROUP_2			66
#define KBASE_GPUPROP_COHERENCY_GROUP_3			67
#define KBASE_GPUPROP_COHERENCY_GROUP_4			68
#define KBASE_GPUPROP_COHERENCY_GROUP_5			69
#define KBASE_GPUPROP_COHERENCY_GROUP_6			70
#define KBASE_GPUPROP_COHERENCY_GROUP_7			71
#define KBASE_GPUPROP_COHERENCY_GROUP_8			72
#define KBASE_GPUPROP_COHERENCY_GROUP_9			73
#define KBASE_GPUPROP_COHERENCY_GROUP_10		74
#define KBASE_GPUPROP_COHERENCY_GROUP_11		75
#define KBASE_GPUPROP_COHERENCY_GROUP_12		76
#define KBASE_GPUPROP_COHERENCY_GROUP_13		77
#define KBASE_GPUPROP_COHERENCY_GROUP_14		78
#define KBASE_GPUPROP_COHERENCY_GROUP_15		79

#define KBASE_GPUPROP_TEXTURE_FEATURES_3		80
#define KBASE_GPUPROP_RAW_TEXTURE_FEATURES_3		81

#define KBASE_GPUPROP_NUM_EXEC_ENGINES			82

#define KBASE_GPUPROP_RAW_THREAD_TLS_ALLOC		83
#define KBASE_GPUPROP_TLS_ALLOC				84
#define KBASE_GPUPROP_RAW_GPU_FEATURES			85
#ifdef __cpluscplus
}
#endif

#endif 
