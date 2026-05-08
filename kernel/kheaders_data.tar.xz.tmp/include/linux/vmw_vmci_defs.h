

#ifndef _VMW_VMCI_DEF_H_
#define _VMW_VMCI_DEF_H_

#include <linux/atomic.h>


#define VMCI_STATUS_ADDR      0x00
#define VMCI_CONTROL_ADDR     0x04
#define VMCI_ICR_ADDR	      0x08
#define VMCI_IMR_ADDR         0x0c
#define VMCI_DATA_OUT_ADDR    0x10
#define VMCI_DATA_IN_ADDR     0x14
#define VMCI_CAPS_ADDR        0x18
#define VMCI_RESULT_LOW_ADDR  0x1c
#define VMCI_RESULT_HIGH_ADDR 0x20


#define VMCI_MAX_DEVICES 1


#define VMCI_STATUS_INT_ON     0x1


#define VMCI_CONTROL_RESET        0x1
#define VMCI_CONTROL_INT_ENABLE   0x2
#define VMCI_CONTROL_INT_DISABLE  0x4


#define VMCI_CAPS_HYPERCALL     0x1
#define VMCI_CAPS_GUESTCALL     0x2
#define VMCI_CAPS_DATAGRAM      0x4
#define VMCI_CAPS_NOTIFICATIONS 0x8


#define VMCI_ICR_DATAGRAM      0x1
#define VMCI_ICR_NOTIFICATION  0x2


#define VMCI_IMR_DATAGRAM      0x1
#define VMCI_IMR_NOTIFICATION  0x2


enum {
	VMCI_INTR_TYPE_INTX = 0,
	VMCI_INTR_TYPE_MSI = 1,
	VMCI_INTR_TYPE_MSIX = 2,
};


#define VMCI_MAX_INTRS 2


enum {
	VMCI_INTR_DATAGRAM = 0,
	VMCI_INTR_NOTIFICATION = 1,
};


#define VMCI_MAX_GUEST_QP_MEMORY (128 * 1024 * 1024)
#define VMCI_MAX_GUEST_QP_COUNT  (VMCI_MAX_GUEST_QP_MEMORY / PAGE_SIZE / 2)


#define VMCI_MAX_GUEST_DOORBELL_COUNT PAGE_SIZE


#define VMCI_MAX_PINNED_QP_MEMORY (32 * 1024)


enum {
	VMCI_RESOURCES_QUERY = 0,
	VMCI_GET_CONTEXT_ID = 1,
	VMCI_SET_NOTIFY_BITMAP = 2,
	VMCI_DOORBELL_LINK = 3,
	VMCI_DOORBELL_UNLINK = 4,
	VMCI_DOORBELL_NOTIFY = 5,
	
	VMCI_DATAGRAM_REQUEST_MAP = 6,
	VMCI_DATAGRAM_REMOVE_MAP = 7,
	VMCI_EVENT_SUBSCRIBE = 8,
	VMCI_EVENT_UNSUBSCRIBE = 9,
	VMCI_QUEUEPAIR_ALLOC = 10,
	VMCI_QUEUEPAIR_DETACH = 11,

	
	VMCI_HGFS_TRANSPORT = 13,
	VMCI_UNITY_PBRPC_REGISTER = 14,
	VMCI_RPC_PRIVILEGED = 15,
	VMCI_RPC_UNPRIVILEGED = 16,
	VMCI_RESOURCE_MAX = 17,
};


struct vmci_handle {
	u32 context;
	u32 resource;
};

#define vmci_make_handle(_cid, _rid) \
	(struct vmci_handle){ .context = _cid, .resource = _rid }

static inline bool vmci_handle_is_equal(struct vmci_handle h1,
					struct vmci_handle h2)
{
	return h1.context == h2.context && h1.resource == h2.resource;
}

#define VMCI_INVALID_ID ~0
static const struct vmci_handle VMCI_INVALID_HANDLE = {
	.context = VMCI_INVALID_ID,
	.resource = VMCI_INVALID_ID
};

