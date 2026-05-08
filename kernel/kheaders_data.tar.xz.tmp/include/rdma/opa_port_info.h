

#if !defined(OPA_PORT_INFO_H)
#define OPA_PORT_INFO_H

#define OPA_PORT_LINK_MODE_NOP	0		
#define OPA_PORT_LINK_MODE_OPA	4		

#define OPA_PORT_PACKET_FORMAT_NOP	0		
#define OPA_PORT_PACKET_FORMAT_8B	1		
#define OPA_PORT_PACKET_FORMAT_9B	2		
#define OPA_PORT_PACKET_FORMAT_10B	4		
#define OPA_PORT_PACKET_FORMAT_16B	8		

#define OPA_PORT_LTP_CRC_MODE_NONE	0	
#define OPA_PORT_LTP_CRC_MODE_14	1	
#define OPA_PORT_LTP_CRC_MODE_16	2	
#define OPA_PORT_LTP_CRC_MODE_48	4	
#define OPA_PORT_LTP_CRC_MODE_PER_LANE  8	


#define OPA_LINKDOWN_REASON_NONE				0	
#define OPA_LINKDOWN_REASON_RCV_ERROR_0				1
#define OPA_LINKDOWN_REASON_BAD_PKT_LEN				2
#define OPA_LINKDOWN_REASON_PKT_TOO_LONG			3
#define OPA_LINKDOWN_REASON_PKT_TOO_SHORT			4
#define OPA_LINKDOWN_REASON_BAD_SLID				5
#define OPA_LINKDOWN_REASON_BAD_DLID				6
#define OPA_LINKDOWN_REASON_BAD_L2				7
#define OPA_LINKDOWN_REASON_BAD_SC				8
#define OPA_LINKDOWN_REASON_RCV_ERROR_8				9
#define OPA_LINKDOWN_REASON_BAD_MID_TAIL			10
#define OPA_LINKDOWN_REASON_RCV_ERROR_10			11
#define OPA_LINKDOWN_REASON_PREEMPT_ERROR			12
#define OPA_LINKDOWN_REASON_PREEMPT_VL15			13
#define OPA_LINKDOWN_REASON_BAD_VL_MARKER			14
#define OPA_LINKDOWN_REASON_RCV_ERROR_14			15
#define OPA_LINKDOWN_REASON_RCV_ERROR_15			16
#define OPA_LINKDOWN_REASON_BAD_HEAD_DIST			17
#define OPA_LINKDOWN_REASON_BAD_TAIL_DIST			18
#define OPA_LINKDOWN_REASON_BAD_CTRL_DIST			19
#define OPA_LINKDOWN_REASON_BAD_CREDIT_ACK			20
#define OPA_LINKDOWN_REASON_UNSUPPORTED_VL_MARKER		21
#define OPA_LINKDOWN_REASON_BAD_PREEMPT				22
#define OPA_LINKDOWN_REASON_BAD_CONTROL_FLIT			23
#define OPA_LINKDOWN_REASON_EXCEED_MULTICAST_LIMIT		24
#define OPA_LINKDOWN_REASON_RCV_ERROR_24			25
#define OPA_LINKDOWN_REASON_RCV_ERROR_25			26
#define OPA_LINKDOWN_REASON_RCV_ERROR_26			27
#define OPA_LINKDOWN_REASON_RCV_ERROR_27			28
#define OPA_LINKDOWN_REASON_RCV_ERROR_28			29
#define OPA_LINKDOWN_REASON_RCV_ERROR_29			30
#define OPA_LINKDOWN_REASON_RCV_ERROR_30			31
#define OPA_LINKDOWN_REASON_EXCESSIVE_BUFFER_OVERRUN		32
#define OPA_LINKDOWN_REASON_UNKNOWN				33

#define OPA_LINKDOWN_REASON_REBOOT				35
#define OPA_LINKDOWN_REASON_NEIGHBOR_UNKNOWN			36

#define OPA_LINKDOWN_REASON_FM_BOUNCE				39
#define OPA_LINKDOWN_REASON_SPEED_POLICY			40
#define OPA_LINKDOWN_REASON_WIDTH_POLICY			41

#define OPA_LINKDOWN_REASON_DISCONNECTED			49
#define OPA_LINKDOWN_REASON_LOCAL_MEDIA_NOT_INSTALLED		50
#define OPA_LINKDOWN_REASON_NOT_INSTALLED			51
#define OPA_LINKDOWN_REASON_CHASSIS_CONFIG			52

