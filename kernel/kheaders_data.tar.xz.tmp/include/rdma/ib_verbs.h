

#if !defined(IB_VERBS_H)
#define IB_VERBS_H

#include <linux/types.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/kref.h>
#include <linux/list.h>
#include <linux/rwsem.h>
#include <linux/scatterlist.h>
#include <linux/workqueue.h>
#include <linux/socket.h>
#include <linux/irq_poll.h>
#include <uapi/linux/if_ether.h>
#include <net/ipv6.h>
#include <net/ip.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/if_link.h>
#include <linux/atomic.h>
#include <linux/mmu_notifier.h>
#include <asm/uaccess.h>

extern struct workqueue_struct *ib_wq;
extern struct workqueue_struct *ib_comp_wq;
extern struct workqueue_struct *ib_comp_unbound_wq;

union ib_gid {
	u8	raw[16];
	struct {
		__be64	subnet_prefix;
		__be64	interface_id;
	} global;
};

extern union ib_gid zgid;

enum ib_gid_type {
	
	IB_GID_TYPE_IB        = 0,
	IB_GID_TYPE_ROCE      = 0,
	IB_GID_TYPE_ROCE_UDP_ENCAP = 1,
	IB_GID_TYPE_SIZE
};

#define ROCE_V2_UDP_DPORT      4791
struct ib_gid_attr {
	enum ib_gid_type	gid_type;
	struct net_device	*ndev;
};

enum rdma_node_type {
	
	RDMA_NODE_IB_CA 	= 1,
	RDMA_NODE_IB_SWITCH,
	RDMA_NODE_IB_ROUTER,
	RDMA_NODE_RNIC,
	RDMA_NODE_USNIC,
	RDMA_NODE_USNIC_UDP,
};

enum {
	
	IB_SA_WELL_KNOWN_GUID	= BIT_ULL(57) | 2,
};

enum rdma_transport_type {
	RDMA_TRANSPORT_IB,
	RDMA_TRANSPORT_IWARP,
	RDMA_TRANSPORT_USNIC,
	RDMA_TRANSPORT_USNIC_UDP
};

enum rdma_protocol_type {
	RDMA_PROTOCOL_IB,
	RDMA_PROTOCOL_IBOE,
	RDMA_PROTOCOL_IWARP,
	RDMA_PROTOCOL_USNIC_UDP
};

__attribute_const__ enum rdma_transport_type
rdma_node_get_transport(enum rdma_node_type node_type);

enum rdma_network_type {
	RDMA_NETWORK_IB,
	RDMA_NETWORK_ROCE_V1 = RDMA_NETWORK_IB,
	RDMA_NETWORK_IPV4,
	RDMA_NETWORK_IPV6
};

static inline enum ib_gid_type ib_network_to_gid_type(enum rdma_network_type network_type)
{
	if (network_type == RDMA_NETWORK_IPV4 ||
	    network_type == RDMA_NETWORK_IPV6)
		return IB_GID_TYPE_ROCE_UDP_ENCAP;

	
	return IB_GID_TYPE_IB;
}

static inline enum rdma_network_type ib_gid_to_network_type(enum ib_gid_type gid_type,
							    union ib_gid *gid)
{
	if (gid_type == IB_GID_TYPE_IB)
		return RDMA_NETWORK_IB;

	if (ipv6_addr_v4mapped((struct in6_addr *)gid))
		return RDMA_NETWORK_IPV4;
	else
		return RDMA_NETWORK_IPV6;
}

enum rdma_link_layer {
	IB_LINK_LAYER_UNSPECIFIED,
	IB_LINK_LAYER_INFINIBAND,
	IB_LINK_LAYER_ETHERNET,
};

enum ib_device_cap_flags {
	IB_DEVICE_RESIZE_MAX_WR			= (1 << 0),
	IB_DEVICE_BAD_PKEY_CNTR			= (1 << 1),
	IB_DEVICE_BAD_QKEY_CNTR			= (1 << 2),
	IB_DEVICE_RAW_MULTI			= (1 << 3),
	IB_DEVICE_AUTO_PATH_MIG			= (1 << 4),
	IB_DEVICE_CHANGE_PHY_PORT		= (1 << 5),
	IB_DEVICE_UD_AV_PORT_ENFORCE		= (1 << 6),
	IB_DEVICE_CURR_QP_STATE_MOD		= (1 << 7),
	IB_DEVICE_SHUTDOWN_PORT			= (1 << 8),
	IB_DEVICE_INIT_TYPE			= (1 << 9),
	IB_DEVICE_PORT_ACTIVE_EVENT		= (1 << 10),
	IB_DEVICE_SYS_IMAGE_GUID		= (1 << 11),
	IB_DEVICE_RC_RNR_NAK_GEN		= (1 << 12),
	IB_DEVICE_SRQ_RESIZE			= (1 << 13),
	IB_DEVICE_N_NOTIFY_CQ			= (1 << 14),

	
	IB_DEVICE_LOCAL_DMA_LKEY		= (1 << 15),
	IB_DEVICE_RESERVED 	= (1 << 16),
	IB_DEVICE_MEM_WINDOW			= (1 << 17),
	
	IB_DEVICE_UD_IP_CSUM			= (1 << 18),
	IB_DEVICE_UD_TSO			= (1 << 19),
	IB_DEVICE_XRC				= (1 << 20),

	
	IB_DEVICE_MEM_MGT_EXTENSIONS		= (1 << 21),
	IB_DEVICE_BLOCK_MULTICAST_LOOPBACK	= (1 << 22),
	IB_DEVICE_MEM_WINDOW_TYPE_2A		= (1 << 23),
	IB_DEVICE_MEM_WINDOW_TYPE_2B		= (1 << 24),
	IB_DEVICE_RC_IP_CSUM			= (1 << 25),
	IB_DEVICE_RAW_IP_CSUM			= (1 << 26),
	
	IB_DEVICE_CROSS_CHANNEL		= (1 << 27),
	IB_DEVICE_MANAGED_FLOW_STEERING		= (1 << 29),
	IB_DEVICE_SIGNATURE_HANDOVER		= (1 << 30),
	IB_DEVICE_ON_DEMAND_PAGING		= (1ULL << 31),
	IB_DEVICE_SG_GAPS_REG			= (1ULL << 32),
	IB_DEVICE_VIRTUAL_FUNCTION		= (1ULL << 33),
	IB_DEVICE_RAW_SCATTER_FCS		= (1ULL << 34),
};

enum ib_signature_prot_cap {
	IB_PROT_T10DIF_TYPE_1 = 1,
	IB_PROT_T10DIF_TYPE_2 = 1 << 1,
	IB_PROT_T10DIF_TYPE_3 = 1 << 2,
};

enum ib_signature_guard_cap {
	IB_GUARD_T10DIF_CRC	= 1,
	IB_GUARD_T10DIF_CSUM	= 1 << 1,
};

enum ib_atomic_cap {
	IB_ATOMIC_NONE,
	IB_ATOMIC_HCA,
	IB_ATOMIC_GLOB
};

enum ib_odp_general_cap_bits {
	IB_ODP_SUPPORT = 1 << 0,
};

enum ib_odp_transport_cap_bits {
	IB_ODP_SUPPORT_SEND	= 1 << 0,
	IB_ODP_SUPPORT_RECV	= 1 << 1,
	IB_ODP_SUPPORT_WRITE	= 1 << 2,
	IB_ODP_SUPPORT_READ	= 1 << 3,
	IB_ODP_SUPPORT_ATOMIC	= 1 << 4,
};

struct ib_odp_caps {
	uint64_t general_caps;
	struct {
		uint32_t  rc_odp_caps;
		uint32_t  uc_odp_caps;
		uint32_t  ud_odp_caps;
	} per_transport_caps;
};

struct ib_rss_caps {
	
	u32 supported_qpts;
	u32 max_rwq_indirection_tables;
	u32 max_rwq_indirection_table_size;
};

enum ib_cq_creation_flags {
	IB_CQ_FLAGS_TIMESTAMP_COMPLETION   = 1 << 0,
	IB_CQ_FLAGS_IGNORE_OVERRUN	   = 1 << 1,
};

struct ib_cq_init_attr {
	unsigned int	cqe;
	int		comp_vector;
	u32		flags;
};

struct ib_device_attr {
	u64			fw_ver;
	__be64			sys_image_guid;
	u64			max_mr_size;
	u64			page_size_cap;
	u32			vendor_id;
	u32			vendor_part_id;
	u32			hw_ver;
	int			max_qp;
	int			max_qp_wr;
	u64			device_cap_flags;
	int			max_sge;
	int			max_sge_rd;
	int			max_cq;
	int			max_cqe;
	int			max_mr;
	int			max_pd;
	int			max_qp_rd_atom;
	int			max_ee_rd_atom;
	int			max_res_rd_atom;
	int			max_qp_init_rd_atom;
	int			max_ee_init_rd_atom;
	enum ib_atomic_cap	atomic_cap;
	enum ib_atomic_cap	masked_atomic_cap;
	int			max_ee;
	int			max_rdd;
	int			max_mw;
	int			max_raw_ipv6_qp;
	int			max_raw_ethy_qp;
	int			max_mcast_grp;
	int			max_mcast_qp_attach;
	int			max_total_mcast_qp_attach;
	int			max_ah;
	int			max_fmr;
	int			max_map_per_fmr;
	int			max_srq;
	int			max_srq_wr;
	int			max_srq_sge;
	unsigned int		max_fast_reg_page_list_len;
	u16			max_pkeys;
	u8			local_ca_ack_delay;
	int			sig_prot_cap;
	int			sig_guard_cap;
	struct ib_odp_caps	odp_caps;
	uint64_t		timestamp_mask;
	uint64_t		hca_core_clock; 
	struct ib_rss_caps	rss_caps;
	u32			max_wq_type_rq;
};

enum ib_mtu {
	IB_MTU_256  = 1,
	IB_MTU_512  = 2,
	IB_MTU_1024 = 3,
	IB_MTU_2048 = 4,
	IB_MTU_4096 = 5
};

static inline int ib_mtu_enum_to_int(enum ib_mtu mtu)
{
	switch (mtu) {
	case IB_MTU_256:  return  256;
	case IB_MTU_512:  return  512;
	case IB_MTU_1024: return 1024;
	case IB_MTU_2048: return 2048;
	case IB_MTU_4096: return 4096;
	default: 	  return -1;
	}
}

enum ib_port_state {
	IB_PORT_NOP		= 0,
	IB_PORT_DOWN		= 1,
	IB_PORT_INIT		= 2,
	IB_PORT_ARMED		= 3,
	IB_PORT_ACTIVE		= 4,
	IB_PORT_ACTIVE_DEFER	= 5
};

