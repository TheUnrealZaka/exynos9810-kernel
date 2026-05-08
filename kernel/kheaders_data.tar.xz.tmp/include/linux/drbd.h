
#ifndef DRBD_H
#define DRBD_H
#include <asm/types.h>

#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/byteorder.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>



#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN_BITFIELD
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN_BITFIELD
#else
# error "sorry, weird endianness on this box"
#endif

#endif

extern const char *drbd_buildtag(void);
#define REL_VERSION "8.4.7"
#define API_VERSION 1
#define PRO_VERSION_MIN 86
#define PRO_VERSION_MAX 101


enum drbd_io_error_p {
	EP_PASS_ON, 
	EP_CALL_HELPER,
	EP_DETACH
};

enum drbd_fencing_p {
	FP_NOT_AVAIL = -1, 
	FP_DONT_CARE = 0,
	FP_RESOURCE,
	FP_STONITH
};

enum drbd_disconnect_p {
	DP_RECONNECT,
	DP_DROP_NET_CONF,
	DP_FREEZE_IO
};

enum drbd_after_sb_p {
	ASB_DISCONNECT,
	ASB_DISCARD_YOUNGER_PRI,
	ASB_DISCARD_OLDER_PRI,
	ASB_DISCARD_ZERO_CHG,
	ASB_DISCARD_LEAST_CHG,
	ASB_DISCARD_LOCAL,
	ASB_DISCARD_REMOTE,
	ASB_CONSENSUS,
	ASB_DISCARD_SECONDARY,
	ASB_CALL_HELPER,
	ASB_VIOLENTLY
};

enum drbd_on_no_data {
	OND_IO_ERROR,
	OND_SUSPEND_IO
};

enum drbd_on_congestion {
	OC_BLOCK,
	OC_PULL_AHEAD,
	OC_DISCONNECT,
};

enum drbd_read_balancing {
	RB_PREFER_LOCAL,
	RB_PREFER_REMOTE,
	RB_ROUND_ROBIN,
	RB_LEAST_PENDING,
	RB_CONGESTED_REMOTE,
	RB_32K_STRIPING,
	RB_64K_STRIPING,
	RB_128K_STRIPING,
	RB_256K_STRIPING,
	RB_512K_STRIPING,
	RB_1M_STRIPING,
};


enum drbd_ret_code {
	ERR_CODE_BASE		= 100,
	NO_ERROR		= 101,
	ERR_LOCAL_ADDR		= 102,
	ERR_PEER_ADDR		= 103,
	ERR_OPEN_DISK		= 104,
	ERR_OPEN_MD_DISK	= 105,
	ERR_DISK_NOT_BDEV	= 107,
	ERR_MD_NOT_BDEV		= 108,
	ERR_DISK_TOO_SMALL	= 111,
	ERR_MD_DISK_TOO_SMALL	= 112,
	ERR_BDCLAIM_DISK	= 114,
	ERR_BDCLAIM_MD_DISK	= 115,
	ERR_MD_IDX_INVALID	= 116,
	ERR_IO_MD_DISK		= 118,
	ERR_MD_INVALID          = 119,
	ERR_AUTH_ALG		= 120,
	ERR_AUTH_ALG_ND		= 121,
	ERR_NOMEM		= 122,
	ERR_DISCARD_IMPOSSIBLE	= 123,
	ERR_DISK_CONFIGURED	= 124,
	ERR_NET_CONFIGURED	= 125,
	ERR_MANDATORY_TAG	= 126,
	ERR_MINOR_INVALID	= 127,
	ERR_INTR		= 129, 
	ERR_RESIZE_RESYNC	= 130,
	ERR_NO_PRIMARY		= 131,
	ERR_RESYNC_AFTER	= 132,
	ERR_RESYNC_AFTER_CYCLE	= 133,
	ERR_PAUSE_IS_SET	= 134,
	ERR_PAUSE_IS_CLEAR	= 135,
	ERR_PACKET_NR		= 137,
	ERR_NO_DISK		= 138,
	ERR_NOT_PROTO_C		= 139,
	ERR_NOMEM_BITMAP	= 140,
	ERR_INTEGRITY_ALG	= 141, 
	ERR_INTEGRITY_ALG_ND	= 142, 
	ERR_CPU_MASK_PARSE	= 143, 
	ERR_CSUMS_ALG		= 144, 
	ERR_CSUMS_ALG_ND	= 145, 
	ERR_VERIFY_ALG		= 146, 
	ERR_VERIFY_ALG_ND	= 147, 
	ERR_CSUMS_RESYNC_RUNNING= 148, 
	ERR_VERIFY_RUNNING	= 149, 
	ERR_DATA_NOT_CURRENT	= 150,
	ERR_CONNECTED		= 151, 
	ERR_PERM		= 152,
	ERR_NEED_APV_93		= 153,
	ERR_STONITH_AND_PROT_A  = 154,
	ERR_CONG_NOT_PROTO_A	= 155,
	ERR_PIC_AFTER_DEP	= 156,
	ERR_PIC_PEER_DEP	= 157,
	ERR_RES_NOT_KNOWN	= 158,
	ERR_RES_IN_USE		= 159,
	ERR_MINOR_CONFIGURED    = 160,
	ERR_MINOR_OR_VOLUME_EXISTS = 161,
	ERR_INVALID_REQUEST	= 162,
	ERR_NEED_APV_100	= 163,
	ERR_NEED_ALLOW_TWO_PRI  = 164,
	ERR_MD_UNCLEAN          = 165,
	ERR_MD_LAYOUT_CONNECTED = 166,
	ERR_MD_LAYOUT_TOO_BIG   = 167,
	ERR_MD_LAYOUT_TOO_SMALL = 168,
	ERR_MD_LAYOUT_NO_FIT    = 169,
	ERR_IMPLICIT_SHRINK     = 170,
	