#define OPA_LINKDOWN_REASON_END_TO_END_NOT_INSTALLED		54

#define OPA_LINKDOWN_REASON_POWER_POLICY			56
#define OPA_LINKDOWN_REASON_LINKSPEED_POLICY			57
#define OPA_LINKDOWN_REASON_LINKWIDTH_POLICY			58

#define OPA_LINKDOWN_REASON_SWITCH_MGMT				60
#define OPA_LINKDOWN_REASON_SMA_DISABLED			61

#define OPA_LINKDOWN_REASON_TRANSIENT				63




#define OPA_LINKINIT_REASON_NOP                 0
#define OPA_LINKINIT_REASON_LINKUP              (1 << 4)
#define OPA_LINKINIT_REASON_FLAPPING            (2 << 4)
#define OPA_LINKINIT_REASON_CLEAR               (8 << 4)
#define OPA_LINKINIT_OUTSIDE_POLICY             (8 << 4)
#define OPA_LINKINIT_QUARANTINED                (9 << 4)
#define OPA_LINKINIT_INSUFIC_CAPABILITY         (10 << 4)

#define OPA_LINK_SPEED_NOP              0x0000  
#define OPA_LINK_SPEED_12_5G            0x0001  
#define OPA_LINK_SPEED_25G              0x0002  

#define OPA_LINK_WIDTH_1X            0x0001
#define OPA_LINK_WIDTH_2X            0x0002
#define OPA_LINK_WIDTH_3X            0x0004
#define OPA_LINK_WIDTH_4X            0x0008

#define OPA_CAP_MASK3_IsSnoopSupported            (1 << 7)
#define OPA_CAP_MASK3_IsAsyncSC2VLSupported       (1 << 6)
#define OPA_CAP_MASK3_IsAddrRangeConfigSupported  (1 << 5)
#define OPA_CAP_MASK3_IsPassThroughSupported      (1 << 4)
#define OPA_CAP_MASK3_IsSharedSpaceSupported      (1 << 3)

#define OPA_CAP_MASK3_IsVLMarkerSupported         (1 << 1)
#define OPA_CAP_MASK3_IsVLrSupported              (1 << 0)


enum {
	OPA_MTU_8192  = 6,
	OPA_MTU_10240 = 7,
};

enum {
	OPA_PORT_PHYS_CONF_DISCONNECTED = 0,
	OPA_PORT_PHYS_CONF_STANDARD     = 1,
	OPA_PORT_PHYS_CONF_FIXED        = 2,
	OPA_PORT_PHYS_CONF_VARIABLE     = 3,
	OPA_PORT_PHYS_CONF_SI_PHOTO     = 4
};

enum port_info_field_masks {
	
	OPA_PI_MASK_VL_CAP                        = 0x1F,
	
	OPA_PI_MASK_OFFLINE_REASON                = 0x0F,
	OPA_PI_MASK_LED_ENABLE                    = 0x40,
	
	OPA_PI_MASK_UNSLEEP_STATE                 = 0xF0,
	OPA_PI_MASK_DOWNDEF_STATE                 = 0x0F,
	
	OPA_PI_MASK_PORT_PHYSICAL_STATE           = 0xF0,
	OPA_PI_MASK_PORT_STATE                    = 0x0F,
	
	OPA_PI_MASK_PORT_PHYSICAL_CONF            = 0x0F,
	
	OPA_PI_MASK_COLLECT_MASK                  = 0x38,
	OPA_PI_MASK_MULTICAST_MASK                = 0x07,
	
	OPA_PI_MASK_MKEY_PROT_BIT                 = 0xC0,
	OPA_PI_MASK_LMC                           = 0x0F,
	
	OPA_PI_MASK_SMSL                          = 0x1F,
	
	
	OPA_PI_MASK_LINKINIT_REASON               = 0xF0,
	OPA_PI_MASK_PARTITION_ENFORCE_IN          = 0x08,
	OPA_PI_MASK_PARTITION_ENFORCE_OUT         = 0x04,
	
	OPA_PI_MASK_OPERATIONAL_VL                = 0x1F,
	
	OPA_PI_MASK_SA_QP                         = 0x00FFFFFF,
	
	OPA_PI_MASK_SM_TRAP_QP                    = 0x00FFFFFF,
	
	OPA_PI_MASK_LOCAL_PHY_ERRORS              = 0xF0,
	OPA_PI_MASK_OVERRUN_ERRORS                = 0x0F,
	