static inline bool vmci_handle_is_invalid(struct vmci_handle h)
{
	return vmci_handle_is_equal(h, VMCI_INVALID_HANDLE);
}


#define VMCI_ANON_SRC_CONTEXT_ID   VMCI_INVALID_ID
#define VMCI_ANON_SRC_RESOURCE_ID  VMCI_INVALID_ID
static const struct vmci_handle VMCI_ANON_SRC_HANDLE = {
	.context = VMCI_ANON_SRC_CONTEXT_ID,
	.resource = VMCI_ANON_SRC_RESOURCE_ID
};


#define VMCI_RESERVED_CID_LIMIT ((u32) 16)


#define VMCI_HYPERVISOR_CONTEXT_ID 0


#define VMCI_WELL_KNOWN_CONTEXT_ID 1


#define VMCI_HOST_CONTEXT_ID  2

#define VMCI_CONTEXT_IS_VM(_cid) (VMCI_INVALID_ID != (_cid) &&		\
				  (_cid) > VMCI_HOST_CONTEXT_ID)


#define VMCI_CONTEXT_RESOURCE_ID 0


enum {
	VMCI_SUCCESS_QUEUEPAIR_ATTACH	= 5,
	VMCI_SUCCESS_QUEUEPAIR_CREATE	= 4,
	VMCI_SUCCESS_LAST_DETACH	= 3,
	VMCI_SUCCESS_ACCESS_GRANTED	= 2,
	VMCI_SUCCESS_ENTRY_DEAD		= 1,
	VMCI_SUCCESS			 = 0,
	VMCI_ERROR_INVALID_RESOURCE	 = (-1),
	VMCI_ERROR_INVALID_ARGS		 = (-2),
	VMCI_ERROR_NO_MEM		 = (-3),
	VMCI_ERROR_DATAGRAM_FAILED	 = (-4),
	VMCI_ERROR_MORE_DATA		 = (-5),
	VMCI_ERROR_NO_MORE_DATAGRAMS	 = (-6),
	VMCI_ERROR_NO_ACCESS		 = (-7),
	VMCI_ERROR_NO_HANDLE		 = (-8),
	VMCI_ERROR_DUPLICATE_ENTRY	 = (-9),
	VMCI_ERROR_DST_UNREACHABLE	 = (-10),
	VMCI_ERROR_PAYLOAD_TOO_LARGE	 = (-11),
	VMCI_ERROR_INVALID_PRIV		 = (-12),
	VMCI_ERROR_GENERIC		 = (-13),
	VMCI_ERROR_PAGE_ALREADY_SHARED	 = (-14),
	VMCI_ERROR_CANNOT_SHARE_PAGE	 = (-15),
	VMCI_ERROR_CANNOT_UNSHARE_PAGE	 = (-16),
	VMCI_ERROR_NO_PROCESS		 = (-17),
	VMCI_ERROR_NO_DATAGRAM		 = (-18),
	VMCI_ERROR_NO_RESOURCES		 = (-19),
	VMCI_ERROR_UNAVAILABLE		 = (-20),
	VMCI_ERROR_NOT_FOUND		 = (-21),
	VMCI_ERROR_ALREADY_EXISTS	 = (-22),
	VMCI_ERROR_NOT_PAGE_ALIGNED	 = (-23),
	VMCI_ERROR_INVALID_SIZE		 = (-24),
	VMCI_ERROR_REGION_ALREADY_SHARED = (-25),
	VMCI_ERROR_TIMEOUT		 = (-26),
	VMCI_ERROR_DATAGRAM_INCOMPLETE	 = (-27),
	VMCI_ERROR_INCORRECT_IRQL	 = (-28),
	VMCI_ERROR_EVENT_UNKNOWN	 = (-29),
	VMCI_ERROR_OBSOLETE		 = (-30),
	VMCI_ERROR_QUEUEPAIR_MISMATCH	 = (-31),
	VMCI_ERROR_QUEUEPAIR_NOTSET	 = (-32),
	VMCI_ERROR_QUEUEPAIR_NOTOWNER	 = (-33),
	VMCI_ERROR_QUEUEPAIR_NOTATTACHED = (-34),
	VMCI_ERROR_QUEUEPAIR_NOSPACE	 = (-35),
	VMCI_ERROR_QUEUEPAIR_NODATA	 = (-36),
	VMCI_ERROR_BUSMEM_INVALIDATION	 = (-37),
	VMCI_ERROR_MODULE_NOT_LOADED	 = (-38),
	VMCI_ERROR_DEVICE_NOT_FOUND	 = (-39),
	VMCI_ERROR_QUEUEPAIR_NOT_READY	 = (-40),
	VMCI_ERROR_WOULD_BLOCK		 = (-41),

	
	VMCI_ERROR_CLIENT_MIN		 = (-500),
	VMCI_ERROR_CLIENT_MAX		 = (-550),

	
	VMCI_SHAREDMEM_ERROR_BAD_CONTEXT = (-1000),
};


