

#ifndef _HYPERV_H
#define _HYPERV_H

#include <uapi/linux/hyperv.h>
#include <uapi/asm/hyperv.h>

#include <linux/types.h>
#include <linux/scatterlist.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/completion.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>


#define MAX_PAGE_BUFFER_COUNT				32
#define MAX_MULTIPAGE_BUFFER_COUNT			32 

#pragma pack(push, 1)


struct hv_page_buffer {
	u32 len;
	u32 offset;
	u64 pfn;
};


struct hv_multipage_buffer {
	
	u32 len;
	u32 offset;
	u64 pfn_array[MAX_MULTIPAGE_BUFFER_COUNT];
};


struct hv_mpb_array {
	
	u32 len;
	u32 offset;
	u64 pfn_array[];
};


#define MAX_PAGE_BUFFER_PACKET		(0x18 +			\
					(sizeof(struct hv_page_buffer) * \
					 MAX_PAGE_BUFFER_COUNT))
#define MAX_MULTIPAGE_BUFFER_PACKET	(0x18 +			\
					 sizeof(struct hv_multipage_buffer))


#pragma pack(pop)

struct hv_ring_buffer {
	
	u32 write_index;

	
	u32 read_index;

	u32 interrupt_mask;

	
	u32 pending_send_sz;

	u32 reserved1[12];

	union {
		struct {
			u32 feat_pending_send_sz:1;
		};
		u32 value;
	} feature_bits;

	
	u8	reserved2[4028];

	
	u8 buffer[0];
} __packed;

struct hv_ring_buffer_info {
	struct hv_ring_buffer *ring_buffer;
	u32 ring_size;			
	spinlock_t ring_lock;

	u32 ring_datasize;		
	u32 ring_data_startoffset;
	u32 priv_write_index;
	u32 priv_read_index;
	u32 cached_read_index;
};


static inline void
hv_get_ringbuffer_availbytes(struct hv_ring_buffer_info *rbi,
			  u32 *read, u32 *write)
{
	u32 read_loc, write_loc, dsize;

	
	read_loc = rbi->ring_buffer->read_index;
	write_loc = rbi->ring_buffer->write_index;
	dsize = rbi->ring_datasize;

	*write = write_loc >= read_loc ? dsize - (write_loc - read_loc) :
		read_loc - write_loc;
	*read = dsize - *write;
}

static inline u32 hv_get_bytes_to_read(struct hv_ring_buffer_info *rbi)
{
	u32 read_loc, write_loc, dsize, read;

	dsize = rbi->ring_datasize;
	read_loc = rbi->ring_buffer->read_index;
	write_loc = READ_ONCE(rbi->ring_buffer->write_index);

	read = write_loc >= read_loc ? (write_loc - read_loc) :
		(dsize - read_loc) + write_loc;

	return read;
}

static inline u32 hv_get_bytes_to_write(struct hv_ring_buffer_info *rbi)
{
	u32 read_loc, write_loc, dsize, write;

	dsize = rbi->ring_datasize;
	read_loc = READ_ONCE(rbi->ring_buffer->read_index);
	write_loc = rbi->ring_buffer->write_index;

	write = write_loc >= read_loc ? dsize - (write_loc - read_loc) :
		read_loc - write_loc;
	return write;
}

static inline u32 hv_get_cached_bytes_to_write(
	const struct hv_ring_buffer_info *rbi)
{
	u32 read_loc, write_loc, dsize, write;

	dsize = rbi->ring_datasize;
	read_loc = rbi->cached_read_index;
	write_loc = rbi->ring_buffer->write_index;

	write = write_loc >= read_loc ? dsize - (write_loc - read_loc) :
		read_loc - write_loc;
	return write;
}


#define VERSION_WS2008  ((0 << 16) | (13))
#define VERSION_WIN7    ((1 << 16) | (1))
#define VERSION_WIN8    ((2 << 16) | (4))
#define VERSION_WIN8_1    ((3 << 16) | (0))
#define VERSION_WIN10	((4 << 16) | (0))

#define VERSION_INVAL -1

#define VERSION_CURRENT VERSION_WIN10


#define MAX_PIPE_DATA_PAYLOAD		(sizeof(u8) * 16384)


#define VMBUS_PIPE_TYPE_BYTE		0x00000000
#define VMBUS_PIPE_TYPE_MESSAGE		0x00000004


#define MAX_USER_DEFINED_BYTES		120


#define MAX_PIPE_USER_DEFINED_BYTES	116