enum ib_port_cap_flags {
	IB_PORT_SM				= 1 <<  1,
	IB_PORT_NOTICE_SUP			= 1 <<  2,
	IB_PORT_TRAP_SUP			= 1 <<  3,
	IB_PORT_OPT_IPD_SUP                     = 1 <<  4,
	IB_PORT_AUTO_MIGR_SUP			= 1 <<  5,
	IB_PORT_SL_MAP_SUP			= 1 <<  6,
	IB_PORT_MKEY_NVRAM			= 1 <<  7,
	IB_PORT_PKEY_NVRAM			= 1 <<  8,
	IB_PORT_LED_INFO_SUP			= 1 <<  9,
	IB_PORT_SM_DISABLED			= 1 << 10,
	IB_PORT_SYS_IMAGE_GUID_SUP		= 1 << 11,
	IB_PORT_PKEY_SW_EXT_PORT_TRAP_SUP	= 1 << 12,
	IB_PORT_EXTENDED_SPEEDS_SUP             = 1 << 14,
	IB_PORT_CM_SUP				= 1 << 16,
	IB_PORT_SNMP_TUNNEL_SUP			= 1 << 17,
	IB_PORT_REINIT_SUP			= 1 << 18,
	IB_PORT_DEVICE_MGMT_SUP			= 1 << 19,
	IB_PORT_VENDOR_CLASS_SUP		= 1 << 20,
	IB_PORT_DR_NOTICE_SUP			= 1 << 21,
	IB_PORT_CAP_MASK_NOTICE_SUP		= 1 << 22,
	IB_PORT_BOOT_MGMT_SUP			= 1 << 23,
	IB_PORT_LINK_LATENCY_SUP		= 1 << 24,
	IB_PORT_CLIENT_REG_SUP			= 1 << 25,
	IB_PORT_IP_BASED_GIDS			= 1 << 26,
};

enum ib_port_width {
	IB_WIDTH_1X	= 1,
	IB_WIDTH_4X	= 2,
	IB_WIDTH_8X	= 4,
	IB_WIDTH_12X	= 8
};

static inline int ib_width_enum_to_int(enum ib_port_width width)
{
	switch (width) {
	case IB_WIDTH_1X:  return  1;
	case IB_WIDTH_4X:  return  4;
	case IB_WIDTH_8X:  return  8;
	case IB_WIDTH_12X: return 12;
	default: 	  return -1;
	}
}

enum ib_port_speed {
	IB_SPEED_SDR	= 1,
	IB_SPEED_DDR	= 2,
	IB_SPEED_QDR	= 4,
	IB_SPEED_FDR10	= 8,
	IB_SPEED_FDR	= 16,
	IB_SPEED_EDR	= 32
};


struct rdma_hw_stats {
	unsigned long	timestamp;
	unsigned long	lifespan;
	const char * const *names;
	int		num_counters;
	u64		value[];
};

#define RDMA_HW_STATS_DEFAULT_LIFESPAN 10

static inline struct rdma_hw_stats *rdma_alloc_hw_stats_struct(
		const char * const *names, int num_counters,
		unsigned long lifespan)
{
	struct rdma_hw_stats *stats;

	stats = kzalloc(sizeof(*stats) + num_counters * sizeof(u64),
			GFP_KERNEL);
	if (!stats)
		return NULL;
	stats->names = names;
	stats->num_counters = num_counters;
	stats->lifespan = msecs_to_jiffies(lifespan);

	return stats;
}




#define RDMA_CORE_CAP_IB_MAD            0x00000001
#define RDMA_CORE_CAP_IB_SMI            0x00000002
#define RDMA_CORE_CAP_IB_CM             0x00000004
#define RDMA_CORE_CAP_IW_CM             0x00000008
#define RDMA_CORE_CAP_IB_SA             0x00000010
#define RDMA_CORE_CAP_OPA_MAD           0x00000020


#define RDMA_CORE_CAP_AF_IB             0x00001000
#define RDMA_CORE_CAP_ETH_AH            0x00002000


#define RDMA_CORE_CAP_PROT_IB           0x00100000
#define RDMA_CORE_CAP_PROT_ROCE         0x00200000
#define RDMA_CORE_CAP_PROT_IWARP        0x00400000
#define RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP 0x00800000

#define RDMA_CORE_PORT_IBA_IB          (RDMA_CORE_CAP_PROT_IB  \
					| RDMA_CORE_CAP_IB_MAD \
					| RDMA_CORE_CAP_IB_SMI \
					| RDMA_CORE_CAP_IB_CM  \
					| RDMA_CORE_CAP_IB_SA  \
					| RDMA_CORE_CAP_AF_IB)
#define RDMA_CORE_PORT_IBA_ROCE        (RDMA_CORE_CAP_PROT_ROCE \
					| RDMA_CORE_CAP_IB_MAD  \
					| RDMA_CORE_CAP_IB_CM   \
					| RDMA_CORE_CAP_AF_IB   \
					| RDMA_CORE_CAP_ETH_AH)
#define RDMA_CORE_PORT_IBA_ROCE_UDP_ENCAP			\
					(RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP \
					| RDMA_CORE_CAP_IB_MAD  \
					| RDMA_CORE_CAP_IB_CM   \
					| RDMA_CORE_CAP_AF_IB   \
					| RDMA_CORE_CAP_ETH_AH)
#define RDMA_CORE_PORT_IWARP           (RDMA_CORE_CAP_PROT_IWARP \
					| RDMA_CORE_CAP_IW_CM)
#define RDMA_CORE_PORT_INTEL_OPA       (RDMA_CORE_PORT_IBA_IB  \
					| RDMA_CORE_CAP_OPA_MAD)

struct ib_port_attr {
	u64			subnet_prefix;
	enum ib_port_state	state;
	enum ib_mtu		max_mtu;
	enum ib_mtu		active_mtu;
	int			gid_tbl_len;
	u32			port_cap_flags;
	u32			max_msg_sz;
	u32			bad_pkey_cntr;
	u32			qkey_viol_cntr;
	u16			pkey_tbl_len;
	u16			lid;
	u16			sm_lid;
	u8			lmc;
	u8			max_vl_num;
	u8			sm_sl;
	u8			subnet_timeout;
	u8			init_type_reply;
	u8			active_width;
	u8			active_speed;
	u8                      phys_state;
	bool			grh_required;
};

enum ib_device_modify_flags {
	IB_DEVICE_MODIFY_SYS_IMAGE_GUID	= 1 << 0,
	IB_DEVICE_MODIFY_NODE_DESC	= 1 << 1
};

#define IB_DEVICE_NODE_DESC_MAX 64

struct ib_device_modify {
	u64	sys_image_guid;
	char	node_desc[IB_DEVICE_NODE_DESC_MAX];
};

enum ib_port_modify_flags {
	IB_PORT_SHUTDOWN		= 1,
	IB_PORT_INIT_TYPE		= (1<<2),
	IB_PORT_RESET_QKEY_CNTR		= (1<<3)
};

struct ib_port_modify {
	u32	set_port_cap_mask;
	u32	clr_port_cap_mask;
	u8	init_type;
};

enum ib_event_type {
	IB_EVENT_CQ_ERR,
	IB_EVENT_QP_FATAL,
	IB_EVENT_QP_REQ_ERR,
	IB_EVENT_QP_ACCESS_ERR,
	IB_EVENT_COMM_EST,
	IB_EVENT_SQ_DRAINED,
	IB_EVENT_PATH_MIG,
	IB_EVENT_PATH_MIG_ERR,
	IB_EVENT_DEVICE_FATAL,
	IB_EVENT_PORT_ACTIVE,
	IB_EVENT_PORT_ERR,
	IB_EVENT_LID_CHANGE,
	IB_EVENT_PKEY_CHANGE,
	IB_EVENT_SM_CHANGE,
	IB_EVENT_SRQ_ERR,
	IB_EVENT_SRQ_LIMIT_REACHED,
	IB_EVENT_QP_LAST_WQE_REACHED,
	IB_EVENT_CLIENT_REREGISTER,
	IB_EVENT_GID_CHANGE,
	IB_EVENT_WQ_FATAL,
};

const char *__attribute_const__ ib_event_msg(enum ib_event_type event);

struct ib_event {
	struct ib_device	*device;
	union {
		struct ib_cq	*cq;
		struct ib_qp	*qp;
		struct ib_srq	*srq;
		struct ib_wq	*wq;
		u8		port_num;
	} element;
	enum ib_event_type	event;
};

struct ib_event_handler {
	struct ib_device *device;
	void            (*handler)(struct ib_event_handler *, struct ib_event *);
	struct list_head  list;
};

#define INIT_IB_EVENT_HANDLER(_ptr, _device, _handler)		\
	do {							\
		(_ptr)->device  = _device;			\
		(_ptr)->handler = _handler;			\
		INIT_LIST_HEAD(&(_ptr)->list);			\
	} while (0)

struct ib_global_route {
	union ib_gid	dgid;
	u32		flow_label;
	u8		sgid_index;
	u8		hop_limit;
	u8		traffic_class;
};

struct ib_grh {
	__be32		version_tclass_flow;
	__be16		paylen;
	u8		next_hdr;
	u8		hop_limit;
	union ib_gid	sgid;
	union ib_gid	dgid;
};

union rdma_network_hdr {
	struct ib_grh ibgrh;
	struct {
		
		u8		reserved[20];
		struct iphdr	roce4grh;
	};
};

enum {
	IB_MULTICAST_QPN = 0xffffff
};

#define IB_LID_PERMISSIVE	cpu_to_be16(0xFFFF)
#define IB_MULTICAST_LID_BASE	cpu_to_be16(0xC000)

enum ib_ah_flags {
	IB_AH_GRH	= 1
};

enum ib_rate {
	IB_RATE_PORT_CURRENT = 0,
	IB_RATE_2_5_GBPS = 2,
	IB_RATE_5_GBPS   = 5,
	IB_RATE_10_GBPS  = 3,
	IB_RATE_20_GBPS  = 6,
	IB_RATE_30_GBPS  = 4,
	IB_RATE_40_GBPS  = 7,
	IB_RATE_60_GBPS  = 8,
	IB_RATE_80_GBPS  = 9,
	IB_RATE_120_GBPS = 10,
	IB_RATE_14_GBPS  = 11,
	IB_RATE_56_GBPS  = 12,
	IB_RATE_112_GBPS = 13,
	IB_RATE_168_GBPS = 14,
	IB_RATE_25_GBPS  = 15,
	IB_RATE_100_GBPS = 16,
	IB_RATE_200_GBPS = 17,
	IB_RATE_300_GBPS = 18
};