enum {
	
	VMCI_EVENT_CTX_ID_UPDATE  = 0,

	
	VMCI_EVENT_CTX_REMOVED	  = 1,

	
	VMCI_EVENT_QP_RESUMED	  = 2,

	
	VMCI_EVENT_QP_PEER_ATTACH = 3,

	
	VMCI_EVENT_QP_PEER_DETACH = 4,

	
	VMCI_EVENT_MEM_ACCESS_ON  = 5,

	
	VMCI_EVENT_MEM_ACCESS_OFF = 6,
	VMCI_EVENT_MAX		  = 7,
};


#define VMCI_EVENT_VALID_VMX(_event) ((_event) == VMCI_EVENT_MEM_ACCESS_ON || \
				      (_event) == VMCI_EVENT_MEM_ACCESS_OFF)

#define VMCI_EVENT_VALID(_event) ((_event) < VMCI_EVENT_MAX &&		\
				  !VMCI_EVENT_VALID_VMX(_event))


#define VMCI_EVENT_HANDLER 0


enum {
	VMCI_NO_PRIVILEGE_FLAGS = 0,
	VMCI_PRIVILEGE_FLAG_RESTRICTED = 1,
	VMCI_PRIVILEGE_FLAG_TRUSTED = 2,
	VMCI_PRIVILEGE_ALL_FLAGS = (VMCI_PRIVILEGE_FLAG_RESTRICTED |
				    VMCI_PRIVILEGE_FLAG_TRUSTED),
	VMCI_DEFAULT_PROC_PRIVILEGE_FLAGS = VMCI_NO_PRIVILEGE_FLAGS,
	VMCI_LEAST_PRIVILEGE_FLAGS = VMCI_PRIVILEGE_FLAG_RESTRICTED,
	VMCI_MAX_PRIVILEGE_FLAGS = VMCI_PRIVILEGE_FLAG_TRUSTED,
};


#define VMCI_RESERVED_RESOURCE_ID_MAX 1023




#define VMCI_VERSION_SHIFT_WIDTH 16
#define VMCI_MAKE_VERSION(_major, _minor)			\
	((_major) << VMCI_VERSION_SHIFT_WIDTH | (u16) (_minor))

#define VMCI_VERSION_MAJOR(v)  ((u32) (v) >> VMCI_VERSION_SHIFT_WIDTH)
#define VMCI_VERSION_MINOR(v)  ((u16) (v))



#define VMCI_VERSION                VMCI_VERSION_NOVMVM
#define VMCI_VERSION_NOVMVM         VMCI_MAKE_VERSION(11, 0)
#define VMCI_VERSION_NOTIFY         VMCI_MAKE_VERSION(10, 0)
#define VMCI_VERSION_HOSTQP         VMCI_MAKE_VERSION(9, 0)
#define VMCI_VERSION_PREHOSTQP      VMCI_MAKE_VERSION(8, 0)
#define VMCI_VERSION_PREVERS2       VMCI_MAKE_VERSION(1, 0)

