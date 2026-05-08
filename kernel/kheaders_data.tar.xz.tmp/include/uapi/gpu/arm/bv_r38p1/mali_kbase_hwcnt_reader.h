/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_HWCNT_READER_H_
#define _UAPI_KBASE_HWCNT_READER_H_

#include <linux/stddef.h>
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


#define PRFCNT_READER_API_VERSION (0)


enum prfcnt_list_type {
	PRFCNT_LIST_TYPE_ENUM,
	PRFCNT_LIST_TYPE_REQUEST,
	PRFCNT_LIST_TYPE_SAMPLE_META,
};

#define FLEX_LIST_TYPE(type, subtype)                                          \
	((__u16)(((type & 0xf) << 12) | (subtype & 0xfff)))
#define FLEX_LIST_TYPE_NONE FLEX_LIST_TYPE(0, 0)

#define PRFCNT_ENUM_TYPE_BLOCK FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_ENUM, 0)
#define PRFCNT_ENUM_TYPE_REQUEST FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_ENUM, 1)
#define PRFCNT_ENUM_TYPE_SAMPLE_INFO FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_ENUM, 2)

#define PRFCNT_REQUEST_TYPE_MODE FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_REQUEST, 0)
#define PRFCNT_REQUEST_TYPE_ENABLE FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_REQUEST, 1)
#define PRFCNT_REQUEST_TYPE_SCOPE FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_REQUEST, 2)

#define PRFCNT_SAMPLE_META_TYPE_SAMPLE                                         \
	FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_SAMPLE_META, 0)
#define PRFCNT_SAMPLE_META_TYPE_CLOCK                                          \
	FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_SAMPLE_META, 1)
#define PRFCNT_SAMPLE_META_TYPE_BLOCK                                          \
	FLEX_LIST_TYPE(PRFCNT_LIST_TYPE_SAMPLE_META, 2)


struct prfcnt_item_header {
	__u16 item_type;
	__u16 item_version;
};


enum prfcnt_block_type {
	PRFCNT_BLOCK_TYPE_FE,
	PRFCNT_BLOCK_TYPE_TILER,
	PRFCNT_BLOCK_TYPE_MEMORY,
	PRFCNT_BLOCK_TYPE_SHADER_CORE,
	PRFCNT_BLOCK_TYPE_RESERVED = 255,
};


enum prfcnt_set {
	PRFCNT_SET_PRIMARY,
	PRFCNT_SET_SECONDARY,
	PRFCNT_SET_TERTIARY,
	PRFCNT_SET_RESERVED = 255,
};


struct prfcnt_enum_block_counter {
	__u8 block_type;
	__u8 set;
	__u8 pad[2];
	__u16 num_instances;
	__u16 num_values;
	__u64 counter_mask[2];
};


struct prfcnt_enum_request {
	__u16 request_item_type;
	__u16 pad;
	__u32 versions_mask;
};


struct prfcnt_enum_sample_info {
	__u32 num_clock_domains;
	__u32 pad;
};


struct prfcnt_enum_item {
	struct prfcnt_item_header hdr;
	
	union {
		struct prfcnt_enum_block_counter block_counter;
		struct prfcnt_enum_request request;
		struct prfcnt_enum_sample_info sample_info;
	} u;
};


enum prfcnt_mode {
	PRFCNT_MODE_MANUAL,
	PRFCNT_MODE_PERIODIC,
	PRFCNT_MODE_RESERVED = 255,
};


struct prfcnt_request_mode {
	__u8 mode;
	__u8 pad[7];
	
	union {
		struct {
			__u64 period_ns;
		} periodic;
	} mode_config;
};


struct prfcnt_request_enable {
	__u8 block_type;
	__u8 set;
	__u8 pad[6];
	__u64 enable_mask[2];
};


enum prfcnt_scope {
	PRFCNT_SCOPE_GLOBAL,
	PRFCNT_SCOPE_RESERVED = 255,
};


struct prfcnt_request_scope {
	__u8 scope;
	__u8 pad[7];
};


struct prfcnt_request_item {
	struct prfcnt_item_header hdr;
	
	union {
		struct prfcnt_request_mode req_mode;
		struct prfcnt_request_enable req_enable;
		struct prfcnt_request_scope req_scope;
	} u;
};


enum prfcnt_request_type {
	PRFCNT_REQUEST_MODE,
	PRFCNT_REQUEST_ENABLE,
	PRFCNT_REQUEST_SCOPE,
};


#define SAMPLE_FLAG_OVERFLOW (1u << 0)

#define SAMPLE_FLAG_ERROR (1u << 30)


struct prfcnt_sample_metadata {
	__u64 timestamp_start;
	__u64 timestamp_end;
	__u64 seq;
	__u64 user_data;
	__u32 flags;
	__u32 pad;
};


#define MAX_REPORTED_DOMAINS (4)


struct prfcnt_clock_metadata {
	__u32 num_domains;
	__u32 pad;
	__u64 cycles[MAX_REPORTED_DOMAINS];
};


#define BLOCK_STATE_UNKNOWN (0)

#define BLOCK_STATE_ON (1 << 0)

#define BLOCK_STATE_OFF (1 << 1)

#define BLOCK_STATE_AVAILABLE (1 << 2)

#define BLOCK_STATE_UNAVAILABLE (1 << 3)

#define BLOCK_STATE_NORMAL (1 << 4)

#define BLOCK_STATE_PROTECTED (1 << 5)


struct prfcnt_block_metadata {
	__u8 block_type;
	__u8 block_idx;
	__u8 set;
	__u8 pad_u8;
	__u32 block_state;
	__u32 values_offset;
	__u32 pad_u32;
};


struct prfcnt_metadata {
	struct prfcnt_item_header hdr;
	union {
		struct prfcnt_sample_metadata sample_md;
		struct prfcnt_clock_metadata clock_md;
		struct prfcnt_block_metadata block_md;
	} u;
};


enum prfcnt_control_cmd_code {
	PRFCNT_CONTROL_CMD_START = 1,
	PRFCNT_CONTROL_CMD_STOP,
	PRFCNT_CONTROL_CMD_SAMPLE_SYNC,
	PRFCNT_CONTROL_CMD_SAMPLE_ASYNC,
	PRFCNT_CONTROL_CMD_DISCARD,
};


struct prfcnt_control_cmd {
	__u16 cmd;
	__u16 pad[3];
	__u64 user_data;
};


struct prfcnt_sample_access {
	__u64 sequence;
	__u64 sample_offset_bytes;
};


#define KBASE_KINSTR_PRFCNT_READER 0xBF

#define KBASE_IOCTL_KINSTR_PRFCNT_CMD                                          \
	_IOW(KBASE_KINSTR_PRFCNT_READER, 0x00, struct prfcnt_control_cmd)

#define KBASE_IOCTL_KINSTR_PRFCNT_GET_SAMPLE                                   \
	_IOR(KBASE_KINSTR_PRFCNT_READER, 0x01, struct prfcnt_sample_access)

#define KBASE_IOCTL_KINSTR_PRFCNT_PUT_SAMPLE                                   \
	_IOW(KBASE_KINSTR_PRFCNT_READER, 0x10, struct prfcnt_sample_access)

#endif 