__attribute_const__ int ib_rate_to_mult(enum ib_rate rate);


__attribute_const__ int ib_rate_to_mbps(enum ib_rate rate);



enum ib_mr_type {
	IB_MR_TYPE_MEM_REG,
	IB_MR_TYPE_SIGNATURE,
	IB_MR_TYPE_SG_GAPS,
};


enum ib_signature_type {
	IB_SIG_TYPE_NONE,
	IB_SIG_TYPE_T10_DIF,
};


enum ib_t10_dif_bg_type {
	IB_T10DIF_CRC,
	IB_T10DIF_CSUM
};


struct ib_t10_dif_domain {
	enum ib_t10_dif_bg_type bg_type;
	u16			pi_interval;
	u16			bg;
	u16			app_tag;
	u32			ref_tag;
	bool			ref_remap;
	bool			app_escape;
	bool			ref_escape;
	u16			apptag_check_mask;
};


struct ib_sig_domain {
	enum ib_signature_type sig_type;
	union {
		struct ib_t10_dif_domain dif;
	} sig;
};


struct ib_sig_attrs {
	u8			check_mask;
	struct ib_sig_domain	mem;
	struct ib_sig_domain	wire;
};

enum ib_sig_err_type {
	IB_SIG_BAD_GUARD,
	IB_SIG_BAD_REFTAG,
	IB_SIG_BAD_APPTAG,
};


struct ib_sig_err {
	enum ib_sig_err_type	err_type;
	u32			expected;
	u32			actual;
	u64			sig_err_offset;
	u32			key;
};

enum ib_mr_status_check {
	IB_MR_CHECK_SIG_STATUS = 1,
};


struct ib_mr_status {
	u32		    fail_status;
	struct ib_sig_err   sig_err;
};


__attribute_const__ enum ib_rate mult_to_ib_rate(int mult);

struct ib_ah_attr {
	struct ib_global_route	grh;
	u16			dlid;
	u8			sl;
	u8			src_path_bits;
	u8			static_rate;
	u8			ah_flags;
	u8			port_num;
	u8			dmac[ETH_ALEN];
};

enum ib_wc_status {
	IB_WC_SUCCESS,
	IB_WC_LOC_LEN_ERR,
	IB_WC_LOC_QP_OP_ERR,
	IB_WC_LOC_EEC_OP_ERR,
	IB_WC_LOC_PROT_ERR,
	IB_WC_WR_FLUSH_ERR,
	IB_WC_MW_BIND_ERR,
	IB_WC_BAD_RESP_ERR,
	IB_WC_LOC_ACCESS_ERR,
	IB_WC_REM_INV_REQ_ERR,
	IB_WC_REM_ACCESS_ERR,
	IB_WC_REM_OP_ERR,
	IB_WC_RETRY_EXC_ERR,
	IB_WC_RNR_RETRY_EXC_ERR,
	IB_WC_LOC_RDD_VIOL_ERR,
	IB_WC_REM_INV_RD_REQ_ERR,
	IB_WC_REM_ABORT_ERR,
	IB_WC_INV_EECN_ERR,
	IB_WC_INV_EEC_STATE_ERR,
	IB_WC_FATAL_ERR,
	IB_WC_RESP_TIMEOUT_ERR,
	IB_WC_GENERAL_ERR
};

const char *__attribute_const__ ib_wc_status_msg(enum ib_wc_status status);

enum ib_wc_opcode {
	IB_WC_SEND,
	IB_WC_RDMA_WRITE,
	IB_WC_RDMA_READ,
	IB_WC_COMP_SWAP,
	IB_WC_FETCH_ADD,
	IB_WC_LSO,
	IB_WC_LOCAL_INV,
	IB_WC_REG_MR,
	IB_WC_MASKED_COMP_SWAP,
	IB_WC_MASKED_FETCH_ADD,

	IB_WC_RECV			= 1 << 7,
	IB_WC_RECV_RDMA_WITH_IMM
};

enum ib_wc_flags {
	IB_WC_GRH		= 1,
	IB_WC_WITH_IMM		= (1<<1),
	IB_WC_WITH_INVALIDATE	= (1<<2),
	IB_WC_IP_CSUM_OK	= (1<<3),
	IB_WC_WITH_SMAC		= (1<<4),
	IB_WC_WITH_VLAN		= (1<<5),
	IB_WC_WITH_NETWORK_HDR_TYPE	= (1<<6),
};

struct ib_wc {
	union {
		u64		wr_id;
		struct ib_cqe	*wr_cqe;
	};
	enum ib_wc_status	status;
	enum ib_wc_opcode	opcode;
	u32			vendor_err;
	u32			byte_len;
	struct ib_qp	       *qp;
	union {
		__be32		imm_data;
		u32		invalidate_rkey;
	} ex;
	u32			src_qp;
	int			wc_flags;
	u16			pkey_index;
	u16			slid;
	u8			sl;
	u8			dlid_path_bits;
	u8			port_num;	
	u8			smac[ETH_ALEN];
	u16			vlan_id;
	u8			network_hdr_type;
};

enum ib_cq_notify_flags {
	IB_CQ_SOLICITED			= 1 << 0,
	IB_CQ_NEXT_COMP			= 1 << 1,
	IB_CQ_SOLICITED_MASK		= IB_CQ_SOLICITED | IB_CQ_NEXT_COMP,
	IB_CQ_REPORT_MISSED_EVENTS	= 1 << 2,
};

enum ib_srq_type {
	IB_SRQT_BASIC,
	IB_SRQT_XRC
};

enum ib_srq_attr_mask {
	IB_SRQ_MAX_WR	= 1 << 0,
	IB_SRQ_LIMIT	= 1 << 1,
};

struct ib_srq_attr {
	u32	max_wr;
	u32	max_sge;
	u32	srq_limit;
};

struct ib_srq_init_attr {
	void		      (*event_handler)(struct ib_event *, void *);
	void		       *srq_context;
	struct ib_srq_attr	attr;
	enum ib_srq_type	srq_type;

	union {
		struct {
			struct ib_xrcd *xrcd;
			struct ib_cq   *cq;
		} xrc;
	} ext;
};

struct ib_qp_cap {
	u32	max_send_wr;
	u32	max_recv_wr;
	u32	max_send_sge;
	u32	max_recv_sge;
	u32	max_inline_data;

	
	u32	max_rdma_ctxs;
};

enum ib_sig_type {
	IB_SIGNAL_ALL_WR,
	IB_SIGNAL_REQ_WR
};

enum ib_qp_type {
	
	IB_QPT_SMI,
	IB_QPT_GSI,

	IB_QPT_RC,
	IB_QPT_UC,
	IB_QPT_UD,
	IB_QPT_RAW_IPV6,
	IB_QPT_RAW_ETHERTYPE,
	IB_QPT_RAW_PACKET = 8,
	IB_QPT_XRC_INI = 9,
	IB_QPT_XRC_TGT,
	IB_QPT_MAX,
	
	IB_QPT_RESERVED1 = 0x1000,
	IB_QPT_RESERVED2,
	IB_QPT_RESERVED3,
	IB_QPT_RESERVED4,
	IB_QPT_RESERVED5,
	IB_QPT_RESERVED6,
	IB_QPT_RESERVED7,
	IB_QPT_RESERVED8,
	IB_QPT_RESERVED9,
	IB_QPT_RESERVED10,
};

enum ib_qp_create_flags {
	IB_QP_CREATE_IPOIB_UD_LSO		= 1 << 0,
	IB_QP_CREATE_BLOCK_MULTICAST_LOOPBACK	= 1 << 1,
	IB_QP_CREATE_CROSS_CHANNEL              = 1 << 2,
	IB_QP_CREATE_MANAGED_SEND               = 1 << 3,
	IB_QP_CREATE_MANAGED_RECV               = 1 << 4,
	IB_QP_CREATE_NETIF_QP			= 1 << 5,
	IB_QP_CREATE_SIGNATURE_EN		= 1 << 6,
	IB_QP_CREATE_USE_GFP_NOIO		= 1 << 7,
	IB_QP_CREATE_SCATTER_FCS		= 1 << 8,
	
	IB_QP_CREATE_RESERVED_START		= 1 << 26,
	IB_QP_CREATE_RESERVED_END		= 1 << 31,
};



struct ib_qp_init_attr {
	void                  (*event_handler)(struct ib_event *, void *);
	void		       *qp_context;
	struct ib_cq	       *send_cq;
	struct ib_cq	       *recv_cq;
	struct ib_srq	       *srq;
	struct ib_xrcd	       *xrcd;     
	struct ib_qp_cap	cap;
	enum ib_sig_type	sq_sig_type;
	enum ib_qp_type		qp_type;
	u32			create_flags;

	
	u8			port_num;
	struct ib_rwq_ind_table *rwq_ind_tbl;
};

struct ib_qp_open_attr {
	void                  (*event_handler)(struct ib_event *, void *);
	void		       *qp_context;
	u32			qp_num;
	enum ib_qp_type		qp_type;
};

enum ib_rnr_timeout {
	IB_RNR_TIMER_655_36 =  0,
	IB_RNR_TIMER_000_01 =  1,
	IB_RNR_TIMER_000_02 =  2,
	IB_RNR_TIMER_000_03 =  3,
	IB_RNR_TIMER_000_04 =  4,
	IB_RNR_TIMER_000_06 =  5,
	IB_RNR_TIMER_000_08 =  6,
	IB_RNR_TIMER_000_12 =  7,
	IB_RNR_TIMER_000_16 =  8,
	IB_RNR_TIMER_000_24 =  9,
	IB_RNR_TIMER_000_32 = 10,
	IB_RNR_TIMER_000_48 = 11,
	IB_RNR_TIMER_000_64 = 12,
	IB_RNR_TIMER_000_96 = 13,
	IB_RNR_TIMER_001_28 = 14,
	IB_RNR_TIMER_001_92 = 15,
	IB_RNR_TIMER_002_56 = 16,
	IB_RNR_TIMER_003_84 = 17,
	IB_RNR_TIMER_005_12 = 18,
	IB_RNR_TIMER_007_68 = 19,
	IB_RNR_TIMER_010_24 = 20,
	IB_RNR_TIMER_015_36 = 21,
	IB_RNR_TIMER_020_48 = 22,
	IB_RNR_TIMER_030_72 = 23,
	IB_RNR_TIMER_040_96 = 24,
	IB_RNR_TIMER_061_44 = 25,
	IB_RNR_TIMER_081_92 = 26,
	IB_RNR_TIMER_122_88 = 27,
	IB_RNR_TIMER_163_84 = 28,
	IB_RNR_TIMER_245_76 = 29,
	IB_RNR_TIMER_327_68 = 30,
	IB_RNR_TIMER_491_52 = 31
};