struct vmbus_channel_offer {
	uuid_le if_type;
	uuid_le if_instance;

	
	u64 reserved1;
	u64 reserved2;

	u16 chn_flags;
	u16 mmio_megabytes;		

	union {
		
		struct {
			unsigned char user_def[MAX_USER_DEFINED_BYTES];
		} std;

		
		struct {
			u32  pipe_mode;
			unsigned char user_def[MAX_PIPE_USER_DEFINED_BYTES];
		} pipe;
	} u;
	
	u16 sub_channel_index;
	u16 reserved3;
} __packed;


#define VMBUS_CHANNEL_ENUMERATE_DEVICE_INTERFACE	1
#define VMBUS_CHANNEL_SERVER_SUPPORTS_TRANSFER_PAGES	2
#define VMBUS_CHANNEL_SERVER_SUPPORTS_GPADLS		4
#define VMBUS_CHANNEL_NAMED_PIPE_MODE			0x10
#define VMBUS_CHANNEL_LOOPBACK_OFFER			0x100
#define VMBUS_CHANNEL_PARENT_OFFER			0x200
#define VMBUS_CHANNEL_REQUEST_MONITORED_NOTIFICATION	0x400
#define VMBUS_CHANNEL_TLNPI_PROVIDER_OFFER		0x2000

struct vmpacket_descriptor {
	u16 type;
	u16 offset8;
	u16 len8;
	u16 flags;
	u64 trans_id;
} __packed;

struct vmpacket_header {
	u32 prev_pkt_start_offset;
	struct vmpacket_descriptor descriptor;
} __packed;

struct vmtransfer_page_range {
	u32 byte_count;
	u32 byte_offset;
} __packed;

struct vmtransfer_page_packet_header {
	struct vmpacket_descriptor d;
	u16 xfer_pageset_id;
	u8  sender_owns_set;
	u8 reserved;
	u32 range_cnt;
	struct vmtransfer_page_range ranges[1];
} __packed;

struct vmgpadl_packet_header {
	struct vmpacket_descriptor d;
	u32 gpadl;
	u32 reserved;
} __packed;

struct vmadd_remove_transfer_page_set {
	struct vmpacket_descriptor d;
	u32 gpadl;
	u16 xfer_pageset_id;
	u16 reserved;
} __packed;


struct gpa_range {
	u32 byte_count;
	u32 byte_offset;
	u64 pfn_array[0];
};


struct vmestablish_gpadl {
	struct vmpacket_descriptor d;
	u32 gpadl;
	u32 range_cnt;
	struct gpa_range range[1];
} __packed;


struct vmteardown_gpadl {
	struct vmpacket_descriptor d;
	u32 gpadl;
	u32 reserved;	
} __packed;


struct vmdata_gpa_direct {
	struct vmpacket_descriptor d;
	u32 reserved;
	u32 range_cnt;
	struct gpa_range range[1];
} __packed;


struct vmadditional_data {
	struct vmpacket_descriptor d;
	u64 total_bytes;
	u32 offset;
	u32 byte_cnt;
	unsigned char data[1];
} __packed;

union vmpacket_largest_possible_header {
	struct vmpacket_descriptor simple_hdr;
	struct vmtransfer_page_packet_header xfer_page_hdr;
	struct vmgpadl_packet_header gpadl_hdr;
	struct vmadd_remove_transfer_page_set add_rm_xfer_page_hdr;
	struct vmestablish_gpadl establish_gpadl_hdr;
	struct vmteardown_gpadl teardown_gpadl_hdr;
	struct vmdata_gpa_direct data_gpa_direct_hdr;
};

#define VMPACKET_DATA_START_ADDRESS(__packet)	\
	(void *)(((unsigned char *)__packet) +	\
	 ((struct vmpacket_descriptor)__packet)->offset8 * 8)

#define VMPACKET_DATA_LENGTH(__packet)		\
	((((struct vmpacket_descriptor)__packet)->len8 -	\
	  ((struct vmpacket_descriptor)__packet)->offset8) * 8)

#define VMPACKET_TRANSFER_MODE(__packet)	\
	(((struct IMPACT)__packet)->type)