	AFTER_LAST_ERR_CODE
};

#define DRBD_PROT_A   1
#define DRBD_PROT_B   2
#define DRBD_PROT_C   3

enum drbd_role {
	R_UNKNOWN = 0,
	R_PRIMARY = 1,     
	R_SECONDARY = 2,   
	R_MASK = 3,
};


enum drbd_conns {
	C_STANDALONE,
	C_DISCONNECTING,  
	C_UNCONNECTED,    

	
	C_TIMEOUT,
	C_BROKEN_PIPE,
	C_NETWORK_FAILURE,
	C_PROTOCOL_ERROR,
	C_TEAR_DOWN,

	C_WF_CONNECTION,
	C_WF_REPORT_PARAMS, 
	C_CONNECTED,      
	C_STARTING_SYNC_S,  
	C_STARTING_SYNC_T,  
	C_WF_BITMAP_S,
	C_WF_BITMAP_T,
	C_WF_SYNC_UUID,

	
	C_SYNC_SOURCE,
	C_SYNC_TARGET,
	C_VERIFY_S,
	C_VERIFY_T,
	C_PAUSED_SYNC_S,
	C_PAUSED_SYNC_T,

	C_AHEAD,
	C_BEHIND,

	C_MASK = 31
};

enum drbd_disk_state {
	D_DISKLESS,
	D_ATTACHING,      
	D_FAILED,         
			  
	D_NEGOTIATING,    
	D_INCONSISTENT,
	D_OUTDATED,
	D_UNKNOWN,       
	D_CONSISTENT,     
	D_UP_TO_DATE,       
	D_MASK = 15
};

union drbd_state {

	struct {
#if defined(__LITTLE_ENDIAN_BITFIELD)
		unsigned role:2 ;   
		unsigned peer:2 ;   
		unsigned conn:5 ;   
		unsigned disk:4 ;   
		unsigned pdsk:4 ;   
		unsigned susp:1 ;   
		unsigned aftr_isp:1 ; 
		unsigned peer_isp:1 ;
		unsigned user_isp:1 ;
		unsigned susp_nod:1 ; 
		unsigned susp_fen:1 ; 
		unsigned _pad:9;   
#elif defined(__BIG_ENDIAN_BITFIELD)
		unsigned _pad:9;
		unsigned susp_fen:1 ;
		unsigned susp_nod:1 ;
		unsigned user_isp:1 ;
		unsigned peer_isp:1 ;
		unsigned aftr_isp:1 ; 
		unsigned susp:1 ;   
		unsigned pdsk:4 ;   
		unsigned disk:4 ;   
		unsigned conn:5 ;   
		unsigned peer:2 ;   
		unsigned role:2 ;   
#else
# error "this endianness is not supported"
#endif
	};
	unsigned int i;
};