enum ib_qp_attr_mask {
	IB_QP_STATE			= 1,
	IB_QP_CUR_STATE			= (1<<1),
	IB_QP_EN_SQD_ASYNC_NOTIFY	= (1<<2),
	IB_QP_ACCESS_FLAGS		= (1<<3),
	IB_QP_PKEY_INDEX		= (1<<4),
	IB_QP_PORT			= (1<<5),
	IB_QP_QKEY			= (1<<6),
	IB_QP_AV			= (1<<7),
	IB_QP_PATH_MTU			= (1<<8),
	IB_QP_TIMEOUT			= (1<<9),
	IB_QP_RETRY_CNT			= (1<<10),
	IB_QP_RNR_RETRY			= (1<<11),
	IB_QP_RQ_PSN			= (1<<12),
	IB_QP_MAX_QP_RD_ATOMIC		= (1<<13),
	IB_QP_ALT_PATH			= (1<<14),
	IB_QP_MIN_RNR_TIMER		= (1<<15),
	IB_QP_SQ_PSN			= (1<<16),
	IB_QP_MAX_DEST_RD_ATOMIC	= (1<<17),
	IB_QP_PATH_MIG_STATE		= (1<<18),
	IB_QP_CAP			= (1<<19),
	IB_QP_DEST_QPN			= (1<<20),
	IB_QP_RESERVED1			= (1<<21),
	IB_QP_RESERVED2			= (1<<22),
	IB_QP_RESERVED3			= (1<<23),
	IB_QP_RESERVED4			= (1<<24),
};

enum ib_qp_state {
	IB_QPS_RESET,
	IB_QPS_INIT,
	IB_QPS_RTR,
	IB_QPS_RTS,
	IB_QPS_SQD,
	IB_QPS_SQE,
	IB_QPS_ERR
};

enum ib_mig_state {
	IB_MIG_MIGRATED,
	IB_MIG_REARM,
	IB_MIG_ARMED
};

enum ib_mw_type {
	IB_MW_TYPE_1 = 1,
	IB_MW_TYPE_2 = 2
};

struct ib_qp_attr {
	enum ib_qp_state	qp_state;
	enum ib_qp_state	cur_qp_state;
	enum ib_mtu		path_mtu;
	enum ib_mig_state	path_mig_state;
	u32			qkey;
	u32			rq_psn;
	u32			sq_psn;
	u32			dest_qp_num;
	int			qp_access_flags;
	struct ib_qp_cap	cap;
	struct ib_ah_attr	ah_attr;
	struct ib_ah_attr	alt_ah_attr;
	u16			pkey_index;
	u16			alt_pkey_index;
	u8			en_sqd_async_notify;
	u8			sq_draining;
	u8			max_rd_atomic;
	u8			max_dest_rd_atomic;
	u8			min_rnr_timer;
	u8			port_num;
	u8			timeout;
	u8			retry_cnt;
	u8			rnr_retry;
	u8			alt_port_num;
	u8			alt_timeout;
};

enum ib_wr_opcode {
	IB_WR_RDMA_WRITE,
	IB_WR_RDMA_WRITE_WITH_IMM,
	IB_WR_SEND,
	IB_WR_SEND_WITH_IMM,
	IB_WR_RDMA_READ,
	IB_WR_ATOMIC_CMP_AND_SWP,
	IB_WR_ATOMIC_FETCH_AND_ADD,
	IB_WR_LSO,
	IB_WR_SEND_WITH_INV,
	IB_WR_RDMA_READ_WITH_INV,
	IB_WR_LOCAL_INV,
	IB_WR_REG_MR,
	IB_WR_MASKED_ATOMIC_CMP_AND_SWP,
	IB_WR_MASKED_ATOMIC_FETCH_AND_ADD,
	IB_WR_REG_SIG_MR,
	
	IB_WR_RESERVED1 = 0xf0,
	IB_WR_RESERVED2,
	IB_WR_RESERVED3,
	IB_WR_RESERVED4,
	IB_WR_RESERVED5,
	IB_WR_RESERVED6,
	IB_WR_RESERVED7,
	IB_WR_RESERVED8,
	IB_WR_RESERVED9,
	IB_WR_RESERVED10,
};

enum ib_send_flags {
	IB_SEND_FENCE		= 1,
	IB_SEND_SIGNALED	= (1<<1),
	IB_SEND_SOLICITED	= (1<<2),
	IB_SEND_INLINE		= (1<<3),
	IB_SEND_IP_CSUM		= (1<<4),

	
	IB_SEND_RESERVED_START	= (1 << 26),
	IB_SEND_RESERVED_END	= (1 << 31),
};

struct ib_sge {
	u64	addr;
	u32	length;
	u32	lkey;
};

struct ib_cqe {
	void (*done)(struct ib_cq *cq, struct ib_wc *wc);
};

struct ib_send_wr {
	struct ib_send_wr      *next;
	union {
		u64		wr_id;
		struct ib_cqe	*wr_cqe;
	};
	struct ib_sge	       *sg_list;
	int			num_sge;
	enum ib_wr_opcode	opcode;
	int			send_flags;
	union {
		__be32		imm_data;
		u32		invalidate_rkey;
	} ex;
};

struct ib_rdma_wr {
	struct ib_send_wr	wr;
	u64			remote_addr;
	u32			rkey;
};

static inline struct ib_rdma_wr *rdma_wr(struct ib_send_wr *wr)
{
	return container_of(wr, struct ib_rdma_wr, wr);
}

struct ib_atomic_wr {
	struct ib_send_wr	wr;
	u64			remote_addr;
	u64			compare_add;
	u64			swap;
	u64			compare_add_mask;
	u64			swap_mask;
	u32			rkey;
};

static inline struct ib_atomic_wr *atomic_wr(struct ib_send_wr *wr)
{
	return container_of(wr, struct ib_atomic_wr, wr);
}

struct ib_ud_wr {
	struct ib_send_wr	wr;
	struct ib_ah		*ah;
	void			*header;
	int			hlen;
	int			mss;
	u32			remote_qpn;
	u32			remote_qkey;
	u16			pkey_index; 
	u8			port_num;   
};

static inline struct ib_ud_wr *ud_wr(struct ib_send_wr *wr)
{
	return container_of(wr, struct ib_ud_wr, wr);
}

struct ib_reg_wr {
	struct ib_send_wr	wr;
	struct ib_mr		*mr;
	u32			key;
	int			access;
};

static inline struct ib_reg_wr *reg_wr(struct ib_send_wr *wr)
{
	return container_of(wr, struct ib_reg_wr, wr);
}

struct ib_sig_handover_wr {
	struct ib_send_wr	wr;
	struct ib_sig_attrs    *sig_attrs;
	struct ib_mr	       *sig_mr;
	int			access_flags;
	struct ib_sge	       *prot;
};

static inline struct ib_sig_handover_wr *sig_handover_wr(struct ib_send_wr *wr)
{
	return container_of(wr, struct ib_sig_handover_wr, wr);
}

struct ib_recv_wr {
	struct ib_recv_wr      *next;
	union {
		u64		wr_id;
		struct ib_cqe	*wr_cqe;
	};
	struct ib_sge	       *sg_list;
	int			num_sge;
};

enum ib_access_flags {
	IB_ACCESS_LOCAL_WRITE	= 1,
	IB_ACCESS_REMOTE_WRITE	= (1<<1),
	IB_ACCESS_REMOTE_READ	= (1<<2),
	IB_ACCESS_REMOTE_ATOMIC	= (1<<3),
	IB_ACCESS_MW_BIND	= (1<<4),
	IB_ZERO_BASED		= (1<<5),
	IB_ACCESS_ON_DEMAND     = (1<<6),
};


enum ib_mr_rereg_flags {
	IB_MR_REREG_TRANS	= 1,
	IB_MR_REREG_PD		= (1<<1),
	IB_MR_REREG_ACCESS	= (1<<2),
	IB_MR_REREG_SUPPORTED	= ((IB_MR_REREG_ACCESS << 1) - 1)
};

struct ib_fmr_attr {
	int	max_pages;
	int	max_maps;
	u8	page_shift;
};

struct ib_umem;

struct ib_ucontext {
	struct ib_device       *device;
	struct list_head	pd_list;
	struct list_head	mr_list;
	struct list_head	mw_list;
	struct list_head	cq_list;
	struct list_head	qp_list;
	struct list_head	srq_list;
	struct list_head	ah_list;
	struct list_head	xrcd_list;
	struct list_head	rule_list;
	struct list_head	wq_list;
	struct list_head	rwq_ind_tbl_list;
	int			closing;

	struct pid             *tgid;
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	struct rb_root      umem_tree;
	
	struct rw_semaphore	umem_rwsem;
	void (*invalidate_range)(struct ib_umem *umem,
				 unsigned long start, unsigned long end);

	struct mmu_notifier	mn;
	atomic_t		notifier_count;
	
	struct list_head	no_private_counters;
	int                     odp_mrs_count;
#endif
};

struct ib_uobject {
	u64			user_handle;	
	struct ib_ucontext     *context;	
	void		       *object;		
	struct list_head	list;		
	int			id;		
	struct kref		ref;
	struct rw_semaphore	mutex;		
	struct rcu_head		rcu;		
	int			live;
};

struct ib_udata {
	const void __user *inbuf;
	void __user *outbuf;
	size_t       inlen;
	size_t       outlen;
};

struct ib_pd {
	u32			local_dma_lkey;
	u32			flags;
	struct ib_device       *device;
	struct ib_uobject      *uobject;
	atomic_t          	usecnt; 

	u32			unsafe_global_rkey;

	
	struct ib_mr	       *__internal_mr;
};

struct ib_xrcd {
	struct ib_device       *device;
	atomic_t		usecnt; 
	struct inode	       *inode;

	struct mutex		tgt_qp_mutex;
	struct list_head	tgt_qp_list;
};

struct ib_ah {
	struct ib_device	*device;
	struct ib_pd		*pd;
	struct ib_uobject	*uobject;
};

typedef void (*ib_comp_handler)(struct ib_cq *cq, void *cq_context);

enum ib_poll_context {
	IB_POLL_DIRECT,		   
	IB_POLL_SOFTIRQ,	   
	IB_POLL_WORKQUEUE,	   
	IB_POLL_UNBOUND_WORKQUEUE, 
};