enum vmbus_packet_type {
	VM_PKT_INVALID				= 0x0,
	VM_PKT_SYNCH				= 0x1,
	VM_PKT_ADD_XFER_PAGESET			= 0x2,
	VM_PKT_RM_XFER_PAGESET			= 0x3,
	VM_PKT_ESTABLISH_GPADL			= 0x4,
	VM_PKT_TEARDOWN_GPADL			= 0x5,
	VM_PKT_DATA_INBAND			= 0x6,
	VM_PKT_DATA_USING_XFER_PAGES		= 0x7,
	VM_PKT_DATA_USING_GPADL			= 0x8,
	VM_PKT_DATA_USING_GPA_DIRECT		= 0x9,
	VM_PKT_CANCEL_REQUEST			= 0xa,
	VM_PKT_COMP				= 0xb,
	VM_PKT_DATA_USING_ADDITIONAL_PKT	= 0xc,
	VM_PKT_ADDITIONAL_DATA			= 0xd
};

#define VMBUS_DATA_PACKET_FLAG_COMPLETION_REQUESTED	1



enum vmbus_channel_message_type {
	CHANNELMSG_INVALID			=  0,
	CHANNELMSG_OFFERCHANNEL		=  1,
	CHANNELMSG_RESCIND_CHANNELOFFER	=  2,
	CHANNELMSG_REQUESTOFFERS		=  3,
	CHANNELMSG_ALLOFFERS_DELIVERED	=  4,
	CHANNELMSG_OPENCHANNEL		=  5,
	CHANNELMSG_OPENCHANNEL_RESULT		=  6,
	CHANNELMSG_CLOSECHANNEL		=  7,
	CHANNELMSG_GPADL_HEADER		=  8,
	CHANNELMSG_GPADL_BODY			=  9,
	CHANNELMSG_GPADL_CREATED		= 10,
	CHANNELMSG_GPADL_TEARDOWN		= 11,
	CHANNELMSG_GPADL_TORNDOWN		= 12,
	CHANNELMSG_RELID_RELEASED		= 13,
	CHANNELMSG_INITIATE_CONTACT		= 14,
	CHANNELMSG_VERSION_RESPONSE		= 15,
	CHANNELMSG_UNLOAD			= 16,
	CHANNELMSG_UNLOAD_RESPONSE		= 17,
	CHANNELMSG_18				= 18,
	CHANNELMSG_19				= 19,
	CHANNELMSG_20				= 20,
	CHANNELMSG_TL_CONNECT_REQUEST		= 21,
	CHANNELMSG_COUNT
};

struct vmbus_channel_message_header {
	enum vmbus_channel_message_type msgtype;
	u32 padding;
} __packed;


struct vmbus_channel_query_vmbus_version {
	struct vmbus_channel_message_header header;
	u32 version;
} __packed;


struct vmbus_channel_version_supported {
	struct vmbus_channel_message_header header;
	u8 version_supported;
} __packed;


struct vmbus_channel_offer_channel {
	struct vmbus_channel_message_header header;
	struct vmbus_channel_offer offer;
	u32 child_relid;
	u8 monitorid;
	
	u8 monitor_allocated:1;
	u8 reserved:7;
	
	u16 is_dedicated_interrupt:1;
	u16 reserved1:15;
	u32 connection_id;
} __packed;


struct vmbus_channel_rescind_offer {
	struct vmbus_channel_message_header header;
	u32 child_relid;
} __packed;




struct vmbus_channel_open_channel {
	struct vmbus_channel_message_header header;

	
	u32 child_relid;

	
	u32 openid;

	
	u32 ringbuffer_gpadlhandle;

	
	u32 target_vp;

	
	u32 downstream_ringbuffer_pageoffset;

	
	unsigned char userdata[MAX_USER_DEFINED_BYTES];
} __packed;


struct vmbus_channel_open_result {
	struct vmbus_channel_message_header header;
	u32 child_relid;
	u32 openid;
	u32 status;
} __packed;


struct vmbus_channel_close_channel {
	struct vmbus_channel_message_header header;
	u32 child_relid;
} __packed;


#define GPADL_TYPE_RING_BUFFER		1
#define GPADL_TYPE_SERVER_SAVE_AREA	2
#define GPADL_TYPE_TRANSACTION		8


struct vmbus_channel_gpadl_header {
	struct vmbus_channel_message_header header;
	u32 child_relid;
	u32 gpadl;
	u16 range_buflen;
	u16 rangecount;
	struct gpa_range range[0];
} __packed;


struct vmbus_channel_gpadl_body {
	struct vmbus_channel_message_header header;
	u32 msgnumber;
	u32 gpadl;
	u64 pfn[0];
} __packed;

struct vmbus_channel_gpadl_created {
	struct vmbus_channel_message_header header;
	u32 child_relid;
	u32 gpadl;
	u32 creation_status;
} __packed;

