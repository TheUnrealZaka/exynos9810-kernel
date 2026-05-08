

#if !defined(IB_MAD_H)
#define IB_MAD_H

#include <linux/list.h>

#include <rdma/ib_verbs.h>
#include <uapi/rdma/ib_user_mad.h>


#define IB_MGMT_BASE_VERSION			1
#define OPA_MGMT_BASE_VERSION			0x80

#define OPA_SMP_CLASS_VERSION			0x80


#define IB_MGMT_CLASS_SUBN_LID_ROUTED		0x01
#define IB_MGMT_CLASS_SUBN_DIRECTED_ROUTE	0x81
#define IB_MGMT_CLASS_SUBN_ADM			0x03
#define IB_MGMT_CLASS_PERF_MGMT			0x04
#define IB_MGMT_CLASS_BM			0x05
#define IB_MGMT_CLASS_DEVICE_MGMT		0x06
#define IB_MGMT_CLASS_CM			0x07
#define IB_MGMT_CLASS_SNMP			0x08
#define IB_MGMT_CLASS_DEVICE_ADM		0x10
#define IB_MGMT_CLASS_BOOT_MGMT			0x11
#define IB_MGMT_CLASS_BIS			0x12
#define IB_MGMT_CLASS_CONG_MGMT			0x21
#define IB_MGMT_CLASS_VENDOR_RANGE2_START	0x30
#define IB_MGMT_CLASS_VENDOR_RANGE2_END		0x4F

#define	IB_OPENIB_OUI				(0x001405)


#define IB_MGMT_METHOD_GET			0x01
#define IB_MGMT_METHOD_SET			0x02
#define IB_MGMT_METHOD_GET_RESP			0x81
#define IB_MGMT_METHOD_SEND			0x03
#define IB_MGMT_METHOD_TRAP			0x05
#define IB_MGMT_METHOD_REPORT			0x06
#define IB_MGMT_METHOD_REPORT_RESP		0x86
#define IB_MGMT_METHOD_TRAP_REPRESS		0x07

#define IB_MGMT_METHOD_RESP			0x80
#define IB_BM_ATTR_MOD_RESP			cpu_to_be32(1)

#define IB_MGMT_MAX_METHODS			128


#define IB_MGMT_MAD_STATUS_SUCCESS			0x0000
#define IB_MGMT_MAD_STATUS_BUSY				0x0001
#define IB_MGMT_MAD_STATUS_REDIRECT_REQD		0x0002
#define IB_MGMT_MAD_STATUS_BAD_VERSION			0x0004
#define IB_MGMT_MAD_STATUS_UNSUPPORTED_METHOD		0x0008
#define IB_MGMT_MAD_STATUS_UNSUPPORTED_METHOD_ATTRIB	0x000c
#define IB_MGMT_MAD_STATUS_INVALID_ATTRIB_VALUE		0x001c


#define IB_MGMT_RMPP_VERSION			1

#define IB_MGMT_RMPP_TYPE_DATA			1
#define IB_MGMT_RMPP_TYPE_ACK			2
#define IB_MGMT_RMPP_TYPE_STOP			3
#define IB_MGMT_RMPP_TYPE_ABORT			4

#define IB_MGMT_RMPP_FLAG_ACTIVE		1
#define IB_MGMT_RMPP_FLAG_FIRST			(1<<1)
#define IB_MGMT_RMPP_FLAG_LAST			(1<<2)

#define IB_MGMT_RMPP_NO_RESPTIME		0x1F

#define	IB_MGMT_RMPP_STATUS_SUCCESS		0
#define	IB_MGMT_RMPP_STATUS_RESX		1
#define	IB_MGMT_RMPP_STATUS_ABORT_MIN		118
#define	IB_MGMT_RMPP_STATUS_T2L			118
#define	IB_MGMT_RMPP_STATUS_BAD_LEN		119
#define	IB_MGMT_RMPP_STATUS_BAD_SEG		120
#define	IB_MGMT_RMPP_STATUS_BADT		121
#define	IB_MGMT_RMPP_STATUS_W2S			122
#define	IB_MGMT_RMPP_STATUS_S2B			123
#define	IB_MGMT_RMPP_STATUS_BAD_STATUS		124
#define	IB_MGMT_RMPP_STATUS_UNV			125
#define	IB_MGMT_RMPP_STATUS_TMR			126
#define	IB_MGMT_RMPP_STATUS_UNSPEC		127
#define	IB_MGMT_RMPP_STATUS_ABORT_MAX		127