struct ib_cq {
	struct ib_device       *device;
	struct ib_uobject      *uobject;
	ib_comp_handler   	comp_handler;
	void                  (*event_handler)(struct ib_event *, void *);
	void                   *cq_context;
	int               	cqe;
	atomic_t          	usecnt; 
	enum ib_poll_context	poll_ctx;
	struct ib_wc		*wc;
	union {
		struct irq_poll		iop;
		struct work_struct	work;
	};
	struct workqueue_struct *comp_wq;
};

struct ib_srq {
	struct ib_device       *device;
	struct ib_pd	       *pd;
	struct ib_uobject      *uobject;
	void		      (*event_handler)(struct ib_event *, void *);
	void		       *srq_context;
	enum ib_srq_type	srq_type;
	atomic_t		usecnt;

	union {
		struct {
			struct ib_xrcd *xrcd;
			struct ib_cq   *cq;
			u32		srq_num;
		} xrc;
	} ext;
};

enum ib_wq_type {
	IB_WQT_RQ
};

enum ib_wq_state {
	IB_WQS_RESET,
	IB_WQS_RDY,
	IB_WQS_ERR
};

struct ib_wq {
	struct ib_device       *device;
	struct ib_uobject      *uobject;
	void		    *wq_context;
	void		    (*event_handler)(struct ib_event *, void *);
	struct ib_pd	       *pd;
	struct ib_cq	       *cq;
	u32		wq_num;
	enum ib_wq_state       state;
	enum ib_wq_type	wq_type;
	atomic_t		usecnt;
};

struct ib_wq_init_attr {
	void		       *wq_context;
	enum ib_wq_type	wq_type;
	u32		max_wr;
	u32		max_sge;
	struct	ib_cq	       *cq;
	void		    (*event_handler)(struct ib_event *, void *);
};

enum ib_wq_attr_mask {
	IB_WQ_STATE	= 1 << 0,
	IB_WQ_CUR_STATE	= 1 << 1,
};

struct ib_wq_attr {
	enum	ib_wq_state	wq_state;
	enum	ib_wq_state	curr_wq_state;
};

struct ib_rwq_ind_table {
	struct ib_device	*device;
	struct ib_uobject      *uobject;
	atomic_t		usecnt;
	u32		ind_tbl_num;
	u32		log_ind_tbl_size;
	struct ib_wq	**ind_tbl;
};

struct ib_rwq_ind_table_init_attr {
	u32		log_ind_tbl_size;
	
	struct ib_wq	**ind_tbl;
};


struct ib_qp {
	struct ib_device       *device;
	struct ib_pd	       *pd;
	struct ib_cq	       *send_cq;
	struct ib_cq	       *recv_cq;
	spinlock_t		mr_lock;
	int			mrs_used;
	struct list_head	rdma_mrs;
	struct list_head	sig_mrs;
	struct ib_srq	       *srq;
	struct ib_xrcd	       *xrcd; 
	struct list_head	xrcd_list;

	
	atomic_t		usecnt;
	struct list_head	open_list;
	struct ib_qp           *real_qp;
	struct ib_uobject      *uobject;
	void                  (*event_handler)(struct ib_event *, void *);
	void		       *qp_context;
	u32			qp_num;
	u32			max_write_sge;
	u32			max_read_sge;
	enum ib_qp_type		qp_type;
	struct ib_rwq_ind_table *rwq_ind_tbl;
};

struct ib_mr {
	struct ib_device  *device;
	struct ib_pd	  *pd;
	u32		   lkey;
	u32		   rkey;
	u64		   iova;
	u32		   length;
	unsigned int	   page_size;
	bool		   need_inval;
	union {
		struct ib_uobject	*uobject;	
		struct list_head	qp_entry;	
	};
};

struct ib_mw {
	struct ib_device	*device;
	struct ib_pd		*pd;
	struct ib_uobject	*uobject;
	u32			rkey;
	enum ib_mw_type         type;
};

struct ib_fmr {
	struct ib_device	*device;
	struct ib_pd		*pd;
	struct list_head	list;
	u32			lkey;
	u32			rkey;
};


enum ib_flow_attr_type {
	
	IB_FLOW_ATTR_NORMAL		= 0x0,
	
	IB_FLOW_ATTR_ALL_DEFAULT	= 0x1,
	
	IB_FLOW_ATTR_MC_DEFAULT		= 0x2,
	
	IB_FLOW_ATTR_SNIFFER		= 0x3
};


enum ib_flow_spec_type {
	
	IB_FLOW_SPEC_ETH	= 0x20,
	IB_FLOW_SPEC_IB		= 0x22,
	
	IB_FLOW_SPEC_IPV4	= 0x30,
	IB_FLOW_SPEC_IPV6	= 0x31,
	
	IB_FLOW_SPEC_TCP	= 0x40,
	IB_FLOW_SPEC_UDP	= 0x41
};
#define IB_FLOW_SPEC_LAYER_MASK	0xF0
#define IB_FLOW_SPEC_SUPPORT_LAYERS 4


enum ib_flow_domain {
	IB_FLOW_DOMAIN_USER,
	IB_FLOW_DOMAIN_ETHTOOL,
	IB_FLOW_DOMAIN_RFS,
	IB_FLOW_DOMAIN_NIC,
	IB_FLOW_DOMAIN_NUM 
};

enum ib_flow_flags {
	IB_FLOW_ATTR_FLAGS_DONT_TRAP = 1UL << 1, 
	IB_FLOW_ATTR_FLAGS_RESERVED  = 1UL << 2  
};

struct ib_flow_eth_filter {
	u8	dst_mac[6];
	u8	src_mac[6];
	__be16	ether_type;
	__be16	vlan_tag;
	
	u8	real_sz[0];
};

struct ib_flow_spec_eth {
	enum ib_flow_spec_type	  type;
	u16			  size;
	struct ib_flow_eth_filter val;
	struct ib_flow_eth_filter mask;
};

struct ib_flow_ib_filter {
	__be16 dlid;
	__u8   sl;
	
	u8	real_sz[0];
};

struct ib_flow_spec_ib {
	enum ib_flow_spec_type	 type;
	u16			 size;
	struct ib_flow_ib_filter val;
	struct ib_flow_ib_filter mask;
};


enum ib_ipv4_flags {
	IB_IPV4_DONT_FRAG = 0x2, 
	IB_IPV4_MORE_FRAG = 0X4  
};

struct ib_flow_ipv4_filter {
	__be32	src_ip;
	__be32	dst_ip;
	u8	proto;
	u8	tos;
	u8	ttl;
	u8	flags;
	
	u8	real_sz[0];
};

struct ib_flow_spec_ipv4 {
	enum ib_flow_spec_type	   type;
	u16			   size;
	struct ib_flow_ipv4_filter val;
	struct ib_flow_ipv4_filter mask;
};

struct ib_flow_ipv6_filter {
	u8	src_ip[16];
	u8	dst_ip[16];
	__be32	flow_label;
	u8	next_hdr;
	u8	traffic_class;
	u8	hop_limit;
	
	u8	real_sz[0];
};

struct ib_flow_spec_ipv6 {
	enum ib_flow_spec_type	   type;
	u16			   size;
	struct ib_flow_ipv6_filter val;
	struct ib_flow_ipv6_filter mask;
};

struct ib_flow_tcp_udp_filter {
	__be16	dst_port;
	__be16	src_port;
	
	u8	real_sz[0];
};

struct ib_flow_spec_tcp_udp {
	enum ib_flow_spec_type	      type;
	u16			      size;
	struct ib_flow_tcp_udp_filter val;
	struct ib_flow_tcp_udp_filter mask;
};

union ib_flow_spec {
	struct {
		enum ib_flow_spec_type	type;
		u16			size;
	};
	struct ib_flow_spec_eth		eth;
	struct ib_flow_spec_ib		ib;
	struct ib_flow_spec_ipv4        ipv4;
	struct ib_flow_spec_tcp_udp	tcp_udp;
	struct ib_flow_spec_ipv6        ipv6;
};

struct ib_flow_attr {
	enum ib_flow_attr_type type;
	u16	     size;
	u16	     priority;
	u32	     flags;
	u8	     num_of_specs;
	u8	     port;
	
};

struct ib_flow {
	struct ib_qp		*qp;
	struct ib_uobject	*uobject;
};

struct ib_mad_hdr;
struct ib_grh;

enum ib_process_mad_flags {
	IB_MAD_IGNORE_MKEY	= 1,
	IB_MAD_IGNORE_BKEY	= 2,
	IB_MAD_IGNORE_ALL	= IB_MAD_IGNORE_MKEY | IB_MAD_IGNORE_BKEY
};

enum ib_mad_result {
	IB_MAD_RESULT_FAILURE  = 0,      
	IB_MAD_RESULT_SUCCESS  = 1 << 0, 
	IB_MAD_RESULT_REPLY    = 1 << 1, 
	IB_MAD_RESULT_CONSUMED = 1 << 2  
};

#define IB_DEVICE_NAME_MAX 64

struct ib_cache {
	rwlock_t                lock;
	struct ib_event_handler event_handler;
	struct ib_pkey_cache  **pkey_cache;
	struct ib_gid_table   **gid_cache;
	u8                     *lmc_cache;
};

struct ib_dma_mapping_ops {
	int		(*mapping_error)(struct ib_device *dev,
					 u64 dma_addr);
	u64		(*map_single)(struct ib_device *dev,
				      void *ptr, size_t size,
				      enum dma_data_direction direction);
	void		(*unmap_single)(struct ib_device *dev,
					u64 addr, size_t size,
					enum dma_data_direction direction);
	u64		(*map_page)(struct ib_device *dev,
				    struct page *page, unsigned long offset,
				    size_t size,
				    enum dma_data_direction direction);
	void		(*unmap_page)(struct ib_device *dev,
				      u64 addr, size_t size,
				      enum dma_data_direction direction);
	int		(*map_sg)(struct ib_device *dev,
				  struct scatterlist *sg, int nents,
				  enum dma_data_direction direction);
	void		(*unmap_sg)(struct ib_device *dev,
				    struct scatterlist *sg, int nents,
				    enum dma_data_direction direction);
	int		(*map_sg_attrs)(struct ib_device *dev,
					struct scatterlist *sg, int nents,
					enum dma_data_direction direction,
					unsigned long attrs);
	void		(*unmap_sg_attrs)(struct ib_device *dev,
					  struct scatterlist *sg, int nents,
					  enum dma_data_direction direction,
					  unsigned long attrs);
	void		(*sync_single_for_cpu)(struct ib_device *dev,
					       u64 dma_handle,
					       size_t size,
					       enum dma_data_direction dir);
	void		(*sync_single_for_device)(struct ib_device *dev,
						  u64 dma_handle,
						  size_t size,
						  enum dma_data_direction dir);
	void		*(*alloc_coherent)(struct ib_device *dev,
					   size_t size,
					   u64 *dma_handle,
					   gfp_t flag);
	void		(*free_coherent)(struct ib_device *dev,
					 size_t size, void *cpu_addr,
					 u64 dma_handle);
};