struct vmbus_channel_gpadl_teardown {
	struct vmbus_channel_message_header header;
	u32 child_relid;
	u32 gpadl;
} __packed;

struct vmbus_channel_gpadl_torndown {
	struct vmbus_channel_message_header header;
	u32 gpadl;
} __packed;

struct vmbus_channel_relid_released {
	struct vmbus_channel_message_header header;
	u32 child_relid;
} __packed;

struct vmbus_channel_initiate_contact {
	struct vmbus_channel_message_header header;
	u32 vmbus_version_requested;
	u32 target_vcpu; 
	u64 interrupt_page;
	u64 monitor_page1;
	u64 monitor_page2;
} __packed;


struct vmbus_channel_tl_connect_request {
	struct vmbus_channel_message_header header;
	uuid_le guest_endpoint_id;
	uuid_le host_service_id;
} __packed;

struct vmbus_channel_version_response {
	struct vmbus_channel_message_header header;
	u8 version_supported;
} __packed;

enum vmbus_channel_state {
	CHANNEL_OFFER_STATE,
	CHANNEL_OPENING_STATE,
	CHANNEL_OPEN_STATE,
	CHANNEL_OPENED_STATE,
};


struct vmbus_channel_msginfo {
	
	struct list_head msglistentry;

	
	struct list_head submsglist;

	
	struct completion  waitevent;
	struct vmbus_channel *waiting_channel;
	union {
		struct vmbus_channel_version_supported version_supported;
		struct vmbus_channel_open_result open_result;
		struct vmbus_channel_gpadl_torndown gpadl_torndown;
		struct vmbus_channel_gpadl_created gpadl_created;
		struct vmbus_channel_version_response version_response;
	} response;

	u32 msgsize;
	
	unsigned char msg[0];
};

struct vmbus_close_msg {
	struct vmbus_channel_msginfo info;
	struct vmbus_channel_close_channel msg;
};


union hv_connection_id {
	u32 asu32;
	struct {
		u32 id:24;
		u32 reserved:8;
	} u;
};


struct hv_input_signal_event {
	union hv_connection_id connectionid;
	u16 flag_number;
	u16 rsvdz;
};

struct hv_input_signal_event_buffer {
	u64 align8;
	struct hv_input_signal_event event;
};

enum hv_signal_policy {
	HV_SIGNAL_POLICY_DEFAULT = 0,
	HV_SIGNAL_POLICY_EXPLICIT,
};

enum hv_numa_policy {
	HV_BALANCED = 0,
	HV_LOCALIZED,
};

enum vmbus_device_type {
	HV_IDE = 0,
	HV_SCSI,
	HV_FC,
	HV_NIC,
	HV_ND,
	HV_PCIE,
	HV_FB,
	HV_KBD,
	HV_MOUSE,
	HV_KVP,
	HV_TS,
	HV_HB,
	HV_SHUTDOWN,
	HV_FCOPY,
	HV_BACKUP,
	HV_DM,
	HV_UNKOWN,
};

struct vmbus_device {
	u16  dev_type;
	uuid_le guid;
	bool perf_device;
};

struct vmbus_channel {
	struct list_head listentry;

	struct hv_device *device_obj;

	enum vmbus_channel_state state;

	struct vmbus_channel_offer_channel offermsg;
	
	u8 monitor_grp;
	u8 monitor_bit;

	bool rescind; 

	u32 ringbuffer_gpadlhandle;

	
	void *ringbuffer_pages;
	u32 ringbuffer_pagecount;
	struct hv_ring_buffer_info outbound;	
	struct hv_ring_buffer_info inbound;	
	spinlock_t inbound_lock;

	struct vmbus_close_msg close_msg;

	
	

	void (*onchannel_callback)(void *context);
	void *channel_callback_context;

	

	bool batched_reading;

	bool is_dedicated_interrupt;
	struct hv_input_signal_event_buffer sig_buf;
	struct hv_input_signal_event *sig_event;

	
	u32 target_vp;
	
	u32 target_cpu;
	
	struct cpumask alloced_cpus_in_node;
	int numa_node;
	

	
	void (*sc_creation_callback)(struct vmbus_channel *new_sc);

	
	void (*chn_rescind_callback)(struct vmbus_channel *channel);

	
	spinlock_t lock;
	
	struct list_head sc_list;
	
	int num_sc;
	
	int next_oc;
	
	struct vmbus_channel *primary_channel;
	
	void *per_channel_state;
	
