/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_HWCNT_READER_H_
#define _UAPI_KBASE_HWCNT_READER_H_

#include <stddef.h>
#include <linux/types.h>


#define KBASE_HWCNT_READER 0xBE
#define KBASE_HWCNT_READER_GET_HWVER       _IOR(KBASE_HWCNT_READER, 0x00, __u32)
#define KBASE_HWCNT_READER_GET_BUFFER_SIZE _IOR(KBASE_HWCNT_READER, 0x01, __u32)
#define KBASE_HWCNT_READER_DUMP            _IOW(KBASE_HWCNT_READER, 0x10, __u32)
#define KBASE_HWCNT_READER_CLEAR           _IOW(KBASE_HWCNT_READER, 0x11, __u32)
#define KBASE_HWCNT_READER_GET_BUFFER      _IOC(_IOC_READ, KBASE_HWCNT_READER, 0x20,\
		offsetof(struct kbase_hwcnt_reader_metadata, cycles))
#define KBASE_HWCNT_READER_GET_BUFFER_WITH_CYCLES      _IOR(KBASE_HWCNT_READER, 0x20,\
		struct kbase_hwcnt_reader_metadata)
#define KBASE_HWCNT_READER_PUT_BUFFER      _IOC(_IOC_WRITE, KBASE_HWCNT_READER, 0x21,\
		offsetof(struct kbase_hwcnt_reader_metadata, cycles))
#define KBASE_HWCNT_READER_PUT_BUFFER_WITH_CYCLES      _IOW(KBASE_HWCNT_READER, 0x21,\
		struct kbase_hwcnt_reader_metadata)
#define KBASE_HWCNT_READER_SET_INTERVAL    _IOW(KBASE_HWCNT_READER, 0x30, __u32)
#define KBASE_HWCNT_READER_ENABLE_EVENT    _IOW(KBASE_HWCNT_READER, 0x40, __u32)
#define KBASE_HWCNT_READER_DISABLE_EVENT   _IOW(KBASE_HWCNT_READER, 0x41, __u32)
#define KBASE_HWCNT_READER_GET_API_VERSION _IOW(KBASE_HWCNT_READER, 0xFF, __u32)
#define KBASE_HWCNT_READER_GET_API_VERSION_WITH_FEATURES \
		_IOW(KBASE_HWCNT_READER, 0xFF, \
		     struct kbase_hwcnt_reader_api_version)


struct kbase_hwcnt_reader_metadata_cycles {
	__u64 top;
	__u64 shader_cores;
};


struct kbase_hwcnt_reader_metadata {
	__u64 timestamp;
	__u32 event_id;
	__u32 buffer_idx;
	struct kbase_hwcnt_reader_metadata_cycles cycles;
};


enum base_hwcnt_reader_event {
	BASE_HWCNT_READER_EVENT_MANUAL,
	BASE_HWCNT_READER_EVENT_PERIODIC,
	BASE_HWCNT_READER_EVENT_PREJOB,
	BASE_HWCNT_READER_EVENT_POSTJOB,
	BASE_HWCNT_READER_EVENT_COUNT
};

#define KBASE_HWCNT_READER_API_VERSION_NO_FEATURE (0)
#define KBASE_HWCNT_READER_API_VERSION_FEATURE_CYCLES_TOP (1 << 0)
#define KBASE_HWCNT_READER_API_VERSION_FEATURE_CYCLES_SHADER_CORES (1 << 1)

struct kbase_hwcnt_reader_api_version {
	__u32 version;
	__u32 features;
};

#endif 