struct iw_cm_verbs;

struct ib_port_immutable {
	int                           pkey_tbl_len;
	int                           gid_tbl_len;
	u32                           core_cap_flags;
	u32                           max_mad_size;
};

struct ib_device {
	struct device                *dma_device;

	char                          name[IB_DEVICE_NAME_MAX];

	struct list_head              event_handler_list;
	spinlock_t                    event_handler_lock;

	spinlock_t                    client_data_lock;
	struct list_head              core_list;
	
	struct list_head              client_data_list;

	struct ib_cache               cache;
	
	struct ib_port_immutable     *port_immutable;

	int			      num_comp_vectors;

	struct iw_cm_verbs	     *iwcm;

	
	struct rdma_hw_stats      *(*alloc_hw_stats)(struct ib_device *device,
						     u8 port_num);
	
	int		           (*get_hw_stats)(struct ib_device *device,
						   struct rdma_hw_stats *stats,
						   u8 port, int index);
	int		           (*query_device)(struct ib_device *device,
						   struct ib_device_attr *device_attr,
						   struct ib_udata *udata);
	int		           (*query_port)(struct ib_device *device,
						 u8 port_num,
						 struct ib_port_attr *port_attr);
	enum rdma_link_layer	   (*get_link_layer)(struct ib_device *device,
						     u8 port_num);
	
	struct net_device	  *(*get_netdev)(struct ib_device *device,
						 u8 port_num);
	int		           (*query_gid)(struct ib_device *device,
						u8 port_num, int index,
						union ib_gid *gid);
	
	int		           (*add_gid)(struct ib_device *device,
					      u8 port_num,
					      unsigned int index,
					      const union ib_gid *gid,
					      const struct ib_gid_attr *attr,
					      void **context);
	
	int		           (*del_gid)(struct ib_device *device,
					      u8 port_num,
					      unsigned int index,
					      void **context);
	int		           (*query_pkey)(struct ib_device *device,
						 u8 port_num, u16 index, u16 *pkey);
	int		           (*modify_device)(struct ib_device *device,
						    int device_modify_mask,
						    struct ib_device_modify *device_modify);
	int		           (*modify_port)(struct ib_device *device,
						  u8 port_num, int port_modify_mask,
						  struct ib_port_modify *port_modify);
	struct ib_ucontext *       (*alloc_ucontext)(struct ib_device *device,
						     struct ib_udata *udata);
	int                        (*dealloc_ucontext)(struct ib_ucontext *context);
	int                        (*mmap)(struct ib_ucontext *context,
					   struct vm_area_struct *vma);
	struct ib_pd *             (*alloc_pd)(struct ib_device *device,
					       struct ib_ucontext *context,
					       struct ib_udata *udata);
	int                        (*dealloc_pd)(struct ib_pd *pd);
	struct ib_ah *             (*create_ah)(struct ib_pd *pd,
						struct ib_ah_attr *ah_attr);
	int                        (*modify_ah)(struct ib_ah *ah,
						struct ib_ah_attr *ah_attr);
	int                        (*query_ah)(struct ib_ah *ah,
					       struct ib_ah_attr *ah_attr);
	int                        (*destroy_ah)(struct ib_ah *ah);
	struct ib_srq *            (*create_srq)(struct ib_pd *pd,
						 struct ib_srq_init_attr *srq_init_attr,
						 struct ib_udata *udata);
	int                        (*modify_srq)(struct ib_srq *srq,
						 struct ib_srq_attr *srq_attr,
						 enum ib_srq_attr_mask srq_attr_mask,
						 struct ib_udata *udata);
	int                        (*query_srq)(struct ib_srq *srq,
						struct ib_srq_attr *srq_attr);
	int                        (*destroy_srq)(struct ib_srq *srq);
	int                        (*post_srq_recv)(struct ib_srq *srq,
						    struct ib_recv_wr *recv_wr,
						    struct ib_recv_wr **bad_recv_wr);
	struct ib_qp *             (*create_qp)(struct ib_pd *pd,
						struct ib_qp_init_attr *qp_init_attr,
						struct ib_udata *udata);
	int                        (*modify_qp)(struct ib_qp *qp,
						struct ib_qp_attr *qp_attr,
						int qp_attr_mask,
						struct ib_udata *udata);
	int                        (*query_qp)(struct ib_qp *qp,
					       struct ib_qp_attr *qp_attr,
					       int qp_attr_mask,
					       struct ib_qp_init_attr *qp_init_attr);
	int                        (*destroy_qp)(struct ib_qp *qp);
	int                        (*post_send)(struct ib_qp *qp,
						struct ib_send_wr *send_wr,
						struct ib_send_wr **bad_send_wr);
	int                        (*post_recv)(struct ib_qp *qp,
						struct ib_recv_wr *recv_wr,
						struct ib_recv_wr **bad_recv_wr);
	struct ib_cq *             (*create_cq)(struct ib_device *device,
						const struct ib_cq_init_attr *attr,
						struct ib_ucontext *context,
						struct ib_udata *udata);
	int                        (*modify_cq)(struct ib_cq *cq, u16 cq_count,
						u16 cq_period);
	int                        (*destroy_cq)(struct ib_cq *cq);
	int                        (*resize_cq)(struct ib_cq *cq, int cqe,
						struct ib_udata *udata);
	int                        (*poll_cq)(struct ib_cq *cq, int num_entries,
					      struct ib_wc *wc);
	int                        (*peek_cq)(struct ib_cq *cq, int wc_cnt);
	int                        (*req_notify_cq)(struct ib_cq *cq,
						    enum ib_cq_notify_flags flags);
	int                        (*req_ncomp_notif)(struct ib_cq *cq,
						      int wc_cnt);
	struct ib_mr *             (*get_dma_mr)(struct ib_pd *pd,
						 int mr_access_flags);
	struct ib_mr *             (*reg_user_mr)(struct ib_pd *pd,
						  u64 start, u64 length,
						  u64 virt_addr,
						  int mr_access_flags,
						  struct ib_udata *udata);
	int			   (*rereg_user_mr)(struct ib_mr *mr,
						    int flags,
						    u64 start, u64 length,
						    u64 virt_addr,
						    int mr_access_flags,
						    struct ib_pd *pd,
						    struct ib_udata *udata);
	int                        (*dereg_mr)(struct ib_mr *mr);
	struct ib_mr *		   (*alloc_mr)(struct ib_pd *pd,
					       enum ib_mr_type mr_type,
					       u32 max_num_sg);
	int                        (*map_mr_sg)(struct ib_mr *mr,
						struct scatterlist *sg,
						int sg_nents,
						unsigned int *sg_offset);
	struct ib_mw *             (*alloc_mw)(struct ib_pd *pd,
					       enum ib_mw_type type,
					       struct ib_udata *udata);
	int                        (*dealloc_mw)(struct ib_mw *mw);
	struct ib_fmr *	           (*alloc_fmr)(struct ib_pd *pd,
						int mr_access_flags,
						struct ib_fmr_attr *fmr_attr);
	int		           (*map_phys_fmr)(struct ib_fmr *fmr,
						   u64 *page_list, int list_len,
						   u64 iova);
	int		           (*unmap_fmr)(struct list_head *fmr_list);
	int		           (*dealloc_fmr)(struct ib_fmr *fmr);
	int                        (*attach_mcast)(struct ib_qp *qp,
						   union ib_gid *gid,
						   u16 lid);
	int                        (*detach_mcast)(struct ib_qp *qp,
						   union ib_gid *gid,
						   u16 lid);
	int                        (*process_mad)(struct ib_device *device,
						  int process_mad_flags,
						  u8 port_num,
						  const struct ib_wc *in_wc,
						  const struct ib_grh *in_grh,
						  const struct ib_mad_hdr *in_mad,
						  size_t in_mad_size,
						  struct ib_mad_hdr *out_mad,
						  size_t *out_mad_size,
						  u16 *out_mad_pkey_index);
	struct ib_xrcd *	   (*alloc_xrcd)(struct ib_device *device,
						 struct ib_ucontext *ucontext,
						 struct ib_udata *udata);
	int			   (*dealloc_xrcd)(struct ib_xrcd *xrcd);
	struct ib_flow *	   (*create_flow)(struct ib_qp *qp,
						  struct ib_flow_attr
						  *flow_attr,
						  int domain);
	int			   (*destroy_flow)(struct ib_flow *flow_id);
	int			   (*check_mr_status)(struct ib_mr *mr, u32 check_mask,
						      struct ib_mr_status *mr_status);
	void			   (*disassociate_ucontext)(struct ib_ucontext *ibcontext);
	void			   (*drain_rq)(struct ib_qp *qp);
	void			   (*drain_sq)(struct ib_qp *qp);
	int			   (*set_vf_link_state)(struct ib_device *device, int vf, u8 port,
							int state);
	int			   (*get_vf_config)(struct ib_device *device, int vf, u8 port,
						   struct ifla_vf_info *ivf);
	int			   (*get_vf_stats)(struct ib_device *device, int vf, u8 port,
						   struct ifla_vf_stats *stats);
	int			   (*set_vf_guid)(struct ib_device *device, int vf, u8 port, u64 guid,
						  int type);
	struct ib_wq *		   (*create_wq)(struct ib_pd *pd,
						struct ib_wq_init_attr *init_attr,
						struct ib_udata *udata);
	int			   (*destroy_wq)(struct ib_wq *wq);
	int			   (*modify_wq)(struct ib_wq *wq,
						struct ib_wq_attr *attr,
						u32 wq_attr_mask,
						struct ib_udata *udata);
	struct ib_rwq_ind_table *  (*create_rwq_ind_table)(struct ib_device *device,
							   struct ib_rwq_ind_table_init_attr *init_attr,
							   struct ib_udata *udata);
	int                        (*destroy_rwq_ind_table)(struct ib_rwq_ind_table *wq_ind_table);
	struct ib_dma_mapping_ops   *dma_ops;

	struct module               *owner;
	struct device                dev;
	struct kobject               *ports_parent;
	struct list_head             port_list;

