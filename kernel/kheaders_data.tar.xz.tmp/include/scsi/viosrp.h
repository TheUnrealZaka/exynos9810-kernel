



























#ifndef VIOSRP_H
#define VIOSRP_H
#include <scsi/srp.h>

#define SRP_VERSION "16.a"
#define SRP_MAX_IU_LEN	256
#define SRP_MAX_LOC_LEN 32

union srp_iu {
	struct srp_login_req login_req;
	struct srp_login_rsp login_rsp;
	struct srp_login_rej login_rej;
	struct srp_i_logout i_logout;
	struct srp_t_logout t_logout;
	struct srp_tsk_mgmt tsk_mgmt;
	struct srp_cmd cmd;
	struct srp_rsp rsp;
	u8 reserved[SRP_MAX_IU_LEN];
};

enum viosrp_crq_headers {
	VIOSRP_CRQ_FREE = 0x00,
	VIOSRP_CRQ_CMD_RSP = 0x80,
	VIOSRP_CRQ_INIT_RSP = 0xC0,
	VIOSRP_CRQ_XPORT_EVENT = 0xFF
};

enum viosrp_crq_init_formats {
	VIOSRP_CRQ_INIT = 0x01,
	VIOSRP_CRQ_INIT_COMPLETE = 0x02
};

enum viosrp_crq_formats {
	VIOSRP_SRP_FORMAT = 0x01,
	VIOSRP_MAD_FORMAT = 0x02,
	VIOSRP_OS400_FORMAT = 0x03,
	VIOSRP_AIX_FORMAT = 0x04,
	VIOSRP_LINUX_FORMAT = 0x05,
	VIOSRP_INLINE_FORMAT = 0x06
};

enum viosrp_crq_status {
	VIOSRP_OK = 0x0,
	VIOSRP_NONRECOVERABLE_ERR = 0x1,
	VIOSRP_VIOLATES_MAX_XFER = 0x2,
	VIOSRP_PARTNER_PANIC = 0x3,
	VIOSRP_DEVICE_BUSY = 0x8,
	VIOSRP_ADAPTER_FAIL = 0x10,
	VIOSRP_OK2 = 0x99,
};

struct viosrp_crq {
	u8 valid;		
	u8 format;		
	u8 reserved;
	u8 status;		
	__be16 timeout;		
	__be16 IU_length;		
	__be64 IU_data_ptr;	
};


enum viosrp_mad_types {
	VIOSRP_EMPTY_IU_TYPE = 0x01,
	VIOSRP_ERROR_LOG_TYPE = 0x02,
	VIOSRP_ADAPTER_INFO_TYPE = 0x03,
	VIOSRP_CAPABILITIES_TYPE = 0x05,
	VIOSRP_ENABLE_FAST_FAIL = 0x08,
};

enum viosrp_mad_status {
	VIOSRP_MAD_SUCCESS = 0x00,
	VIOSRP_MAD_NOT_SUPPORTED = 0xF1,
	VIOSRP_MAD_FAILED = 0xF7,
};

enum viosrp_capability_type {
	MIGRATION_CAPABILITIES = 0x01,
	RESERVATION_CAPABILITIES = 0x02,
};

enum viosrp_capability_support {
	SERVER_DOES_NOT_SUPPORTS_CAP = 0x0,
	SERVER_SUPPORTS_CAP = 0x01,
	SERVER_CAP_DATA = 0x02,
};

enum viosrp_reserve_type {
	CLIENT_RESERVE_SCSI_2 = 0x01,
};

enum viosrp_capability_flag {
	CLIENT_MIGRATED = 0x01,
	CLIENT_RECONNECT = 0x02,
	CAP_LIST_SUPPORTED = 0x04,
	CAP_LIST_DATA = 0x08,
};


struct mad_common {
	__be32 type;
	__be16 status;
	__be16 length;
	__be64 tag;
};


struct viosrp_empty_iu {
	struct mad_common common;
	__be64 buffer;
	__be32 port;
};

struct viosrp_error_log {
	struct mad_common common;
	__be64 buffer;
};

struct viosrp_adapter_info {
	struct mad_common common;
	__be64 buffer;
};

struct viosrp_fast_fail {
	struct mad_common common;
};

struct viosrp_capabilities {
	struct mad_common common;
	__be64 buffer;
};

struct mad_capability_common {
	__be32 cap_type;
	__be16 length;
	__be16 server_support;
};

struct mad_reserve_cap {
	struct mad_capability_common common;
	__be32 type;
};

struct mad_migration_cap {
	struct mad_capability_common common;
	__be32 ecl;
};

struct capabilities {
	__be32 flags;
	char name[SRP_MAX_LOC_LEN];
	char loc[SRP_MAX_LOC_LEN];
	struct mad_migration_cap migration;
	struct mad_reserve_cap reserve;
};

union mad_iu {
	struct viosrp_empty_iu empty_iu;
	struct viosrp_error_log error_log;
	struct viosrp_adapter_info adapter_info;
	struct viosrp_fast_fail fast_fail;
	struct viosrp_capabilities capabilities;
};

union viosrp_iu {
	union srp_iu srp;
	union mad_iu mad;
};

struct mad_adapter_info_data {
	char srp_version[8];
	char partition_name[96];
	__be32 partition_number;
#define SRP_MAD_VERSION_1 1
	__be32 mad_version;
#define SRP_MAD_OS_LINUX 2
#define SRP_MAD_OS_AIX 3
	__be32 os_type;
	__be32 port_max_txu[8];	
};

#endif