	OPA_PI_MASK_CLIENT_REREGISTER             = 0x80,
	OPA_PI_MASK_SUBNET_TIMEOUT                = 0x1F,
	
	OPA_PI_MASK_PORT_LINK_SUPPORTED           = (0x001F << 10),
	OPA_PI_MASK_PORT_LINK_ENABLED             = (0x001F <<  5),
	OPA_PI_MASK_PORT_LINK_ACTIVE              = (0x001F <<  0),
	
	OPA_PI_MASK_PORT_LINK_CRC_SUPPORTED       = 0x0F00,
	OPA_PI_MASK_PORT_LINK_CRC_ENABLED         = 0x00F0,
	OPA_PI_MASK_PORT_LINK_CRC_ACTIVE          = 0x000F,
	
	OPA_PI_MASK_PORT_MODE_SECURITY_CHECK      = 0x0001,
	OPA_PI_MASK_PORT_MODE_16B_TRAP_QUERY      = 0x0002,
	OPA_PI_MASK_PORT_MODE_PKEY_CONVERT        = 0x0004,
	OPA_PI_MASK_PORT_MODE_SC2SC_MAPPING       = 0x0008,
	OPA_PI_MASK_PORT_MODE_VL_MARKER           = 0x0010,
	OPA_PI_MASK_PORT_PASS_THROUGH             = 0x0020,
	OPA_PI_MASK_PORT_ACTIVE_OPTOMIZE          = 0x0040,
	
	OPA_PI_MASK_INTERLEAVE_DIST_SUP           = (0x0003 << 12),
	OPA_PI_MASK_INTERLEAVE_DIST_ENABLE        = (0x0003 << 10),
	OPA_PI_MASK_INTERLEAVE_MAX_NEST_TX        = (0x001F <<  5),
	OPA_PI_MASK_INTERLEAVE_MAX_NEST_RX        = (0x001F <<  0),

	
	OPA_PI_MASK_EX_BUFFER_OVERRUN                  = 0x80000000,
		
	OPA_PI_MASK_FM_CFG_ERR_EXCEED_MULTICAST_LIMIT  = 0x00800000,
	OPA_PI_MASK_FM_CFG_BAD_CONTROL_FLIT            = 0x00400000,
	OPA_PI_MASK_FM_CFG_BAD_PREEMPT                 = 0x00200000,
	OPA_PI_MASK_FM_CFG_UNSUPPORTED_VL_MARKER       = 0x00100000,
	OPA_PI_MASK_FM_CFG_BAD_CRDT_ACK                = 0x00080000,
	OPA_PI_MASK_FM_CFG_BAD_CTRL_DIST               = 0x00040000,
	OPA_PI_MASK_FM_CFG_BAD_TAIL_DIST               = 0x00020000,
	OPA_PI_MASK_FM_CFG_BAD_HEAD_DIST               = 0x00010000,
		
	OPA_PI_MASK_PORT_RCV_BAD_VL_MARKER             = 0x00002000,
	OPA_PI_MASK_PORT_RCV_PREEMPT_VL15              = 0x00001000,
	OPA_PI_MASK_PORT_RCV_PREEMPT_ERROR             = 0x00000800,
		
	OPA_PI_MASK_PORT_RCV_BAD_MidTail               = 0x00000200,
		
	OPA_PI_MASK_PORT_RCV_BAD_SC                    = 0x00000080,
	OPA_PI_MASK_PORT_RCV_BAD_L2                    = 0x00000040,
	OPA_PI_MASK_PORT_RCV_BAD_DLID                  = 0x00000020,
	OPA_PI_MASK_PORT_RCV_BAD_SLID                  = 0x00000010,
	OPA_PI_MASK_PORT_RCV_PKTLEN_TOOSHORT           = 0x00000008,
	OPA_PI_MASK_PORT_RCV_PKTLEN_TOOLONG            = 0x00000004,
	OPA_PI_MASK_PORT_RCV_BAD_PKTLEN                = 0x00000002,
	OPA_PI_MASK_PORT_RCV_BAD_LT                    = 0x00000001,

	
	OPA_PI_MASK_PASS_THROUGH_DR_CONTROL       = 0x01,

	
	OPA_PI_MASK_BUF_UNIT_VL15_INIT            = (0x00000FFF  << 11),
	OPA_PI_MASK_BUF_UNIT_VL15_CREDIT_RATE     = (0x0000001F  <<  6),
	OPA_PI_MASK_BUF_UNIT_CREDIT_ACK           = (0x00000003  <<  3),
	OPA_PI_MASK_BUF_UNIT_BUF_ALLOC            = (0x00000003  <<  0),

	
	OPA_PI_MASK_NEIGH_MTU_PVL0                = 0xF0,
	OPA_PI_MASK_NEIGH_MTU_PVL1                = 0x0F,

	
	OPA_PI_MASK_VL_STALL                      = (0x03 << 5),
	OPA_PI_MASK_HOQ_LIFE                      = (0x1F << 0),

	
	OPA_PI_MASK_NEIGH_MGMT_ALLOWED            = (0x01 << 3),
	OPA_PI_MASK_NEIGH_FW_AUTH_BYPASS          = (0x01 << 2),
	OPA_PI_MASK_NEIGH_NODE_TYPE               = (0x03 << 0),

	
	OPA_PI_MASK_RESPONSE_TIME_VALUE           = 0x1F,

	
	OPA_PI_MASK_MTU_CAP                       = 0x0F,
};