	enum {
		IB_DEV_UNINITIALIZED,
		IB_DEV_REGISTERED,
		IB_DEV_UNREGISTERED
	}                            reg_state;

	int			     uverbs_abi_ver;
	u64			     uverbs_cmd_mask;
	u64			     uverbs_ex_cmd_mask;

	char			     node_desc[IB_DEVICE_NODE_DESC_MAX];
	__be64			     node_guid;
	u32			     local_dma_lkey;
	u16                          is_switch:1;
	u8                           node_type;
	u8                           phys_port_cnt;
	struct ib_device_attr        attrs;
	struct attribute_group	     *hw_stats_ag;
	struct rdma_hw_stats         *hw_stats;

	
	int (*get_port_immutable)(struct ib_device *, u8, struct ib_port_immutable *);
	void (*get_dev_fw_str)(struct ib_device *, char *str, size_t str_len);
};

struct ib_client {
	char  *name;
	void (*add)   (struct ib_device *);
	void (*remove)(struct ib_device *, void *client_data);

	
	struct net_device *(*get_net_dev_by_params)(
			struct ib_device *dev,
			u8 port,
			u16 pkey,
			const union ib_gid *gid,
			const struct sockaddr *addr,
			void *client_data);
	struct list_head list;
};

struct ib_device *ib_alloc_device(size_t size);
void ib_dealloc_device(struct ib_device *device);

void ib_get_device_fw_str(struct ib_device *device, char *str, size_t str_len);

int ib_register_device(struct ib_device *device,
		       int (*port_callback)(struct ib_device *,
					    u8, struct kobject *));
void ib_unregister_device(struct ib_device *device);

int ib_register_client   (struct ib_client *client);
void ib_unregister_client(struct ib_client *client);

void *ib_get_client_data(struct ib_device *device, struct ib_client *client);
void  ib_set_client_data(struct ib_device *device, struct ib_client *client,
			 void *data);

static inline int ib_copy_from_udata(void *dest, struct ib_udata *udata, size_t len)
{
	return copy_from_user(dest, udata->inbuf, len) ? -EFAULT : 0;
}

static inline int ib_copy_to_udata(struct ib_udata *udata, void *src, size_t len)
{
	return copy_to_user(udata->outbuf, src, len) ? -EFAULT : 0;
}

static inline bool ib_is_udata_cleared(struct ib_udata *udata,
				       size_t offset,
				       size_t len)
{
	const void __user *p = udata->inbuf + offset;
	bool ret;
	u8 *buf;

	if (len > USHRT_MAX)
		return false;

	buf = memdup_user(p, len);
	if (IS_ERR(buf))
		return false;

	ret = !memchr_inv(buf, 0, len);
	kfree(buf);
	return ret;
}


int ib_modify_qp_is_ok(enum ib_qp_state cur_state, enum ib_qp_state next_state,
		       enum ib_qp_type type, enum ib_qp_attr_mask mask,
		       enum rdma_link_layer ll);

int ib_register_event_handler  (struct ib_event_handler *event_handler);
int ib_unregister_event_handler(struct ib_event_handler *event_handler);
void ib_dispatch_event(struct ib_event *event);

int ib_query_port(struct ib_device *device,
		  u8 port_num, struct ib_port_attr *port_attr);

enum rdma_link_layer rdma_port_get_link_layer(struct ib_device *device,
					       u8 port_num);


static inline bool rdma_cap_ib_switch(const struct ib_device *device)
{
	return device->is_switch;
}


static inline u8 rdma_start_port(const struct ib_device *device)
{
	return rdma_cap_ib_switch(device) ? 0 : 1;
}


static inline u8 rdma_end_port(const struct ib_device *device)
{
	return rdma_cap_ib_switch(device) ? 0 : device->phys_port_cnt;
}

static inline bool rdma_protocol_ib(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_PROT_IB;
}

static inline bool rdma_protocol_roce(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags &
		(RDMA_CORE_CAP_PROT_ROCE | RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP);
}

static inline bool rdma_protocol_roce_udp_encap(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP;
}

static inline bool rdma_protocol_roce_eth_encap(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_PROT_ROCE;
}

static inline bool rdma_protocol_iwarp(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_PROT_IWARP;
}

static inline bool rdma_ib_or_roce(const struct ib_device *device, u8 port_num)
{
	return rdma_protocol_ib(device, port_num) ||
		rdma_protocol_roce(device, port_num);
}


static inline bool rdma_cap_ib_mad(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_IB_MAD;
}


static inline bool rdma_cap_opa_mad(struct ib_device *device, u8 port_num)
{
	return (device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_OPA_MAD)
		== RDMA_CORE_CAP_OPA_MAD;
}


static inline bool rdma_cap_ib_smi(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_IB_SMI;
}


static inline bool rdma_cap_ib_cm(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_IB_CM;
}


static inline bool rdma_cap_iw_cm(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_IW_CM;
}


static inline bool rdma_cap_ib_sa(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_IB_SA;
}


static inline bool rdma_cap_ib_mcast(const struct ib_device *device, u8 port_num)
{
	return rdma_cap_ib_sa(device, port_num);
}


static inline bool rdma_cap_af_ib(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_AF_IB;
}


static inline bool rdma_cap_eth_ah(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].core_cap_flags & RDMA_CORE_CAP_ETH_AH;
}


static inline size_t rdma_max_mad_size(const struct ib_device *device, u8 port_num)
{
	return device->port_immutable[port_num].max_mad_size;
}


static inline bool rdma_cap_roce_gid_table(const struct ib_device *device,
					   u8 port_num)
{
	return rdma_protocol_roce(device, port_num) &&
		device->add_gid && device->del_gid;
}


static inline bool rdma_cap_read_inv(struct ib_device *dev, u32 port_num)
{
	
	return rdma_protocol_iwarp(dev, port_num);
}

int ib_query_gid(struct ib_device *device,
		 u8 port_num, int index, union ib_gid *gid,
		 struct ib_gid_attr *attr);

int ib_set_vf_link_state(struct ib_device *device, int vf, u8 port,
			 int state);
int ib_get_vf_config(struct ib_device *device, int vf, u8 port,
		     struct ifla_vf_info *info);
int ib_get_vf_stats(struct ib_device *device, int vf, u8 port,
		    struct ifla_vf_stats *stats);
int ib_set_vf_guid(struct ib_device *device, int vf, u8 port, u64 guid,
		   int type);

int ib_query_pkey(struct ib_device *device,
		  u8 port_num, u16 index, u16 *pkey);

int ib_modify_device(struct ib_device *device,
		     int device_modify_mask,
		     struct ib_device_modify *device_modify);

int ib_modify_port(struct ib_device *device,
		   u8 port_num, int port_modify_mask,
		   struct ib_port_modify *port_modify);

int ib_find_gid(struct ib_device *device, union ib_gid *gid,
		enum ib_gid_type gid_type, struct net_device *ndev,
		u8 *port_num, u16 *index);

int ib_find_pkey(struct ib_device *device,
		 u8 port_num, u16 pkey, u16 *index);

enum ib_pd_flags {
	
	IB_PD_UNSAFE_GLOBAL_RKEY	= 0x01,
};

struct ib_pd *__ib_alloc_pd(struct ib_device *device, unsigned int flags,
		const char *caller);
#define ib_alloc_pd(device, flags) \
	__ib_alloc_pd((device), (flags), __func__)
void ib_dealloc_pd(struct ib_pd *pd);


struct ib_ah *ib_create_ah(struct ib_pd *pd, struct ib_ah_attr *ah_attr);


int ib_init_ah_from_wc(struct ib_device *device, u8 port_num,
		       const struct ib_wc *wc, const struct ib_grh *grh,
		       struct ib_ah_attr *ah_attr);


struct ib_ah *ib_create_ah_from_wc(struct ib_pd *pd, const struct ib_wc *wc,
				   const struct ib_grh *grh, u8 port_num);


int ib_modify_ah(struct ib_ah *ah, struct ib_ah_attr *ah_attr);


int ib_query_ah(struct ib_ah *ah, struct ib_ah_attr *ah_attr);


int ib_destroy_ah(struct ib_ah *ah);


struct ib_srq *ib_create_srq(struct ib_pd *pd,
			     struct ib_srq_init_attr *srq_init_attr);


int ib_modify_srq(struct ib_srq *srq,
		  struct ib_srq_attr *srq_attr,
		  enum ib_srq_attr_mask srq_attr_mask);


int ib_query_srq(struct ib_srq *srq,
		 struct ib_srq_attr *srq_attr);


int ib_destroy_srq(struct ib_srq *srq);


static inline int ib_post_srq_recv(struct ib_srq *srq,
				   struct ib_recv_wr *recv_wr,
				   struct ib_recv_wr **bad_recv_wr)
{
	return srq->device->post_srq_recv(srq, recv_wr, bad_recv_wr);
}


struct ib_qp *ib_create_qp(struct ib_pd *pd,
			   struct ib_qp_init_attr *qp_init_attr);


int ib_modify_qp(struct ib_qp *qp,
		 struct ib_qp_attr *qp_attr,
		 int qp_attr_mask);


int ib_query_qp(struct ib_qp *qp,
		struct ib_qp_attr *qp_attr,
		int qp_attr_mask,
		struct ib_qp_init_attr *qp_init_attr);


int ib_destroy_qp(struct ib_qp *qp);


struct ib_qp *ib_open_qp(struct ib_xrcd *xrcd,
			 struct ib_qp_open_attr *qp_open_attr);


int ib_close_qp(struct ib_qp *qp);


static inline int ib_post_send(struct ib_qp *qp,
			       struct ib_send_wr *send_wr,
			       struct ib_send_wr **bad_send_wr)
{
	return qp->device->post_send(qp, send_wr, bad_send_wr);
}


static inline int ib_post_recv(struct ib_qp *qp,
			       struct ib_recv_wr *recv_wr,
			       struct ib_recv_wr **bad_recv_wr)
{
	return qp->device->post_recv(qp, recv_wr, bad_recv_wr);
}

struct ib_cq *ib_alloc_cq(struct ib_device *dev, void *private,
		int nr_cqe, int comp_vector, enum ib_poll_context poll_ctx);
void ib_free_cq(struct ib_cq *cq);
int ib_process_cq_direct(struct ib_cq *cq, int budget);


struct ib_cq *ib_create_cq(struct ib_device *device,
			   ib_comp_handler comp_handler,
			   void (*event_handler)(struct ib_event *, void *),
			   void *cq_context,
			   const struct ib_cq_init_attr *cq_attr);