#define IB_QP0		0
#define IB_QP1		cpu_to_be32(1)
#define IB_QP1_QKEY	0x80010000
#define IB_QP_SET_QKEY	0x80000000

#define IB_DEFAULT_PKEY_PARTIAL 0x7FFF
#define IB_DEFAULT_PKEY_FULL	0xFFFF


#define IB_NOTICE_TYPE_FATAL	0x80
#define IB_NOTICE_TYPE_URGENT	0x81
#define IB_NOTICE_TYPE_SECURITY	0x82
#define IB_NOTICE_TYPE_SM	0x83
#define IB_NOTICE_TYPE_INFO	0x84


#define IB_NOTICE_PROD_CA		cpu_to_be16(1)
#define IB_NOTICE_PROD_SWITCH		cpu_to_be16(2)
#define IB_NOTICE_PROD_ROUTER		cpu_to_be16(3)
#define IB_NOTICE_PROD_CLASS_MGR	cpu_to_be16(4)

enum {
	IB_MGMT_MAD_HDR = 24,
	IB_MGMT_MAD_DATA = 232,
	IB_MGMT_RMPP_HDR = 36,
	IB_MGMT_RMPP_DATA = 220,
	IB_MGMT_VENDOR_HDR = 40,
	IB_MGMT_VENDOR_DATA = 216,
	IB_MGMT_SA_HDR = 56,
	IB_MGMT_SA_DATA = 200,
	IB_MGMT_DEVICE_HDR = 64,
	IB_MGMT_DEVICE_DATA = 192,
	IB_MGMT_MAD_SIZE = IB_MGMT_MAD_HDR + IB_MGMT_MAD_DATA,
	OPA_MGMT_MAD_DATA = 2024,
	OPA_MGMT_RMPP_DATA = 2012,
	OPA_MGMT_MAD_SIZE = IB_MGMT_MAD_HDR + OPA_MGMT_MAD_DATA,
};

struct ib_mad_hdr {
	u8	base_version;
	u8	mgmt_class;
	u8	class_version;
	u8	method;
	__be16	status;
	__be16	class_specific;
	__be64	tid;
	__be16	attr_id;
	__be16	resv;
	__be32	attr_mod;
};

struct ib_rmpp_hdr {
	u8	rmpp_version;
	u8	rmpp_type;
	u8	rmpp_rtime_flags;
	u8	rmpp_status;
	__be32	seg_num;
	__be32	paylen_newwin;
};

typedef u64 __bitwise ib_sa_comp_mask;

#define IB_SA_COMP_MASK(n) ((__force ib_sa_comp_mask) cpu_to_be64(1ull << (n)))


struct ib_sa_hdr {
	__be64			sm_key;
	__be16			attr_offset;
	__be16			reserved;
	ib_sa_comp_mask		comp_mask;
} __attribute__ ((packed));

struct ib_mad {
	struct ib_mad_hdr	mad_hdr;
	u8			data[IB_MGMT_MAD_DATA];
};

struct opa_mad {
	struct ib_mad_hdr	mad_hdr;
	u8			data[OPA_MGMT_MAD_DATA];
};

struct ib_rmpp_mad {
	struct ib_mad_hdr	mad_hdr;
	struct ib_rmpp_hdr	rmpp_hdr;
	u8			data[IB_MGMT_RMPP_DATA];
};

struct opa_rmpp_mad {
	struct ib_mad_hdr	mad_hdr;
	struct ib_rmpp_hdr	rmpp_hdr;
	u8			data[OPA_MGMT_RMPP_DATA];
};

struct ib_sa_mad {
	struct ib_mad_hdr	mad_hdr;
	struct ib_rmpp_hdr	rmpp_hdr;
	struct ib_sa_hdr	sa_hdr;
	u8			data[IB_MGMT_SA_DATA];
} __attribute__ ((packed));

struct ib_vendor_mad {
	struct ib_mad_hdr	mad_hdr;
	struct ib_rmpp_hdr	rmpp_hdr;
	u8			reserved;
	u8			oui[3];
	u8			data[IB_MGMT_VENDOR_DATA];
};

#define IB_MGMT_CLASSPORTINFO_ATTR_ID	cpu_to_be16(0x0001)

#define IB_CLASS_PORT_INFO_RESP_TIME_MASK	0x1F
#define IB_CLASS_PORT_INFO_RESP_TIME_FIELD_SIZE 5

struct ib_class_port_info {
	u8			base_version;
	u8			class_version;
	__be16			capability_mask;
	  