struct opa_port_states {
	u8     reserved;
	u8     ledenable_offlinereason;   
	u8     reserved2;
	u8     portphysstate_portstate;   
};

struct opa_port_state_info {
	struct opa_port_states port_states;
	__be16 link_width_downgrade_tx_active;
	__be16 link_width_downgrade_rx_active;
};

struct opa_port_info {
	__be32 lid;
	__be32 flow_control_mask;

	struct {
		u8     res;                       
		u8     cap;                       
		__be16 high_limit;
		__be16 preempt_limit;
		u8     arb_high_cap;
		u8     arb_low_cap;
	} vl;

	struct opa_port_states  port_states;
	u8     port_phys_conf;                    
	u8     collectivemask_multicastmask;      
	u8     mkeyprotect_lmc;                   
	u8     smsl;                              

	u8     partenforce_filterraw;             
	u8     operational_vls;                    
	__be16 pkey_8b;
	__be16 pkey_10b;
	__be16 mkey_violations;

	__be16 pkey_violations;
	__be16 qkey_violations;
	__be32 sm_trap_qp;                        

	__be32 sa_qp;                             
	u8     neigh_port_num;
	u8     link_down_reason;
	u8     neigh_link_down_reason;
	u8     clientrereg_subnettimeout;	  

	struct {
		__be16 supported;
		__be16 enabled;
		__be16 active;
	} link_speed;
	struct {
		__be16 supported;
		__be16 enabled;
		__be16 active;
	} link_width;
	struct {
		__be16 supported;
		__be16 enabled;
		__be16 tx_active;
		__be16 rx_active;
	} link_width_downgrade;
	__be16 port_link_mode;                  
	__be16 port_ltp_crc_mode;               

	__be16 port_mode;                       
	struct {
		__be16 supported;
		__be16 enabled;
	} port_packet_format;
	struct {
		__be16 interleave;  
		struct {
			__be16 min_initial;
			__be16 min_tail;
			u8     large_pkt_limit;
			u8     small_pkt_limit;
			u8     max_small_pkt_limit;
			u8     preemption_limit;
		} preemption;
	} flit_control;

	__be32 reserved4;
	__be32 port_error_action; 

	struct {
		u8 egress_port;
		u8 res_drctl;                    
	} pass_through;
	__be16 mkey_lease_period;
	__be32 buffer_units;                     

	__be32 reserved5;
	__be32 sm_lid;

	__be64 mkey;

	__be64 subnet_prefix;

	struct {
		u8 pvlx_to_mtu[OPA_MAX_VLS/2]; 
	} neigh_mtu;

	struct {
		u8 vlstall_hoqlife;             
	} xmit_q[OPA_MAX_VLS];

	struct {
		u8 addr[16];
	} ipaddr_ipv6;

	struct {
		u8 addr[4];
	} ipaddr_ipv4;

	u32    reserved6;
	u32    reserved7;
	u32    reserved8;

	__be64 neigh_node_guid;

	__be32 ib_cap_mask;
	__be16 reserved9;                    
	__be16 opa_cap_mask;

	__be32 reserved10;                   
	__be16 overall_buffer_space;
	__be16 reserved11;                   

	__be16 diag_code;
	struct {
		u8 buffer;
		u8 wire;
	} replay_depth;
	u8     port_neigh_mode;
	u8     mtucap;                          

	u8     resptimevalue;		        
	u8     local_port_num;
	u8     reserved12;
	u8     reserved13;                       
} __attribute__ ((packed));

#endif 