#define VMCI_SOCKETS_MAKE_VERSION(_p)					\
	((((_p)[0] & 0xFF) << 24) | (((_p)[1] & 0xFF) << 16) | ((_p)[2]))


#define IOCTL_VMCI_VERSION			_IO(7, 0x9f)	
#define IOCTL_VMCI_INIT_CONTEXT			_IO(7, 0xa0)
#define IOCTL_VMCI_QUEUEPAIR_SETVA		_IO(7, 0xa4)
#define IOCTL_VMCI_NOTIFY_RESOURCE		_IO(7, 0xa5)
#define IOCTL_VMCI_NOTIFICATIONS_RECEIVE	_IO(7, 0xa6)
#define IOCTL_VMCI_VERSION2			_IO(7, 0xa7)
#define IOCTL_VMCI_QUEUEPAIR_ALLOC		_IO(7, 0xa8)
#define IOCTL_VMCI_QUEUEPAIR_SETPAGEFILE	_IO(7, 0xa9)
#define IOCTL_VMCI_QUEUEPAIR_DETACH		_IO(7, 0xaa)
#define IOCTL_VMCI_DATAGRAM_SEND		_IO(7, 0xab)
#define IOCTL_VMCI_DATAGRAM_RECEIVE		_IO(7, 0xac)
#define IOCTL_VMCI_CTX_ADD_NOTIFICATION		_IO(7, 0xaf)
#define IOCTL_VMCI_CTX_REMOVE_NOTIFICATION	_IO(7, 0xb0)
#define IOCTL_VMCI_CTX_GET_CPT_STATE		_IO(7, 0xb1)
#define IOCTL_VMCI_CTX_SET_CPT_STATE		_IO(7, 0xb2)
#define IOCTL_VMCI_GET_CONTEXT_ID		_IO(7, 0xb3)
#define IOCTL_VMCI_SOCKETS_VERSION		_IO(7, 0xb4)
#define IOCTL_VMCI_SOCKETS_GET_AF_VALUE		_IO(7, 0xb8)
#define IOCTL_VMCI_SOCKETS_GET_LOCAL_CID	_IO(7, 0xb9)
#define IOCTL_VMCI_SET_NOTIFY			_IO(7, 0xcb)	
	


struct vmci_queue_header {
	
	struct vmci_handle handle;	
	atomic64_t producer_tail;	
	atomic64_t consumer_head;	
};


struct vmci_datagram {
	struct vmci_handle dst;
	struct vmci_handle src;
	u64 payload_size;
};


#define VMCI_FLAG_DG_NONE          0
#define VMCI_FLAG_WELLKNOWN_DG_HND 0x1
#define VMCI_FLAG_ANYCID_DG_HND    0x2
#define VMCI_FLAG_DG_DELAYED_CB    0x4


#define VMCI_MAX_DG_SIZE (17 * 4096)
#define VMCI_MAX_DG_PAYLOAD_SIZE (VMCI_MAX_DG_SIZE - \
				  sizeof(struct vmci_datagram))
#define VMCI_DG_PAYLOAD(_dg) (void *)((char *)(_dg) +			\
				      sizeof(struct vmci_datagram))
#define VMCI_DG_HEADERSIZE sizeof(struct vmci_datagram)
#define VMCI_DG_SIZE(_dg) (VMCI_DG_HEADERSIZE + (size_t)(_dg)->payload_size)
#define VMCI_DG_SIZE_ALIGNED(_dg) ((VMCI_DG_SIZE(_dg) + 7) & (~((size_t) 0x7)))
#define VMCI_MAX_DATAGRAM_QUEUE_SIZE (VMCI_MAX_DG_SIZE * 2)