	__be32			cap_mask2_resp_time;
	u8			redirect_gid[16];
	__be32			redirect_tcslfl;
	__be16			redirect_lid;
	__be16			redirect_pkey;
	__be32			redirect_qp;
	__be32			redirect_qkey;
	u8			trap_gid[16];
	__be32			trap_tcslfl;
	__be16			trap_lid;
	__be16			trap_pkey;
	__be32			trap_hlqp;
	__be32			trap_qkey;
};


static inline u8 ib_get_cpi_resp_time(struct ib_class_port_info *cpi)
{
	return (u8)(be32_to_cpu(cpi->cap_mask2_resp_time) &
		    IB_CLASS_PORT_INFO_RESP_TIME_MASK);
}


static inline void ib_set_cpi_resp_time(struct ib_class_port_info *cpi,
					u8 rtime)
{
	cpi->cap_mask2_resp_time =
		(cpi->cap_mask2_resp_time &
		 cpu_to_be32(~IB_CLASS_PORT_INFO_RESP_TIME_MASK)) |
		cpu_to_be32(rtime & IB_CLASS_PORT_INFO_RESP_TIME_MASK);
}


static inline u32 ib_get_cpi_capmask2(struct ib_class_port_info *cpi)
{
	return (be32_to_cpu(cpi->cap_mask2_resp_time) >>
		IB_CLASS_PORT_INFO_RESP_TIME_FIELD_SIZE);
}


static inline void ib_set_cpi_capmask2(struct ib_class_port_info *cpi,
				       u32 capmask2)
{
	cpi->cap_mask2_resp_time =
		(cpi->cap_mask2_resp_time &
		 cpu_to_be32(IB_CLASS_PORT_INFO_RESP_TIME_MASK)) |
		cpu_to_be32(capmask2 <<
			    IB_CLASS_PORT_INFO_RESP_TIME_FIELD_SIZE);
}

struct ib_mad_notice_attr {
	u8 generic_type;
	u8 prod_type_msb;
	__be16 prod_type_lsb;
	__be16 trap_num;
	__be16 issuer_lid;
	__be16 toggle_count;

	union {
		struct {
			u8	details[54];
		} raw_data;

		struct {
			__be16	reserved;
			__be16	lid;		
			u8	port_num;	
		} __packed ntc_129_131;

		struct {
			__be16	reserved;
			__be16	lid;		
			u8	reserved2;
			u8	local_changes;	
			__be32	new_cap_mask;	
			u8	reserved3;
			u8	change_flags;	
		} __packed ntc_144;

		struct {
			__be16	reserved;
			__be16	lid;		
			__be16	reserved2;
			__be64	new_sys_guid;
		} __packed ntc_145;

		struct {
			__be16	reserved;
			__be16	lid;
			__be16	dr_slid;
			u8	method;
			u8	reserved2;
			__be16	attr_id;
			__be32	attr_mod;
			__be64	mkey;
			u8	reserved3;
			u8	dr_trunc_hop;
			u8	dr_rtn_path[30];
		} __packed ntc_256;

		struct {
			__be16		reserved;
			__be16		lid1;
			__be16		lid2;
			__be32		key;
			__be32		sl_qp1;	
			__be32		qp2;	
			union ib_gid	gid1;
			union ib_gid	gid2;
		} __packed ntc_257_258;

	} details;
};


struct ib_mad_send_buf {
	struct ib_mad_send_buf	*next;
	void			*mad;
	struct ib_mad_agent	*mad_agent;
	struct ib_ah		*ah;
	void			*context[2];
	int			hdr_len;
	int			data_len;
	int			seg_count;
	int			seg_size;
	int			seg_rmpp_size;
	int			timeout_ms;
	int			retries;
};


int ib_response_mad(const struct ib_mad_hdr *hdr);


static inline u8 ib_get_rmpp_resptime(struct ib_rmpp_hdr *rmpp_hdr)
{
	return rmpp_hdr->rmpp_rtime_flags >> 3;
}


static inline u8 ib_get_rmpp_flags(struct ib_rmpp_hdr *rmpp_hdr)
{
	return rmpp_hdr->rmpp_rtime_flags & 0x7;
}


static inline void ib_set_rmpp_resptime(struct ib_rmpp_hdr *rmpp_hdr, u8 rtime)
{
	rmpp_hdr->rmpp_rtime_flags = ib_get_rmpp_flags(rmpp_hdr) | (rtime << 3);
}


static inline void ib_set_rmpp_flags(struct ib_rmpp_hdr *rmpp_hdr, u8 flags)
{
	rmpp_hdr->rmpp_rtime_flags = (rmpp_hdr->rmpp_rtime_flags & 0xF8) |
				     (flags & 0x7);
}