	struct list_head percpu_list;
	
	enum hv_signal_policy  signal_policy;
	
	bool acquire_ring_lock;
	
	bool low_latency;

	
	enum hv_numa_policy affinity_policy;

};

static inline void set_channel_lock_state(struct vmbus_channel *c, bool state)
{
	c->acquire_ring_lock = state;
}

static inline bool is_hvsock_channel(const struct vmbus_channel *c)
{
	return !!(c->offermsg.offer.chn_flags &
		  VMBUS_CHANNEL_TLNPI_PROVIDER_OFFER);
}

static inline void set_channel_signal_state(struct vmbus_channel *c,
					    enum hv_signal_policy policy)
{
	c->signal_policy = policy;
}

static inline void set_channel_affinity_state(struct vmbus_channel *c,
					      enum hv_numa_policy policy)
{
	c->affinity_policy = policy;
}

static inline void set_channel_read_state(struct vmbus_channel *c, bool state)
{
	c->batched_reading = state;
}

static inline void set_per_channel_state(struct vmbus_channel *c, void *s)
{
	c->per_channel_state = s;
}

static inline void *get_per_channel_state(struct vmbus_channel *c)
{
	return c->per_channel_state;
}

static inline void set_channel_pending_send_size(struct vmbus_channel *c,
						 u32 size)
{
	c->outbound.ring_buffer->pending_send_sz = size;
}

static inline void set_low_latency_mode(struct vmbus_channel *c)
{
	c->low_latency = true;
}

static inline void clear_low_latency_mode(struct vmbus_channel *c)
{
	c->low_latency = false;
}

void vmbus_onmessage(void *context);

int vmbus_request_offers(void);



void vmbus_set_sc_create_callback(struct vmbus_channel *primary_channel,
			void (*sc_cr_cb)(struct vmbus_channel *new_sc));

void vmbus_set_chn_rescind_callback(struct vmbus_channel *channel,
		void (*chn_rescind_cb)(struct vmbus_channel *));


struct vmbus_channel *vmbus_get_outgoing_channel(struct vmbus_channel *primary);


bool vmbus_are_subchannels_present(struct vmbus_channel *primary);


struct vmbus_channel_packet_page_buffer {
	u16 type;
	u16 dataoffset8;
	u16 length8;
	u16 flags;
	u64 transactionid;
	u32 reserved;
	u32 rangecount;
	struct hv_page_buffer range[MAX_PAGE_BUFFER_COUNT];
} __packed;


struct vmbus_channel_packet_multipage_buffer {
	u16 type;
	u16 dataoffset8;
	u16 length8;
	u16 flags;
	u64 transactionid;
	u32 reserved;
	u32 rangecount;		
	struct hv_multipage_buffer range;
} __packed;


struct vmbus_packet_mpb_array {
	u16 type;
	u16 dataoffset8;
	u16 length8;
	u16 flags;
	u64 transactionid;
	u32 reserved;
	u32 rangecount;         
	struct hv_mpb_array range;
} __packed;


extern int vmbus_open(struct vmbus_channel *channel,
			    u32 send_ringbuffersize,
			    u32 recv_ringbuffersize,
			    void *userdata,
			    u32 userdatalen,
			    void(*onchannel_callback)(void *context),
			    void *context);

extern void vmbus_close(struct vmbus_channel *channel);

extern int vmbus_sendpacket(struct vmbus_channel *channel,
				  void *buffer,
				  u32 bufferLen,
				  u64 requestid,
				  enum vmbus_packet_type type,
				  u32 flags);

extern int vmbus_sendpacket_ctl(struct vmbus_channel *channel,
				  void *buffer,
				  u32 bufferLen,
				  u64 requestid,
				  enum vmbus_packet_type type,
				  u32 flags,
				  bool kick_q);

extern int vmbus_sendpacket_pagebuffer(struct vmbus_channel *channel,
					    struct hv_page_buffer pagebuffers[],
					    u32 pagecount,
					    void *buffer,
					    u32 bufferlen,
					    u64 requestid);

extern int vmbus_sendpacket_pagebuffer_ctl(struct vmbus_channel *channel,
					   struct hv_page_buffer pagebuffers[],
					   u32 pagecount,
					   void *buffer,
					   u32 bufferlen,
					   u64 requestid,
					   u32 flags,
					   bool kick_q);

extern int vmbus_sendpacket_multipagebuffer(struct vmbus_channel *channel,
					struct hv_multipage_buffer *mpb,
					void *buffer,
					u32 bufferlen,
					u64 requestid);

