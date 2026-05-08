/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_BASE_COMMON_KERNEL_H_
#define _UAPI_BASE_COMMON_KERNEL_H_

#include <linux/types.h>

struct base_mem_handle {
	struct {
		__u64 handle;
	} basep;
};

#define BASE_GPU_NUM_TEXTURE_FEATURES_REGISTERS 4





#define BASE_MEM_PROT_CPU_RD ((base_mem_alloc_flags)1 << 0)


#define BASE_MEM_PROT_CPU_WR ((base_mem_alloc_flags)1 << 1)


#define BASE_MEM_PROT_GPU_RD ((base_mem_alloc_flags)1 << 2)


#define BASE_MEM_PROT_GPU_WR ((base_mem_alloc_flags)1 << 3)


#define BASE_MEM_PROT_GPU_EX ((base_mem_alloc_flags)1 << 4)


#define BASEP_MEM_PERMANENT_KERNEL_MAPPING ((base_mem_alloc_flags)1 << 5)


#define BASE_MEM_GPU_VA_SAME_4GB_PAGE ((base_mem_alloc_flags)1 << 6)


#define BASEP_MEM_NO_USER_FREE ((base_mem_alloc_flags)1 << 7)


#define BASE_MEM_GROW_ON_GPF ((base_mem_alloc_flags)1 << 9)


#define BASE_MEM_COHERENT_SYSTEM ((base_mem_alloc_flags)1 << 10)


#define BASE_MEM_COHERENT_LOCAL ((base_mem_alloc_flags)1 << 11)



#define BASE_MEM_CACHED_CPU ((base_mem_alloc_flags)1 << 12)



#define BASE_MEM_SAME_VA ((base_mem_alloc_flags)1 << 13)



#define BASE_MEM_NEED_MMAP ((base_mem_alloc_flags)1 << 14)



#define BASE_MEM_COHERENT_SYSTEM_REQUIRED ((base_mem_alloc_flags)1 << 15)


#define BASE_MEM_PROTECTED ((base_mem_alloc_flags)1 << 16)


#define BASE_MEM_DONT_NEED ((base_mem_alloc_flags)1 << 17)


#define BASE_MEM_IMPORT_SHARED ((base_mem_alloc_flags)1 << 18)


#define BASE_MEM_UNCACHED_GPU ((base_mem_alloc_flags)1 << 21)


#define BASEP_MEM_GROUP_ID_SHIFT 22
#define BASE_MEM_GROUP_ID_MASK ((base_mem_alloc_flags)0xF << BASEP_MEM_GROUP_ID_SHIFT)


#define BASE_MEM_IMPORT_SYNC_ON_MAP_UNMAP ((base_mem_alloc_flags)1 << 26)



#define BASE_MEM_KERNEL_SYNC ((base_mem_alloc_flags)1 << 28)


#define BASE_MEM_FLAGS_NR_BITS 30


#define BASE_MEM_FLAGS_OUTPUT_MASK BASE_MEM_NEED_MMAP


#define BASE_MEM_FLAGS_INPUT_MASK                                                                  \
	(((1 << BASE_MEM_FLAGS_NR_BITS) - 1) & ~BASE_MEM_FLAGS_OUTPUT_MASK)


#define BASEP_MEM_INVALID_HANDLE (0ul)
#define BASE_MEM_MMU_DUMP_HANDLE (1ul << LOCAL_PAGE_SHIFT)
#define BASE_MEM_TRACE_BUFFER_HANDLE (2ul << LOCAL_PAGE_SHIFT)
#define BASE_MEM_MAP_TRACKING_HANDLE (3ul << LOCAL_PAGE_SHIFT)
#define BASEP_MEM_WRITE_ALLOC_PAGES_HANDLE (4ul << LOCAL_PAGE_SHIFT)

#define BASE_MEM_COOKIE_BASE (64ul << LOCAL_PAGE_SHIFT)
#define BASE_MEM_FIRST_FREE_ADDRESS ((BITS_PER_LONG << LOCAL_PAGE_SHIFT) + BASE_MEM_COOKIE_BASE)


typedef __u32 base_context_create_flags;




#define BASE_CONTEXT_CREATE_FLAG_NONE ((base_context_create_flags)0)


#define BASE_CONTEXT_CCTX_EMBEDDED ((base_context_create_flags)1 << 0)


#define BASE_CONTEXT_SYSTEM_MONITOR_SUBMIT_DISABLED ((base_context_create_flags)1 << 1)


#define BASEP_CONTEXT_MMU_GROUP_ID_SHIFT (3)


#define BASEP_CONTEXT_MMU_GROUP_ID_MASK                                                            \
	((base_context_create_flags)0xF << BASEP_CONTEXT_MMU_GROUP_ID_SHIFT)


#define BASEP_CONTEXT_CREATE_KERNEL_FLAGS                                                          \
	(BASE_CONTEXT_SYSTEM_MONITOR_SUBMIT_DISABLED | BASEP_CONTEXT_MMU_GROUP_ID_MASK)




#define BASE_TLSTREAM_ENABLE_LATENCY_TRACEPOINTS (1 << 0)


#define BASE_TLSTREAM_JOB_DUMPING_ENABLED (1 << 1)

#endif 
