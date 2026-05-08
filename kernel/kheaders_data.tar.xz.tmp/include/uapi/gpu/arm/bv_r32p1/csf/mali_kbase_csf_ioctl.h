/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_CSF_IOCTL_H_
#define _UAPI_KBASE_CSF_IOCTL_H_

#include <asm-generic/ioctl.h>
#include <linux/types.h>



#define BASE_UK_VERSION_MAJOR 1
#define BASE_UK_VERSION_MINOR 5


struct kbase_ioctl_version_check {
	__u16 major;
	__u16 minor;
};

#define KBASE_IOCTL_VERSION_CHECK_RESERVED \
	_IOWR(KBASE_IOCTL_TYPE, 0, struct kbase_ioctl_version_check)



struct kbase_ioctl_cs_queue_register {
	__u64 buffer_gpu_addr;
	__u32 buffer_size;
	__u8 priority;
	__u8 padding[3];
};

#define KBASE_IOCTL_CS_QUEUE_REGISTER \
	_IOW(KBASE_IOCTL_TYPE, 36, struct kbase_ioctl_cs_queue_register)


struct kbase_ioctl_cs_queue_kick {
	__u64 buffer_gpu_addr;
};

#define KBASE_IOCTL_CS_QUEUE_KICK \
	_IOW(KBASE_IOCTL_TYPE, 37, struct kbase_ioctl_cs_queue_kick)


union kbase_ioctl_cs_queue_bind {
	struct {
		__u64 buffer_gpu_addr;
		__u8 group_handle;
		__u8 csi_index;
		__u8 padding[6];
	} in;
	struct {
		__u64 mmap_handle;
	} out;
};

#define KBASE_IOCTL_CS_QUEUE_BIND \
	_IOWR(KBASE_IOCTL_TYPE, 39, union kbase_ioctl_cs_queue_bind)


struct kbase_ioctl_cs_queue_register_ex {
	__u64 buffer_gpu_addr;
	__u32 buffer_size;
	__u8 priority;
	__u8 padding[3];
	__u64 ex_offset_var_addr;
	__u64 ex_buffer_base;
	__u32 ex_buffer_size;
	__u8 ex_event_size;
	__u8 ex_event_state;
	__u8 ex_padding[2];
};

#define KBASE_IOCTL_CS_QUEUE_REGISTER_EX \
	_IOW(KBASE_IOCTL_TYPE, 40, struct kbase_ioctl_cs_queue_register_ex)


struct kbase_ioctl_cs_queue_terminate {
	__u64 buffer_gpu_addr;
};

#define KBASE_IOCTL_CS_QUEUE_TERMINATE \
	_IOW(KBASE_IOCTL_TYPE, 41, struct kbase_ioctl_cs_queue_terminate)


union kbase_ioctl_cs_queue_group_create {
	struct {
		__u64 tiler_mask;
		__u64 fragment_mask;
		__u64 compute_mask;
		__u8 cs_min;
		__u8 priority;
		__u8 tiler_max;
		__u8 fragment_max;
		__u8 compute_max;
		__u8 padding[3];

	} in;
	struct {
		__u8 group_handle;
		__u8 padding[3];
		__u32 group_uid;
	} out;
};

#define KBASE_IOCTL_CS_QUEUE_GROUP_CREATE \
	_IOWR(KBASE_IOCTL_TYPE, 42, union kbase_ioctl_cs_queue_group_create)


struct kbase_ioctl_cs_queue_group_term {
	__u8 group_handle;
	__u8 padding[7];
};

#define KBASE_IOCTL_CS_QUEUE_GROUP_TERMINATE \
	_IOW(KBASE_IOCTL_TYPE, 43, struct kbase_ioctl_cs_queue_group_term)

#define KBASE_IOCTL_CS_EVENT_SIGNAL \
	_IO(KBASE_IOCTL_TYPE, 44)

typedef __u8 base_kcpu_queue_id; 


struct kbase_ioctl_kcpu_queue_new {
	base_kcpu_queue_id id;
	__u8 padding[7];
};

#define KBASE_IOCTL_KCPU_QUEUE_CREATE \
	_IOR(KBASE_IOCTL_TYPE, 45, struct kbase_ioctl_kcpu_queue_new)


struct kbase_ioctl_kcpu_queue_delete {
	base_kcpu_queue_id id;
	__u8 padding[7];
};

#define KBASE_IOCTL_KCPU_QUEUE_DELETE \
	_IOW(KBASE_IOCTL_TYPE, 46, struct kbase_ioctl_kcpu_queue_delete)


struct kbase_ioctl_kcpu_queue_enqueue {
	__u64 addr;
	__u32 nr_commands;
	base_kcpu_queue_id id;
	__u8 padding[3];
};

#define KBASE_IOCTL_KCPU_QUEUE_ENQUEUE \
	_IOW(KBASE_IOCTL_TYPE, 47, struct kbase_ioctl_kcpu_queue_enqueue)


union kbase_ioctl_cs_tiler_heap_init {
	struct {
		__u32 chunk_size;
		__u32 initial_chunks;
		__u32 max_chunks;
		__u16 target_in_flight;
		__u8 group_id;
		__u8 padding;
	} in;
	struct {
		__u64 gpu_heap_va;
		__u64 first_chunk_va;
	} out;
};

#define KBASE_IOCTL_CS_TILER_HEAP_INIT \
	_IOWR(KBASE_IOCTL_TYPE, 48, union kbase_ioctl_cs_tiler_heap_init)


struct kbase_ioctl_cs_tiler_heap_term {
	__u64 gpu_heap_va;
};

#define KBASE_IOCTL_CS_TILER_HEAP_TERM \
	_IOW(KBASE_IOCTL_TYPE, 49, struct kbase_ioctl_cs_tiler_heap_term)


union kbase_ioctl_cs_get_glb_iface {
	struct {
		__u32 max_group_num;
		__u32 max_total_stream_num;
		__u64 groups_ptr;
		__u64 streams_ptr;
	} in;
	struct {
		__u32 glb_version;
		__u32 features;
		__u32 group_num;
		__u32 prfcnt_size;
		__u32 total_stream_num;
		__u32 instr_features;
	} out;
};

#define KBASE_IOCTL_CS_GET_GLB_IFACE \
	_IOWR(KBASE_IOCTL_TYPE, 51, union kbase_ioctl_cs_get_glb_iface)

struct kbase_ioctl_cs_cpu_queue_info {
	__u64 buffer;
	__u64 size;
};

#define KBASE_IOCTL_VERSION_CHECK \
	_IOWR(KBASE_IOCTL_TYPE, 52, struct kbase_ioctl_version_check)

#define KBASE_IOCTL_CS_CPU_QUEUE_DUMP \
	_IOW(KBASE_IOCTL_TYPE, 53, struct kbase_ioctl_cs_cpu_queue_info)


#if MALI_UNIT_TEST



struct kbase_ioctl_cs_event_memory_write {
	__u64 cpu_addr;
	__u8 value;
	__u8 padding[7];
};


union kbase_ioctl_cs_event_memory_read {
	struct {
		__u64 cpu_addr;
	} in;
	struct {
		__u8 value;
		__u8 padding[7];
	} out;
};

#endif 

#endif 
