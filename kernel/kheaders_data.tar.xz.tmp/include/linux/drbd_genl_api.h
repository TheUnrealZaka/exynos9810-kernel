#ifndef DRBD_GENL_STRUCT_H
#define DRBD_GENL_STRUCT_H


struct drbd_genlmsghdr {
	__u32 minor;
	union {
	__u32 flags;
	__s32 ret_code;
	};
};


enum {
	DRBD_GENL_F_SET_DEFAULTS = 1,
};

enum drbd_state_info_bcast_reason {
	SIB_GET_STATUS_REPLY = 1,
	SIB_STATE_CHANGE = 2,
	SIB_HELPER_PRE = 3,
	SIB_HELPER_POST = 4,
	SIB_SYNC_PROGRESS = 5,
};


#undef linux

#include <linux/drbd.h>
#define GENL_MAGIC_VERSION	API_VERSION
#define GENL_MAGIC_FAMILY	drbd
#define GENL_MAGIC_FAMILY_HDRSZ	sizeof(struct drbd_genlmsghdr)
#define GENL_MAGIC_INCLUDE_FILE <linux/drbd_genl.h>
#include <linux/genl_magic_struct.h>

#endif