int ib_resize_cq(struct ib_cq *cq, int cqe);


int ib_modify_cq(struct ib_cq *cq, u16 cq_count, u16 cq_period);


int ib_destroy_cq(struct ib_cq *cq);


static inline int ib_poll_cq(struct ib_cq *cq, int num_entries,
			     struct ib_wc *wc)
{
	return cq->device->poll_cq(cq, num_entries, wc);
}


int ib_peek_cq(struct ib_cq *cq, int wc_cnt);


static inline int ib_req_notify_cq(struct ib_cq *cq,
				   enum ib_cq_notify_flags flags)
{
	return cq->device->req_notify_cq(cq, flags);
}


static inline int ib_req_ncomp_notif(struct ib_cq *cq, int wc_cnt)
{
	return cq->device->req_ncomp_notif ?
		cq->device->req_ncomp_notif(cq, wc_cnt) :
		-ENOSYS;
}


static inline int ib_dma_mapping_error(struct ib_device *dev, u64 dma_addr)
{
	if (dev->dma_ops)
		return dev->dma_ops->mapping_error(dev, dma_addr);
	return dma_mapping_error(dev->dma_device, dma_addr);
}


static inline u64 ib_dma_map_single(struct ib_device *dev,
				    void *cpu_addr, size_t size,
				    enum dma_data_direction direction)
{
	if (dev->dma_ops)
		return dev->dma_ops->map_single(dev, cpu_addr, size, direction);
	return dma_map_single(dev->dma_device, cpu_addr, size, direction);
}


static inline void ib_dma_unmap_single(struct ib_device *dev,
				       u64 addr, size_t size,
				       enum dma_data_direction direction)
{
	if (dev->dma_ops)
		dev->dma_ops->unmap_single(dev, addr, size, direction);
	else
		dma_unmap_single(dev->dma_device, addr, size, direction);
}

static inline u64 ib_dma_map_single_attrs(struct ib_device *dev,
					  void *cpu_addr, size_t size,
					  enum dma_data_direction direction,
					  unsigned long dma_attrs)
{
	return dma_map_single_attrs(dev->dma_device, cpu_addr, size,
				    direction, dma_attrs);
}

static inline void ib_dma_unmap_single_attrs(struct ib_device *dev,
					     u64 addr, size_t size,
					     enum dma_data_direction direction,
					     unsigned long dma_attrs)
{
	return dma_unmap_single_attrs(dev->dma_device, addr, size,
				      direction, dma_attrs);
}


static inline u64 ib_dma_map_page(struct ib_device *dev,
				  struct page *page,
				  unsigned long offset,
				  size_t size,
					 enum dma_data_direction direction)
{
	if (dev->dma_ops)
		return dev->dma_ops->map_page(dev, page, offset, size, direction);
	return dma_map_page(dev->dma_device, page, offset, size, direction);
}


static inline void ib_dma_unmap_page(struct ib_device *dev,
				     u64 addr, size_t size,
				     enum dma_data_direction direction)
{
	if (dev->dma_ops)
		dev->dma_ops->unmap_page(dev, addr, size, direction);
	else
		dma_unmap_page(dev->dma_device, addr, size, direction);
}


static inline int ib_dma_map_sg(struct ib_device *dev,
				struct scatterlist *sg, int nents,
				enum dma_data_direction direction)
{
	if (dev->dma_ops)
		return dev->dma_ops->map_sg(dev, sg, nents, direction);
	return dma_map_sg(dev->dma_device, sg, nents, direction);
}


static inline void ib_dma_unmap_sg(struct ib_device *dev,
				   struct scatterlist *sg, int nents,
				   enum dma_data_direction direction)
{
	if (dev->dma_ops)
		dev->dma_ops->unmap_sg(dev, sg, nents, direction);
	else
		dma_unmap_sg(dev->dma_device, sg, nents, direction);
}

static inline int ib_dma_map_sg_attrs(struct ib_device *dev,
				      struct scatterlist *sg, int nents,
				      enum dma_data_direction direction,
				      unsigned long dma_attrs)
{
	if (dev->dma_ops)
		return dev->dma_ops->map_sg_attrs(dev, sg, nents, direction,
						  dma_attrs);
	else
		return dma_map_sg_attrs(dev->dma_device, sg, nents, direction,
					dma_attrs);
}

static inline void ib_dma_unmap_sg_attrs(struct ib_device *dev,
					 struct scatterlist *sg, int nents,
					 enum dma_data_direction direction,
					 unsigned long dma_attrs)
{
	if (dev->dma_ops)
		return dev->dma_ops->unmap_sg_attrs(dev, sg, nents, direction,
						  dma_attrs);
	else
		dma_unmap_sg_attrs(dev->dma_device, sg, nents, direction,
				   dma_attrs);
}

static inline u64 ib_sg_dma_address(struct ib_device *dev,
				    struct scatterlist *sg)
{
	return sg_dma_address(sg);
}


static inline unsigned int ib_sg_dma_len(struct ib_device *dev,
					 struct scatterlist *sg)
{
	return sg_dma_len(sg);
}


static inline void ib_dma_sync_single_for_cpu(struct ib_device *dev,
					      u64 addr,
					      size_t size,
					      enum dma_data_direction dir)
{
	if (dev->dma_ops)
		dev->dma_ops->sync_single_for_cpu(dev, addr, size, dir);
	else
		dma_sync_single_for_cpu(dev->dma_device, addr, size, dir);
}


static inline void ib_dma_sync_single_for_device(struct ib_device *dev,
						 u64 addr,
						 size_t size,
						 enum dma_data_direction dir)
{
	if (dev->dma_ops)
		dev->dma_ops->sync_single_for_device(dev, addr, size, dir);
	else
		dma_sync_single_for_device(dev->dma_device, addr, size, dir);
}


static inline void *ib_dma_alloc_coherent(struct ib_device *dev,
					   size_t size,
					   u64 *dma_handle,
					   gfp_t flag)
{
	if (dev->dma_ops)
		return dev->dma_ops->alloc_coherent(dev, size, dma_handle, flag);
	else {
		dma_addr_t handle;
		void *ret;

		ret = dma_alloc_coherent(dev->dma_device, size, &handle, flag);
		*dma_handle = handle;
		return ret;
	}
}


static inline void ib_dma_free_coherent(struct ib_device *dev,
					size_t size, void *cpu_addr,
					u64 dma_handle)
{
	if (dev->dma_ops)
		dev->dma_ops->free_coherent(dev, size, cpu_addr, dma_handle);
	else
		dma_free_coherent(dev->dma_device, size, cpu_addr, dma_handle);
}


int ib_dereg_mr(struct ib_mr *mr);

struct ib_mr *ib_alloc_mr(struct ib_pd *pd,
			  enum ib_mr_type mr_type,
			  u32 max_num_sg);


static inline void ib_update_fast_reg_key(struct ib_mr *mr, u8 newkey)
{
	mr->lkey = (mr->lkey & 0xffffff00) | newkey;
	mr->rkey = (mr->rkey & 0xffffff00) | newkey;
}


static inline u32 ib_inc_rkey(u32 rkey)
{
	const u32 mask = 0x000000ff;
	return ((rkey + 1) & mask) | (rkey & ~mask);
}


struct ib_fmr *ib_alloc_fmr(struct ib_pd *pd,
			    int mr_access_flags,
			    struct ib_fmr_attr *fmr_attr);


static inline int ib_map_phys_fmr(struct ib_fmr *fmr,
				  u64 *page_list, int list_len,
				  u64 iova)
{
	return fmr->device->map_phys_fmr(fmr, page_list, list_len, iova);
}


int ib_unmap_fmr(struct list_head *fmr_list);


int ib_dealloc_fmr(struct ib_fmr *fmr);


int ib_attach_mcast(struct ib_qp *qp, union ib_gid *gid, u16 lid);


int ib_detach_mcast(struct ib_qp *qp, union ib_gid *gid, u16 lid);


struct ib_xrcd *ib_alloc_xrcd(struct ib_device *device);


int ib_dealloc_xrcd(struct ib_xrcd *xrcd);

struct ib_flow *ib_create_flow(struct ib_qp *qp,
			       struct ib_flow_attr *flow_attr, int domain);
int ib_destroy_flow(struct ib_flow *flow_id);

static inline int ib_check_mr_access(int flags)
{
	
	if (flags & (IB_ACCESS_REMOTE_ATOMIC | IB_ACCESS_REMOTE_WRITE) &&
	    !(flags & IB_ACCESS_LOCAL_WRITE))
		return -EINVAL;

	return 0;
}

static inline bool ib_access_writable(int access_flags)
{
	
	return access_flags &
		(IB_ACCESS_LOCAL_WRITE   | IB_ACCESS_REMOTE_WRITE |
		 IB_ACCESS_REMOTE_ATOMIC | IB_ACCESS_MW_BIND);
}


int ib_check_mr_status(struct ib_mr *mr, u32 check_mask,
		       struct ib_mr_status *mr_status);

struct net_device *ib_get_net_dev_by_params(struct ib_device *dev, u8 port,
					    u16 pkey, const union ib_gid *gid,
					    const struct sockaddr *addr);
struct ib_wq *ib_create_wq(struct ib_pd *pd,
			   struct ib_wq_init_attr *init_attr);
int ib_destroy_wq(struct ib_wq *wq);
int ib_modify_wq(struct ib_wq *wq, struct ib_wq_attr *attr,
		 u32 wq_attr_mask);
struct ib_rwq_ind_table *ib_create_rwq_ind_table(struct ib_device *device,
						 struct ib_rwq_ind_table_init_attr*
						 wq_ind_table_init_attr);
int ib_destroy_rwq_ind_table(struct ib_rwq_ind_table *wq_ind_table);

int ib_map_mr_sg(struct ib_mr *mr, struct scatterlist *sg, int sg_nents,
		 unsigned int *sg_offset, unsigned int page_size);

static inline int
ib_map_mr_sg_zbva(struct ib_mr *mr, struct scatterlist *sg, int sg_nents,
		  unsigned int *sg_offset, unsigned int page_size)
{
	int n;

	n = ib_map_mr_sg(mr, sg, sg_nents, sg_offset, page_size);
	mr->iova = 0;

	return n;
}

int ib_sg_to_pages(struct ib_mr *mr, struct scatterlist *sgl, int sg_nents,
		unsigned int *sg_offset, int (*set_page)(struct ib_mr *, u64));

void ib_drain_rq(struct ib_qp *qp);
void ib_drain_sq(struct ib_qp *qp);
void ib_drain_qp(struct ib_qp *qp);
#endif 