extern int vmbus_sendpacket_mpb_desc(struct vmbus_channel *channel,
				     struct vmbus_packet_mpb_array *mpb,
				     u32 desc_size,
				     void *buffer,
				     u32 bufferlen,
				     u64 requestid);

extern int vmbus_establish_gpadl(struct vmbus_channel *channel,
				      void *kbuffer,
				      u32 size,
				      u32 *gpadl_handle);

extern int vmbus_teardown_gpadl(struct vmbus_channel *channel,
				     u32 gpadl_handle);

extern int vmbus_recvpacket(struct vmbus_channel *channel,
				  void *buffer,
				  u32 bufferlen,
				  u32 *buffer_actual_len,
				  u64 *requestid);

extern int vmbus_recvpacket_raw(struct vmbus_channel *channel,
				     void *buffer,
				     u32 bufferlen,
				     u32 *buffer_actual_len,
				     u64 *requestid);


extern void vmbus_ontimer(unsigned long data);


struct hv_driver {
	const char *name;

	
	bool hvsock;

	
	uuid_le dev_type;
	const struct hv_vmbus_device_id *id_table;

	struct device_driver driver;

	int (*probe)(struct hv_device *, const struct hv_vmbus_device_id *);
	int (*remove)(struct hv_device *);
	void (*shutdown)(struct hv_device *);

};


struct hv_device {
	
	uuid_le dev_type;

	
	uuid_le dev_instance;
	u16 vendor_id;
	u16 device_id;

	struct device device;

	struct vmbus_channel *channel;
};


static inline struct hv_device *device_to_hv_device(struct device *d)
{
	return container_of(d, struct hv_device, device);
}

static inline struct hv_driver *drv_to_hv_drv(struct device_driver *d)
{
	return container_of(d, struct hv_driver, driver);
}

static inline void hv_set_drvdata(struct hv_device *dev, void *data)
{
	dev_set_drvdata(&dev->device, data);
}

static inline void *hv_get_drvdata(struct hv_device *dev)
{
	return dev_get_drvdata(&dev->device);
}


#define vmbus_driver_register(driver)	\
	__vmbus_driver_register(driver, THIS_MODULE, KBUILD_MODNAME)
int __must_check __vmbus_driver_register(struct hv_driver *hv_driver,
					 struct module *owner,
					 const char *mod_name);
void vmbus_driver_unregister(struct hv_driver *hv_driver);

void vmbus_hvsock_device_unregister(struct vmbus_channel *channel);

int vmbus_allocate_mmio(struct resource **new, struct hv_device *device_obj,
			resource_size_t min, resource_size_t max,
			resource_size_t size, resource_size_t align,
			bool fb_overlap_ok);
void vmbus_free_mmio(resource_size_t start, resource_size_t size);
int vmbus_cpu_number_to_vp_number(int cpu_number);
u64 hv_do_hypercall(u64 control, void *input, void *output);




#define HV_NIC_GUID \
	.guid = UUID_LE(0xf8615163, 0xdf3e, 0x46c5, 0x91, 0x3f, \
			0xf2, 0xd2, 0xf9, 0x65, 0xed, 0x0e)


#define HV_IDE_GUID \
	.guid = UUID_LE(0x32412632, 0x86cb, 0x44a2, 0x9b, 0x5c, \
			0x50, 0xd1, 0x41, 0x73, 0x54, 0xf5)


#define HV_SCSI_GUID \
	.guid = UUID_LE(0xba6163d9, 0x04a1, 0x4d29, 0xb6, 0x05, \
			0x72, 0xe2, 0xff, 0xb1, 0xdc, 0x7f)


#define HV_SHUTDOWN_GUID \
	.guid = UUID_LE(0x0e0b6031, 0x5213, 0x4934, 0x81, 0x8b, \
			0x38, 0xd9, 0x0c, 0xed, 0x39, 0xdb)


#define HV_TS_GUID \
	.guid = UUID_LE(0x9527e630, 0xd0ae, 0x497b, 0xad, 0xce, \
			0xe8, 0x0a, 0xb0, 0x17, 0x5c, 0xaf)


#define HV_HEART_BEAT_GUID \
	.guid = UUID_LE(0x57164f39, 0x9115, 0x4e78, 0xab, 0x55, \
			0x38, 0x2f, 0x3b, 0xd5, 0x42, 0x2d)