struct ib_mad_agent;
struct ib_mad_send_wc;
struct ib_mad_recv_wc;


typedef void (*ib_mad_send_handler)(struct ib_mad_agent *mad_agent,
				    struct ib_mad_send_wc *mad_send_wc);


typedef void (*ib_mad_snoop_handler)(struct ib_mad_agent *mad_agent,
				     struct ib_mad_send_buf *send_buf,
				     struct ib_mad_send_wc *mad_send_wc);


typedef void (*ib_mad_recv_handler)(struct ib_mad_agent *mad_agent,
				    struct ib_mad_send_buf *send_buf,
				    struct ib_mad_recv_wc *mad_recv_wc);


enum {
	IB_MAD_USER_RMPP = IB_USER_MAD_USER_RMPP,
};
struct ib_mad_agent {
	struct ib_device	*device;
	struct ib_qp		*qp;
	ib_mad_recv_handler	recv_handler;
	ib_mad_send_handler	send_handler;
	ib_mad_snoop_handler	snoop_handler;
	void			*context;
	u32			hi_tid;
	u32			flags;
	u8			port_num;
	u8			rmpp_version;
};


struct ib_mad_send_wc {
	struct ib_mad_send_buf	*send_buf;
	enum ib_wc_status	status;
	u32			vendor_err;
};


struct ib_mad_recv_buf {
	struct list_head	list;
	struct ib_grh		*grh;
	union {
		struct ib_mad	*mad;
		struct opa_mad	*opa_mad;
	};
};


struct ib_mad_recv_wc {
	struct ib_wc		*wc;
	struct ib_mad_recv_buf	recv_buf;
	struct list_head	rmpp_list;
	int			mad_len;
	size_t			mad_seg_size;
};


struct ib_mad_reg_req {
	u8	mgmt_class;
	u8	mgmt_class_version;
	u8	oui[3];
	DECLARE_BITMAP(method_mask, IB_MGMT_MAX_METHODS);
};


struct ib_mad_agent *ib_register_mad_agent(struct ib_device *device,
					   u8 port_num,
					   enum ib_qp_type qp_type,
					   struct ib_mad_reg_req *mad_reg_req,
					   u8 rmpp_version,
					   ib_mad_send_handler send_handler,
					   ib_mad_recv_handler recv_handler,
					   void *context,
					   u32 registration_flags);

enum ib_mad_snoop_flags {
	
	
	IB_MAD_SNOOP_SEND_COMPLETIONS	   = (1<<2),
	
	IB_MAD_SNOOP_RECVS		   = (1<<4)
	
	
};


struct ib_mad_agent *ib_register_mad_snoop(struct ib_device *device,
					   u8 port_num,
					   enum ib_qp_type qp_type,
					   int mad_snoop_flags,
					   ib_mad_snoop_handler snoop_handler,
					   ib_mad_recv_handler recv_handler,
					   void *context);


int ib_unregister_mad_agent(struct ib_mad_agent *mad_agent);


int ib_post_send_mad(struct ib_mad_send_buf *send_buf,
		     struct ib_mad_send_buf **bad_send_buf);



void ib_free_recv_mad(struct ib_mad_recv_wc *mad_recv_wc);


void ib_cancel_mad(struct ib_mad_agent *mad_agent,
		   struct ib_mad_send_buf *send_buf);


int ib_modify_mad(struct ib_mad_agent *mad_agent,
		  struct ib_mad_send_buf *send_buf, u32 timeout_ms);


struct ib_mad_agent *ib_redirect_mad_qp(struct ib_qp *qp,
					u8 rmpp_version,
					ib_mad_send_handler send_handler,
					ib_mad_recv_handler recv_handler,
					void *context);


int ib_process_mad_wc(struct ib_mad_agent *mad_agent,
		      struct ib_wc *wc);


struct ib_mad_send_buf *ib_create_send_mad(struct ib_mad_agent *mad_agent,
					   u32 remote_qpn, u16 pkey_index,
					   int rmpp_active,
					   int hdr_len, int data_len,
					   gfp_t gfp_mask,
					   u8 base_version);


int ib_is_mad_class_rmpp(u8 mgmt_class);


int ib_get_mad_data_offset(u8 mgmt_class);


void *ib_get_rmpp_segment(struct ib_mad_send_buf *send_buf, int seg_num);


void ib_free_send_mad(struct ib_mad_send_buf *send_buf);


int ib_mad_kernel_rmpp_agent(const struct ib_mad_agent *agent);

#endif 