struct vmci_event_payload_qp {
	struct vmci_handle handle;  
	u32 peer_id;		    
	u32 _pad;
};


enum {
	
	VMCI_QPFLAG_ATTACH_ONLY = 1 << 0,

	
	VMCI_QPFLAG_LOCAL = 1 << 1,

	
	VMCI_QPFLAG_NONBLOCK = 1 << 2,

	
	VMCI_QPFLAG_PINNED = 1 << 3,

	
	VMCI_QP_ALL_FLAGS = (VMCI_QPFLAG_ATTACH_ONLY | VMCI_QPFLAG_LOCAL |
			     VMCI_QPFLAG_NONBLOCK | VMCI_QPFLAG_PINNED),

	
	VMCI_QP_ASYMM = (VMCI_QPFLAG_NONBLOCK | VMCI_QPFLAG_PINNED),
	VMCI_QP_ASYMM_PEER = (VMCI_QPFLAG_ATTACH_ONLY | VMCI_QP_ASYMM),
};


#define VMCI_MAX_DATAGRAM_AND_EVENT_QUEUE_SIZE				\
	(VMCI_MAX_DATAGRAM_QUEUE_SIZE +					\
	 1024 * (sizeof(struct vmci_datagram) +				\
		 sizeof(struct vmci_event_data_max)))


struct vmci_resource_query_hdr {
	struct vmci_datagram hdr;
	u32 num_resources;
	u32 _padding;
};


struct vmci_resource_query_msg {
	u32 num_resources;
	u32 _padding;
	u32 resources[1];
};


#define VMCI_RESOURCE_QUERY_MAX_NUM 31


#define VMCI_RESOURCE_QUERY_MAX_SIZE				\
	(sizeof(struct vmci_resource_query_hdr) +		\
	 sizeof(u32) * VMCI_RESOURCE_QUERY_MAX_NUM)


struct vmci_notify_bm_set_msg {
	struct vmci_datagram hdr;
	u32 bitmap_ppn;
	u32 _pad;
};


struct vmci_doorbell_link_msg {
	struct vmci_datagram hdr;
	struct vmci_handle handle;
	u64 notify_idx;
};


struct vmci_doorbell_unlink_msg {
	struct vmci_datagram hdr;
	struct vmci_handle handle;
};


struct vmci_doorbell_notify_msg {
	struct vmci_datagram hdr;
	struct vmci_handle handle;
};


struct vmci_event_data {
	u32 event;		
	u32 _pad;
	
};


struct vmci_event_payld_ctx {
	u32 context_id;	
	u32 _pad;
};

struct vmci_event_payld_qp {
	struct vmci_handle handle;  
	u32 peer_id;	    
	u32 _pad;
};


struct vmci_event_data_max {
	struct vmci_event_data event_data;
	union {
		struct vmci_event_payld_ctx context_payload;
		struct vmci_event_payld_qp qp_payload;
	} ev_data_payload;
};


struct vmci_event_msg {
	struct vmci_datagram hdr;

	
	struct vmci_event_data event_data;

	
};


struct vmci_event_ctx {
	struct vmci_event_msg msg;
	struct vmci_event_payld_ctx payload;
};


struct vmci_event_qp {
	struct vmci_event_msg msg;
	struct vmci_event_payld_qp payload;
};


struct vmci_qp_alloc_msg {
	struct vmci_datagram hdr;
	struct vmci_handle handle;
	u32 peer;
	u32 flags;
	u64 produce_size;
	u64 consume_size;
	u64 num_ppns;

	
};

struct vmci_qp_detach_msg {
	struct vmci_datagram hdr;
	struct vmci_handle handle;
};


#define VMCI_FLAG_DELAYED_CB 0x01

typedef void (*vmci_callback) (void *client_data);


struct vmci_qp;


typedef int (*vmci_datagram_recv_cb) (void *client_data,
				      struct vmci_datagram *msg);