#define HV_KVP_GUID \
	.guid = UUID_LE(0xa9a0f4e7, 0x5a45, 0x4d96, 0xb8, 0x27, \
			0x8a, 0x84, 0x1e, 0x8c, 0x03, 0xe6)


#define HV_DM_GUID \
	.guid = UUID_LE(0x525074dc, 0x8985, 0x46e2, 0x80, 0x57, \
			0xa3, 0x07, 0xdc, 0x18, 0xa5, 0x02)


#define HV_MOUSE_GUID \
	.guid = UUID_LE(0xcfa8b69e, 0x5b4a, 0x4cc0, 0xb9, 0x8b, \
			0x8b, 0xa1, 0xa1, 0xf3, 0xf9, 0x5a)


#define HV_KBD_GUID \
	.guid = UUID_LE(0xf912ad6d, 0x2b17, 0x48ea, 0xbd, 0x65, \
			0xf9, 0x27, 0xa6, 0x1c, 0x76, 0x84)


#define HV_VSS_GUID \
	.guid = UUID_LE(0x35fa2e29, 0xea23, 0x4236, 0x96, 0xae, \
			0x3a, 0x6e, 0xba, 0xcb, 0xa4, 0x40)

#define HV_SYNTHVID_GUID \
	.guid = UUID_LE(0xda0a7802, 0xe377, 0x4aac, 0x8e, 0x77, \
			0x05, 0x58, 0xeb, 0x10, 0x73, 0xf8)


#define HV_SYNTHFC_GUID \
	.guid = UUID_LE(0x2f9bcc4a, 0x0069, 0x4af3, 0xb7, 0x6b, \
			0x6f, 0xd0, 0xbe, 0x52, 0x8c, 0xda)



#define HV_FCOPY_GUID \
	.guid = UUID_LE(0x34d14be3, 0xdee4, 0x41c8, 0x9a, 0xe7, \
			0x6b, 0x17, 0x49, 0x77, 0xc1, 0x92)


#define HV_ND_GUID \
	.guid = UUID_LE(0x8c2eaf3d, 0x32a7, 0x4b09, 0xab, 0x99, \
			0xbd, 0x1f, 0x1c, 0x86, 0xb5, 0x01)



#define HV_PCIE_GUID \
	.guid = UUID_LE(0x44c4f61d, 0x4444, 0x4400, 0x9d, 0x52, \
			0x80, 0x2e, 0x27, 0xed, 0xe1, 0x9f)



#define HV_AVMA1_GUID \
	.guid = UUID_LE(0xf8e65716, 0x3cb3, 0x4a06, 0x9a, 0x60, \
			0x18, 0x89, 0xc5, 0xcc, 0xca, 0xb5)

#define HV_AVMA2_GUID \
	.guid = UUID_LE(0x3375baf4, 0x9e15, 0x4b30, 0xb7, 0x65, \
			0x67, 0xac, 0xb1, 0x0d, 0x60, 0x7b)

#define HV_RDV_GUID \
	.guid = UUID_LE(0x276aacf4, 0xac15, 0x426c, 0x98, 0xdd, \
			0x75, 0x21, 0xad, 0x3f, 0x01, 0xfe)



#define ICMSGTYPE_NEGOTIATE		0
#define ICMSGTYPE_HEARTBEAT		1
#define ICMSGTYPE_KVPEXCHANGE		2
#define ICMSGTYPE_SHUTDOWN		3
#define ICMSGTYPE_TIMESYNC		4
#define ICMSGTYPE_VSS			5

#define ICMSGHDRFLAG_TRANSACTION	1
#define ICMSGHDRFLAG_REQUEST		2
#define ICMSGHDRFLAG_RESPONSE		4




struct hv_util_service {
	u8 *recv_buffer;
	void *channel;
	void (*util_cb)(void *);
	int (*util_init)(struct hv_util_service *);
	void (*util_deinit)(void);
};

struct vmbuspipe_hdr {
	u32 flags;
	u32 msgsize;
} __packed;

struct ic_version {
	u16 major;
	u16 minor;
} __packed;

struct icmsg_hdr {
	struct ic_version icverframe;
	u16 icmsgtype;
	struct ic_version icvermsg;
	u16 icmsgsize;
	u32 status;
	u8 ictransaction_id;
	u8 icflags;
	u8 reserved[2];
} __packed;

struct icmsg_negotiate {
	u16 icframe_vercnt;
	u16 icmsg_vercnt;
	u32 reserved;
	struct ic_version icversion_data[1]; 
} __packed;