enum drbd_state_rv {
	SS_CW_NO_NEED = 4,
	SS_CW_SUCCESS = 3,
	SS_NOTHING_TO_DO = 2,
	SS_SUCCESS = 1,
	SS_UNKNOWN_ERROR = 0, 
	SS_TWO_PRIMARIES = -1,
	SS_NO_UP_TO_DATE_DISK = -2,
	SS_NO_LOCAL_DISK = -4,
	SS_NO_REMOTE_DISK = -5,
	SS_CONNECTED_OUTDATES = -6,
	SS_PRIMARY_NOP = -7,
	SS_RESYNC_RUNNING = -8,
	SS_ALREADY_STANDALONE = -9,
	SS_CW_FAILED_BY_PEER = -10,
	SS_IS_DISKLESS = -11,
	SS_DEVICE_IN_USE = -12,
	SS_NO_NET_CONFIG = -13,
	SS_NO_VERIFY_ALG = -14,       
	SS_NEED_CONNECTION = -15,    
	SS_LOWER_THAN_OUTDATED = -16,
	SS_NOT_SUPPORTED = -17,      
	SS_IN_TRANSIENT_STATE = -18,  
	SS_CONCURRENT_ST_CHG = -19,   
	SS_O_VOL_PEER_PRI = -20,
	SS_OUTDATE_WO_CONN = -21,
	SS_AFTER_LAST_ERROR = -22,    
};

#define SHARED_SECRET_MAX 64

#define MDF_CONSISTENT		(1 << 0)
#define MDF_PRIMARY_IND		(1 << 1)
#define MDF_CONNECTED_IND	(1 << 2)
#define MDF_FULL_SYNC		(1 << 3)
#define MDF_WAS_UP_TO_DATE	(1 << 4)
#define MDF_PEER_OUT_DATED	(1 << 5)
#define MDF_CRASHED_PRIMARY	(1 << 6)
#define MDF_AL_CLEAN		(1 << 7)
#define MDF_AL_DISABLED		(1 << 8)

#define MAX_PEERS 32

enum drbd_uuid_index {
	UI_CURRENT,
	UI_BITMAP,
	UI_HISTORY_START,
	UI_HISTORY_END,
	UI_SIZE,      
	UI_FLAGS,     
	UI_EXTENDED_SIZE   
};

#define HISTORY_UUIDS MAX_PEERS

enum drbd_timeout_flag {
	UT_DEFAULT      = 0,
	UT_DEGRADED     = 1,
	UT_PEER_OUTDATED = 2,
};

enum drbd_notification_type {
	NOTIFY_EXISTS,
	NOTIFY_CREATE,
	NOTIFY_CHANGE,
	NOTIFY_DESTROY,
	NOTIFY_CALL,
	NOTIFY_RESPONSE,

	NOTIFY_CONTINUES = 0x8000,
	NOTIFY_FLAGS = NOTIFY_CONTINUES,
};

enum drbd_peer_state {
	P_INCONSISTENT = 3,
	P_OUTDATED = 4,
	P_DOWN = 5,
	P_PRIMARY = 6,
	P_FENCING = 7,
};

#define UUID_JUST_CREATED ((__u64)4)

enum write_ordering_e {
	WO_NONE,
	WO_DRAIN_IO,
	WO_BDEV_FLUSH,
	WO_BIO_BARRIER
};


#define DRBD_MAGIC 0x83740267
#define DRBD_MAGIC_BIG 0x835a
#define DRBD_MAGIC_100 0x8620ec20

#define DRBD_MD_MAGIC_07   (DRBD_MAGIC+3)
#define DRBD_MD_MAGIC_08   (DRBD_MAGIC+4)
#define DRBD_MD_MAGIC_84_UNCLEAN	(DRBD_MAGIC+5)



#define DRBD_AL_MAGIC 0x69cb65a2


#define DRBD_MD_INDEX_INTERNAL -1
#define DRBD_MD_INDEX_FLEX_EXT -2
#define DRBD_MD_INDEX_FLEX_INT -3

#define DRBD_CPU_MASK_SIZE 32

#endif