typedef void (*vmci_event_cb) (u32 sub_id, const struct vmci_event_data *ed,
			       void *client_data);


static inline const void *
vmci_event_data_const_payload(const struct vmci_event_data *ev_data)
{
	return (const char *)ev_data + sizeof(*ev_data);
}

static inline void *vmci_event_data_payload(struct vmci_event_data *ev_data)
{
	return (void *)vmci_event_data_const_payload(ev_data);
}


static inline u64 vmci_q_read_pointer(atomic64_t *var)
{
#if defined(CONFIG_X86_32)
	return atomic_read((atomic_t *)var);
#else
	return atomic64_read(var);
#endif
}


static inline void vmci_q_set_pointer(atomic64_t *var,
				      u64 new_val)
{
#if defined(CONFIG_X86_32)
	return atomic_set((atomic_t *)var, (u32)new_val);
#else
	return atomic64_set(var, new_val);
#endif
}


static inline void vmci_qp_add_pointer(atomic64_t *var,
				       size_t add,
				       u64 size)
{
	u64 new_val = vmci_q_read_pointer(var);

	if (new_val >= size - add)
		new_val -= size;

	new_val += add;

	vmci_q_set_pointer(var, new_val);
}


static inline u64
vmci_q_header_producer_tail(const struct vmci_queue_header *q_header)
{
	struct vmci_queue_header *qh = (struct vmci_queue_header *)q_header;
	return vmci_q_read_pointer(&qh->producer_tail);
}


static inline u64
vmci_q_header_consumer_head(const struct vmci_queue_header *q_header)
{
	struct vmci_queue_header *qh = (struct vmci_queue_header *)q_header;
	return vmci_q_read_pointer(&qh->consumer_head);
}


static inline void
vmci_q_header_add_producer_tail(struct vmci_queue_header *q_header,
				size_t add,
				u64 queue_size)
{
	vmci_qp_add_pointer(&q_header->producer_tail, add, queue_size);
}


static inline void
vmci_q_header_add_consumer_head(struct vmci_queue_header *q_header,
				size_t add,
				u64 queue_size)
{
	vmci_qp_add_pointer(&q_header->consumer_head, add, queue_size);
}


static inline void
vmci_q_header_get_pointers(const struct vmci_queue_header *produce_q_header,
			   const struct vmci_queue_header *consume_q_header,
			   u64 *producer_tail,
			   u64 *consumer_head)
{
	if (producer_tail)
		*producer_tail = vmci_q_header_producer_tail(produce_q_header);

	if (consumer_head)
		*consumer_head = vmci_q_header_consumer_head(consume_q_header);
}

static inline void vmci_q_header_init(struct vmci_queue_header *q_header,
				      const struct vmci_handle handle)
{
	q_header->handle = handle;
	atomic64_set(&q_header->producer_tail, 0);
	atomic64_set(&q_header->consumer_head, 0);
}


static s64
vmci_q_header_free_space(const struct vmci_queue_header *produce_q_header,
			 const struct vmci_queue_header *consume_q_header,
			 const u64 produce_q_size)
{
	u64 tail;
	u64 head;
	u64 free_space;

	tail = vmci_q_header_producer_tail(produce_q_header);
	head = vmci_q_header_consumer_head(consume_q_header);

	if (tail >= produce_q_size || head >= produce_q_size)
		return VMCI_ERROR_INVALID_SIZE;

	
	if (tail >= head)
		free_space = produce_q_size - (tail - head) - 1;
	else
		free_space = head - tail - 1;

	return free_space;
}


static inline s64
vmci_q_header_buf_ready(const struct vmci_queue_header *consume_q_header,
			const struct vmci_queue_header *produce_q_header,
			const u64 consume_q_size)
{
	s64 free_space;

	free_space = vmci_q_header_free_space(consume_q_header,
					      produce_q_header, consume_q_size);
	if (free_space < VMCI_SUCCESS)
		return free_space;

	return consume_q_size - free_space - 1;
}


#endif 