struct shutdown_msg_data {
	u32 reason_code;
	u32 timeout_seconds;
	u32 flags;
	u8  display_message[2048];
} __packed;

struct heartbeat_msg_data {
	u64 seq_num;
	u32 reserved[8];
} __packed;


#define ICTIMESYNCFLAG_PROBE	0
#define ICTIMESYNCFLAG_SYNC	1
#define ICTIMESYNCFLAG_SAMPLE	2

#ifdef __x86_64__
#define WLTIMEDELTA	116444736000000000L	
#else
#define WLTIMEDELTA	116444736000000000LL
#endif

struct ictimesync_data {
	u64 parenttime;
	u64 childtime;
	u64 roundtriptime;
	u8 flags;
} __packed;

struct ictimesync_ref_data {
	u64 parenttime;
	u64 vmreferencetime;
	u8 flags;
	char leapflags;
	char stratum;
	u8 reserved[3];
} __packed;

struct hyperv_service_callback {
	u8 msg_type;
	char *log_msg;
	uuid_le data;
	struct vmbus_channel *channel;
	void (*callback) (void *context);
};

#define MAX_SRV_VER	0x7ffffff
extern bool vmbus_prep_negotiate_resp(struct icmsg_hdr *,
					struct icmsg_negotiate *, u8 *, int,
					int);

void hv_event_tasklet_disable(struct vmbus_channel *channel);
void hv_event_tasklet_enable(struct vmbus_channel *channel);

void hv_process_channel_removal(struct vmbus_channel *channel, u32 relid);

void vmbus_setevent(struct vmbus_channel *channel);


extern __u32 vmbus_proto_version;

int vmbus_send_tl_connect_request(const uuid_le *shv_guest_servie_id,
				  const uuid_le *shv_host_servie_id);
void vmbus_set_event(struct vmbus_channel *channel);


static inline void *
hv_get_ring_buffer(struct hv_ring_buffer_info *ring_info)
{
	return (void *)ring_info->ring_buffer->buffer;
}



static inline  void hv_signal_on_read(struct vmbus_channel *channel)
{
	u32 cur_write_sz, cached_write_sz;
	u32 pending_sz;
	struct hv_ring_buffer_info *rbi = &channel->inbound;

	
	virt_mb();

	pending_sz = READ_ONCE(rbi->ring_buffer->pending_send_sz);
	
	if (pending_sz == 0)
		return;

	cur_write_sz = hv_get_bytes_to_write(rbi);

	if (cur_write_sz <= pending_sz)
		return;

	cached_write_sz = hv_get_cached_bytes_to_write(rbi);
	if (cached_write_sz <= pending_sz)
		vmbus_setevent(channel);

	return;
}

static inline void
init_cached_read_index(struct vmbus_channel *channel)
{
	struct hv_ring_buffer_info *rbi = &channel->inbound;

	rbi->cached_read_index = rbi->ring_buffer->read_index;
}


#define VMBUS_PKT_TRAILER	8

static inline struct vmpacket_descriptor *
get_next_pkt_raw(struct vmbus_channel *channel)
{
	struct hv_ring_buffer_info *ring_info = &channel->inbound;
	u32 priv_read_loc = ring_info->priv_read_index;
	void *ring_buffer = hv_get_ring_buffer(ring_info);
	u32 dsize = ring_info->ring_datasize;
	
	u32 delta = priv_read_loc >= ring_info->ring_buffer->read_index ?
		priv_read_loc - ring_info->ring_buffer->read_index :
		(dsize - ring_info->ring_buffer->read_index) + priv_read_loc;
	u32 bytes_avail_toread = (hv_get_bytes_to_read(ring_info) - delta);

	if (bytes_avail_toread < sizeof(struct vmpacket_descriptor))
		return NULL;

	return ring_buffer + priv_read_loc;
}


static inline void put_pkt_raw(struct vmbus_channel *channel,
				struct vmpacket_descriptor *desc)
{
	struct hv_ring_buffer_info *ring_info = &channel->inbound;
	u32 packetlen = desc->len8 << 3;
	u32 dsize = ring_info->ring_datasize;

	
	ring_info->priv_read_index += packetlen + VMBUS_PKT_TRAILER;
	ring_info->priv_read_index %= dsize;
}


static inline void commit_rd_index(struct vmbus_channel *channel)
{
	struct hv_ring_buffer_info *ring_info = &channel->inbound;
	
	virt_rmb();
	ring_info->ring_buffer->read_index = ring_info->priv_read_index;

	hv_signal_on_read(channel);
}


#endif 
