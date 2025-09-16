

#ifndef _LINUX_SKBUFF_H
#define _LINUX_SKBUFF_H

#include <linux/kernel.h>
#include <linux/kmemcheck.h>
#include <linux/compiler.h>
#include <linux/time.h>
#include <linux/bug.h>
#include <linux/cache.h>
#include <linux/rbtree.h>
#include <linux/socket.h>

#include <linux/atomic.h>
#include <asm/types.h>
#include <linux/spinlock.h>
#include <linux/net.h>
#include <linux/textsearch.h>
#include <net/checksum.h>
#include <linux/rcupdate.h>
#include <linux/hrtimer.h>
#include <linux/dma-mapping.h>
#include <linux/netdev_features.h>
#include <linux/sched.h>
#include <net/flow_dissector.h>
#include <linux/splice.h>
#include <linux/in6.h>
#include <linux/if_packet.h>
#include <net/flow.h>




#define CHECKSUM_NONE		0
#define CHECKSUM_UNNECESSARY	1
#define CHECKSUM_COMPLETE	2
#define CHECKSUM_PARTIAL	3


#define SKB_MAX_CSUM_LEVEL	3

#define SKB_DATA_ALIGN(X)	ALIGN(X, SMP_CACHE_BYTES)
#define SKB_WITH_OVERHEAD(X)	\
	((X) - SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))
#define SKB_MAX_ORDER(X, ORDER) \
	SKB_WITH_OVERHEAD((PAGE_SIZE << (ORDER)) - (X))
#define SKB_MAX_HEAD(X)		(SKB_MAX_ORDER((X), 0))
#define SKB_MAX_ALLOC		(SKB_MAX_ORDER(0, 2))


#define SKB_TRUESIZE(X) ((X) +						\
			 SKB_DATA_ALIGN(sizeof(struct sk_buff)) +	\
			 SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))

struct net_device;
struct scatterlist;
struct pipe_inode_info;
struct iov_iter;
struct napi_struct;

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
struct nf_conntrack {
	atomic_t use;
};
#endif

#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
struct nf_bridge_info {
	atomic_t		use;
	enum {
		BRNF_PROTO_UNCHANGED,
		BRNF_PROTO_8021Q,
		BRNF_PROTO_PPPOE
	} orig_proto:8;
	u8			pkt_otherhost:1;
	u8			in_prerouting:1;
	u8			bridged_dnat:1;
	__u16			frag_max_size;
	struct net_device	*physindev;

	
	struct net_device	*physoutdev;
	union {
		
		__be32          ipv4_daddr;
		struct in6_addr ipv6_daddr;

		
		char neigh_header[8];
	};
};
#endif

struct sk_buff_head {
	
	struct sk_buff	*next;
	struct sk_buff	*prev;

	__u32		qlen;
	spinlock_t	lock;
};

struct sk_buff;


#if (65536/PAGE_SIZE + 1) < 16
#define MAX_SKB_FRAGS 16UL
#else
#define MAX_SKB_FRAGS (65536/PAGE_SIZE + 1)
#endif
extern int sysctl_max_skb_frags;


#define GSO_BY_FRAGS	0xFFFF

typedef struct skb_frag_struct skb_frag_t;

struct skb_frag_struct {
	struct {
		struct page *p;
	} page;
#if (BITS_PER_LONG > 32) || (PAGE_SIZE >= 65536)
	__u32 page_offset;
	__u32 size;
#else
	__u16 page_offset;
	__u16 size;
#endif
};

static inline unsigned int skb_frag_size(const skb_frag_t *frag)
{
	return frag->size;
}

static inline void skb_frag_size_set(skb_frag_t *frag, unsigned int size)
{
	frag->size = size;
}

static inline void skb_frag_size_add(skb_frag_t *frag, int delta)
{
	frag->size += delta;
}

static inline void skb_frag_size_sub(skb_frag_t *frag, int delta)
{
	frag->size -= delta;
}

#define HAVE_HW_TIME_STAMP


struct skb_shared_hwtstamps {
	ktime_t	hwtstamp;
};


enum {
	
	SKBTX_HW_TSTAMP = 1 << 0,

	
	SKBTX_SW_TSTAMP = 1 << 1,

	
	SKBTX_IN_PROGRESS = 1 << 2,

	
	SKBTX_DEV_ZEROCOPY = 1 << 3,

	
	SKBTX_WIFI_STATUS = 1 << 4,

	
	SKBTX_SHARED_FRAG = 1 << 5,

	
	SKBTX_SCHED_TSTAMP = 1 << 6,
};

#define SKBTX_ANY_SW_TSTAMP	(SKBTX_SW_TSTAMP    | \
				 SKBTX_SCHED_TSTAMP)
#define SKBTX_ANY_TSTAMP	(SKBTX_HW_TSTAMP | SKBTX_ANY_SW_TSTAMP)


struct ubuf_info {
	void (*callback)(struct ubuf_info *, bool zerocopy_success);
	void *ctx;
	unsigned long desc;
};


struct skb_shared_info {
	__u8		__unused;
	__u8		meta_len;
	__u8		nr_frags;
	__u8		tx_flags;
	unsigned short	gso_size;
	
	unsigned short	gso_segs;
	struct sk_buff	*frag_list;
	struct skb_shared_hwtstamps hwtstamps;
	unsigned int	gso_type;
	u32		tskey;
	__be32          ip6_frag_id;

	
	atomic_t	dataref;

	
	void *		destructor_arg;

// ------------- START of KNOX_VPN ------------------//
	uid_t uid;
	pid_t pid;
	u_int32_t knox_mark;
// ------------- END of KNOX_VPN -------------------//

	
	skb_frag_t	frags[MAX_SKB_FRAGS];
};


#define SKB_DATAREF_SHIFT 16
#define SKB_DATAREF_MASK ((1 << SKB_DATAREF_SHIFT) - 1)


enum {
	SKB_FCLONE_UNAVAILABLE,	
	SKB_FCLONE_ORIG,	
	SKB_FCLONE_CLONE,	
};

enum {
	SKB_GSO_TCPV4 = 1 << 0,
	SKB_GSO_UDP = 1 << 1,

	
	SKB_GSO_DODGY = 1 << 2,

	
	SKB_GSO_TCP_ECN = 1 << 3,

	SKB_GSO_TCP_FIXEDID = 1 << 4,

	SKB_GSO_TCPV6 = 1 << 5,

	SKB_GSO_FCOE = 1 << 6,

	SKB_GSO_GRE = 1 << 7,

	SKB_GSO_GRE_CSUM = 1 << 8,

	SKB_GSO_IPXIP4 = 1 << 9,

	SKB_GSO_IPXIP6 = 1 << 10,

	SKB_GSO_UDP_TUNNEL = 1 << 11,

	SKB_GSO_UDP_TUNNEL_CSUM = 1 << 12,

	SKB_GSO_PARTIAL = 1 << 13,

	SKB_GSO_TUNNEL_REMCSUM = 1 << 14,

	SKB_GSO_SCTP = 1 << 15,
};

#if BITS_PER_LONG > 32
#define NET_SKBUFF_DATA_USES_OFFSET 1
#endif

#ifdef NET_SKBUFF_DATA_USES_OFFSET
typedef unsigned int sk_buff_data_t;
#else
typedef unsigned char *sk_buff_data_t;
#endif


struct skb_mstamp {
	union {
		u64		v64;
		struct {
			u32	stamp_us;
			u32	stamp_jiffies;
		};
	};
};


static inline void skb_mstamp_get(struct skb_mstamp *cl)
{
	u64 val = local_clock();

	do_div(val, NSEC_PER_USEC);
	cl->stamp_us = (u32)val;
	cl->stamp_jiffies = (u32)jiffies;
}


static inline u32 skb_mstamp_us_delta(const struct skb_mstamp *t1,
				      const struct skb_mstamp *t0)
{
	s32 delta_us = t1->stamp_us - t0->stamp_us;
	u32 delta_jiffies = t1->stamp_jiffies - t0->stamp_jiffies;

	
	if (delta_us <= 0 ||
	    delta_jiffies >= (INT_MAX / (USEC_PER_SEC / HZ)))

		delta_us = jiffies_to_usecs(delta_jiffies);

	return delta_us;
}

static inline bool skb_mstamp_after(const struct skb_mstamp *t1,
				    const struct skb_mstamp *t0)
{
	s32 diff = t1->stamp_jiffies - t0->stamp_jiffies;

	if (!diff)
		diff = t1->stamp_us - t0->stamp_us;
	return diff > 0;
}



struct sk_buff {
	union {
		struct {
			
			struct sk_buff		*next;
			struct sk_buff		*prev;

			union {
				ktime_t		tstamp;
				struct skb_mstamp skb_mstamp;
			};
		};
		struct rb_node		rbnode; 
	};

	union {
		struct sock		*sk;
		int			ip_defrag_offset;
	};

	struct net_device	*dev;

	
#ifdef CONFIG_MPTCP
	char			cb[72] __aligned(8);
#else
	char			cb[48] __aligned(8);
#endif

	unsigned long		_skb_refdst;
	void			(*destructor)(struct sk_buff *skb);
#ifdef CONFIG_XFRM
	struct	sec_path	*sp;
#endif
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	struct nf_conntrack	*nfct;
#endif
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	struct nf_bridge_info	*nf_bridge;
#endif
	unsigned int		len,
				data_len;
	__u16			mac_len,
				hdr_len;

	
	kmemcheck_bitfield_begin(flags1);
	__u16			queue_mapping;


#ifdef __BIG_ENDIAN_BITFIELD
#define CLONED_MASK	(1 << 7)
#else
#define CLONED_MASK	1
#endif
#define CLONED_OFFSET()		offsetof(struct sk_buff, __cloned_offset)

	__u8			__cloned_offset[0];
	__u8			cloned:1,
				nohdr:1,
				fclone:2,
				peeked:1,
				head_frag:1,
				xmit_more:1,
				pfmemalloc:1;
	kmemcheck_bitfield_end(flags1);

	
	
	__u32			headers_start[0];
	


#ifdef __BIG_ENDIAN_BITFIELD
#define PKT_TYPE_MAX	(7 << 5)
#else
#define PKT_TYPE_MAX	7
#endif
#define PKT_TYPE_OFFSET()	offsetof(struct sk_buff, __pkt_type_offset)

	__u8			__pkt_type_offset[0];
	__u8			pkt_type:3;
	__u8			ignore_df:1;
	__u8			nfctinfo:3;
	__u8			nf_trace:1;

	__u8			ip_summed:2;
	__u8			ooo_okay:1;
	__u8			l4_hash:1;
	__u8			sw_hash:1;
	__u8			wifi_acked_valid:1;
	__u8			wifi_acked:1;
	__u8			no_fcs:1;

	
	__u8			encapsulation:1;
	__u8			encap_hdr_csum:1;
	__u8			csum_valid:1;
	__u8			csum_complete_sw:1;
	__u8			csum_level:2;
	__u8			csum_bad:1;
#ifdef CONFIG_IPV6_NDISC_NODETYPE
	__u8			ndisc_nodetype:2;
#endif
	__u8			ipvs_property:1;

	__u8			inner_protocol_type:1;
	__u8			remcsum_offload:1;
#ifdef CONFIG_NET_SWITCHDEV
	__u8			offload_fwd_mark:1;
#endif
	

#ifdef CONFIG_NET_SCHED
	__u16			tc_index;	
#ifdef CONFIG_NET_CLS_ACT
	__u16			tc_verd;	
#endif
#endif

	union {
		__wsum		csum;
		struct {
			__u16	csum_start;
			__u16	csum_offset;
		};
	};
	__u32			priority;
	int			skb_iif;
	__u32			hash;
	__be16			vlan_proto;
	__u16			vlan_tci;
#if defined(CONFIG_NET_RX_BUSY_POLL) || defined(CONFIG_XPS)
	union {
		unsigned int	napi_id;
		unsigned int	sender_cpu;
	};
#endif
#ifdef CONFIG_NETWORK_SECMARK
	__u32		secmark;
#endif

	__u32			priomark;

	union {
		__u32		mark;
		__u32		reserved_tailroom;
	};

	union {
		__be16		inner_protocol;
		__u8		inner_ipproto;
	};

	__u16			inner_transport_header;
	__u16			inner_network_header;
	__u16			inner_mac_header;

	__be16			protocol;
	__u16			transport_header;
	__u16			network_header;
	__u16			mac_header;

	
	__u32			headers_end[0];
	

	
	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head,
				*data;
	unsigned int		truesize;
	atomic_t		users;
};

#ifdef __KERNEL__

#include <linux/slab.h>


#define SKB_ALLOC_FCLONE	0x01
#define SKB_ALLOC_RX		0x02
#define SKB_ALLOC_NAPI		0x04


static inline bool skb_pfmemalloc(const struct sk_buff *skb)
{
	return unlikely(skb->pfmemalloc);
}


#define SKB_DST_NOREF	1UL
#define SKB_DST_PTRMASK	~(SKB_DST_NOREF)


static inline struct dst_entry *skb_dst(const struct sk_buff *skb)
{
	
	WARN_ON((skb->_skb_refdst & SKB_DST_NOREF) &&
		!rcu_read_lock_held() &&
		!rcu_read_lock_bh_held());
	return (struct dst_entry *)(skb->_skb_refdst & SKB_DST_PTRMASK);
}


static inline void skb_dst_set(struct sk_buff *skb, struct dst_entry *dst)
{
	skb->_skb_refdst = (unsigned long)dst;
}


static inline void skb_dst_set_noref(struct sk_buff *skb, struct dst_entry *dst)
{
	WARN_ON(!rcu_read_lock_held() && !rcu_read_lock_bh_held());
	skb->_skb_refdst = (unsigned long)dst | SKB_DST_NOREF;
}


static inline bool skb_dst_is_noref(const struct sk_buff *skb)
{
	return (skb->_skb_refdst & SKB_DST_NOREF) && skb_dst(skb);
}

static inline struct rtable *skb_rtable(const struct sk_buff *skb)
{
	return (struct rtable *)skb_dst(skb);
}


static inline bool skb_pkt_type_ok(u32 ptype)
{
	return ptype <= PACKET_OTHERHOST;
}

void kfree_skb(struct sk_buff *skb);
void kfree_skb_list(struct sk_buff *segs);
void skb_tx_error(struct sk_buff *skb);
void consume_skb(struct sk_buff *skb);
void  __kfree_skb(struct sk_buff *skb);
extern struct kmem_cache *skbuff_head_cache;

void kfree_skb_partial(struct sk_buff *skb, bool head_stolen);
bool skb_try_coalesce(struct sk_buff *to, struct sk_buff *from,
		      bool *fragstolen, int *delta_truesize);

struct sk_buff *__alloc_skb(unsigned int size, gfp_t priority, int flags,
			    int node);
struct sk_buff *__build_skb(void *data, unsigned int frag_size);
struct sk_buff *build_skb(void *data, unsigned int frag_size);
static inline struct sk_buff *alloc_skb(unsigned int size,
					gfp_t priority)
{
	return __alloc_skb(size, priority, 0, NUMA_NO_NODE);
}

struct sk_buff *alloc_skb_with_frags(unsigned long header_len,
				     unsigned long data_len,
				     int max_page_order,
				     int *errcode,
				     gfp_t gfp_mask);


struct sk_buff_fclones {
	struct sk_buff	skb1;

	struct sk_buff	skb2;

	atomic_t	fclone_ref;
};


static inline bool skb_fclone_busy(const struct sock *sk,
				   const struct sk_buff *skb)
{
	const struct sk_buff_fclones *fclones;

	fclones = container_of(skb, struct sk_buff_fclones, skb1);

	return skb->fclone == SKB_FCLONE_ORIG &&
	       atomic_read(&fclones->fclone_ref) > 1 &&
	       fclones->skb2.sk == sk;
}

static inline struct sk_buff *alloc_skb_fclone(unsigned int size,
					       gfp_t priority)
{
	return __alloc_skb(size, priority, SKB_ALLOC_FCLONE, NUMA_NO_NODE);
}

struct sk_buff *__alloc_skb_head(gfp_t priority, int node);
static inline struct sk_buff *alloc_skb_head(gfp_t priority)
{
	return __alloc_skb_head(priority, -1);
}

struct sk_buff *skb_morph(struct sk_buff *dst, struct sk_buff *src);
int skb_copy_ubufs(struct sk_buff *skb, gfp_t gfp_mask);
struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t priority);
void skb_copy_header(struct sk_buff *new, const struct sk_buff *old);
struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t priority);
struct sk_buff *__pskb_copy_fclone(struct sk_buff *skb, int headroom,
				   gfp_t gfp_mask, bool fclone);
static inline struct sk_buff *__pskb_copy(struct sk_buff *skb, int headroom,
					  gfp_t gfp_mask)
{
	return __pskb_copy_fclone(skb, headroom, gfp_mask, false);
}

int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail, gfp_t gfp_mask);
struct sk_buff *skb_realloc_headroom(struct sk_buff *skb,
				     unsigned int headroom);
struct sk_buff *skb_copy_expand(const struct sk_buff *skb, int newheadroom,
				int newtailroom, gfp_t priority);
int __must_check skb_to_sgvec_nomark(struct sk_buff *skb, struct scatterlist *sg,
				     int offset, int len);
int __must_check skb_to_sgvec(struct sk_buff *skb, struct scatterlist *sg,
			      int offset, int len);
int skb_cow_data(struct sk_buff *skb, int tailbits, struct sk_buff **trailer);
int skb_pad(struct sk_buff *skb, int pad);
#define dev_kfree_skb(a)	consume_skb(a)

int skb_append_datato_frags(struct sock *sk, struct sk_buff *skb,
			    int getfrag(void *from, char *to, int offset,
					int len, int odd, struct sk_buff *skb),
			    void *from, int length);

int skb_append_pagefrags(struct sk_buff *skb, struct page *page,
			 int offset, size_t size);

struct skb_seq_state {
	__u32		lower_offset;
	__u32		upper_offset;
	__u32		frag_idx;
	__u32		stepped_offset;
	struct sk_buff	*root_skb;
	struct sk_buff	*cur_skb;
	__u8		*frag_data;
};

void skb_prepare_seq_read(struct sk_buff *skb, unsigned int from,
			  unsigned int to, struct skb_seq_state *st);
unsigned int skb_seq_read(unsigned int consumed, const u8 **data,
			  struct skb_seq_state *st);
void skb_abort_seq_read(struct skb_seq_state *st);

unsigned int skb_find_text(struct sk_buff *skb, unsigned int from,
			   unsigned int to, struct ts_config *config);


enum pkt_hash_types {
	PKT_HASH_TYPE_NONE,	
	PKT_HASH_TYPE_L2,	
	PKT_HASH_TYPE_L3,	
	PKT_HASH_TYPE_L4,	
};

static inline void skb_clear_hash(struct sk_buff *skb)
{
	skb->hash = 0;
	skb->sw_hash = 0;
	skb->l4_hash = 0;
}

static inline void skb_clear_hash_if_not_l4(struct sk_buff *skb)
{
	if (!skb->l4_hash)
		skb_clear_hash(skb);
}

static inline void
__skb_set_hash(struct sk_buff *skb, __u32 hash, bool is_sw, bool is_l4)
{
	skb->l4_hash = is_l4;
	skb->sw_hash = is_sw;
	skb->hash = hash;
}

static inline void
skb_set_hash(struct sk_buff *skb, __u32 hash, enum pkt_hash_types type)
{
	
	__skb_set_hash(skb, hash, false, type == PKT_HASH_TYPE_L4);
}

static inline void
__skb_set_sw_hash(struct sk_buff *skb, __u32 hash, bool is_l4)
{
	__skb_set_hash(skb, hash, true, is_l4);
}

void __skb_get_hash(struct sk_buff *skb);
u32 __skb_get_hash_symmetric(struct sk_buff *skb);
u32 skb_get_poff(const struct sk_buff *skb);
u32 __skb_get_poff(const struct sk_buff *skb, void *data,
		   const struct flow_keys *keys, int hlen);
__be32 __skb_flow_get_ports(const struct sk_buff *skb, int thoff, u8 ip_proto,
			    void *data, int hlen_proto);

static inline __be32 skb_flow_get_ports(const struct sk_buff *skb,
					int thoff, u8 ip_proto)
{
	return __skb_flow_get_ports(skb, thoff, ip_proto, NULL, 0);
}

void skb_flow_dissector_init(struct flow_dissector *flow_dissector,
			     const struct flow_dissector_key *key,
			     unsigned int key_count);

bool __skb_flow_dissect(const struct sk_buff *skb,
			struct flow_dissector *flow_dissector,
			void *target_container,
			void *data, __be16 proto, int nhoff, int hlen,
			unsigned int flags);

static inline bool skb_flow_dissect(const struct sk_buff *skb,
				    struct flow_dissector *flow_dissector,
				    void *target_container, unsigned int flags)
{
	return __skb_flow_dissect(skb, flow_dissector, target_container,
				  NULL, 0, 0, 0, flags);
}

static inline bool skb_flow_dissect_flow_keys(const struct sk_buff *skb,
					      struct flow_keys *flow,
					      unsigned int flags)
{
	memset(flow, 0, sizeof(*flow));
	return __skb_flow_dissect(skb, &flow_keys_dissector, flow,
				  NULL, 0, 0, 0, flags);
}

static inline bool skb_flow_dissect_flow_keys_buf(struct flow_keys *flow,
						  void *data, __be16 proto,
						  int nhoff, int hlen,
						  unsigned int flags)
{
	memset(flow, 0, sizeof(*flow));
	return __skb_flow_dissect(NULL, &flow_keys_buf_dissector, flow,
				  data, proto, nhoff, hlen, flags);
}

static inline __u32 skb_get_hash(struct sk_buff *skb)
{
	if (!skb->l4_hash && !skb->sw_hash)
		__skb_get_hash(skb);

	return skb->hash;
}

__u32 __skb_get_hash_flowi6(struct sk_buff *skb, const struct flowi6 *fl6);

static inline __u32 skb_get_hash_flowi6(struct sk_buff *skb, const struct flowi6 *fl6)
{
	if (!skb->l4_hash && !skb->sw_hash) {
		struct flow_keys keys;
		__u32 hash = __get_hash_from_flowi6(fl6, &keys);

		__skb_set_sw_hash(skb, hash, flow_keys_have_l4(&keys));
	}

	return skb->hash;
}

__u32 __skb_get_hash_flowi4(struct sk_buff *skb, const struct flowi4 *fl);

static inline __u32 skb_get_hash_flowi4(struct sk_buff *skb, const struct flowi4 *fl4)
{
	if (!skb->l4_hash && !skb->sw_hash) {
		struct flow_keys keys;
		__u32 hash = __get_hash_from_flowi4(fl4, &keys);

		__skb_set_sw_hash(skb, hash, flow_keys_have_l4(&keys));
	}

	return skb->hash;
}

__u32 skb_get_hash_perturb(const struct sk_buff *skb,
			   const siphash_key_t *perturb);

static inline __u32 skb_get_hash_raw(const struct sk_buff *skb)
{
	return skb->hash;
}

static inline void skb_copy_hash(struct sk_buff *to, const struct sk_buff *from)
{
	to->hash = from->hash;
	to->sw_hash = from->sw_hash;
	to->l4_hash = from->l4_hash;
};

#ifdef NET_SKBUFF_DATA_USES_OFFSET
static inline unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->head + skb->end;
}

static inline unsigned int skb_end_offset(const struct sk_buff *skb)
{
	return skb->end;
}
#else
static inline unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->end;
}

static inline unsigned int skb_end_offset(const struct sk_buff *skb)
{
	return skb->end - skb->head;
}
#endif


#define skb_shinfo(SKB)	((struct skb_shared_info *)(skb_end_pointer(SKB)))

static inline struct skb_shared_hwtstamps *skb_hwtstamps(struct sk_buff *skb)
{
	return &skb_shinfo(skb)->hwtstamps;
}


static inline int skb_queue_empty(const struct sk_buff_head *list)
{
	return list->next == (const struct sk_buff *) list;
}


static inline bool skb_queue_is_last(const struct sk_buff_head *list,
				     const struct sk_buff *skb)
{
	return skb->next == (const struct sk_buff *) list;
}


static inline bool skb_queue_is_first(const struct sk_buff_head *list,
				      const struct sk_buff *skb)
{
	return skb->prev == (const struct sk_buff *) list;
}


static inline struct sk_buff *skb_queue_next(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	
	BUG_ON(skb_queue_is_last(list, skb));
	return skb->next;
}


static inline struct sk_buff *skb_queue_prev(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	
	BUG_ON(skb_queue_is_first(list, skb));
	return skb->prev;
}


static inline struct sk_buff *skb_get(struct sk_buff *skb)
{
	atomic_inc(&skb->users);
	return skb;
}




static inline int skb_cloned(const struct sk_buff *skb)
{
	return skb->cloned &&
	       (atomic_read(&skb_shinfo(skb)->dataref) & SKB_DATAREF_MASK) != 1;
}

static inline int skb_unclone(struct sk_buff *skb, gfp_t pri)
{
	might_sleep_if(gfpflags_allow_blocking(pri));

	if (skb_cloned(skb))
		return pskb_expand_head(skb, 0, 0, pri);

	return 0;
}


static inline int skb_header_cloned(const struct sk_buff *skb)
{
	int dataref;

	if (!skb->cloned)
		return 0;

	dataref = atomic_read(&skb_shinfo(skb)->dataref);
	dataref = (dataref & SKB_DATAREF_MASK) - (dataref >> SKB_DATAREF_SHIFT);
	return dataref != 1;
}

static inline int skb_header_unclone(struct sk_buff *skb, gfp_t pri)
{
	might_sleep_if(gfpflags_allow_blocking(pri));

	if (skb_header_cloned(skb))
		return pskb_expand_head(skb, 0, 0, pri);

	return 0;
}


static inline void skb_header_release(struct sk_buff *skb)
{
	BUG_ON(skb->nohdr);
	skb->nohdr = 1;
	atomic_add(1 << SKB_DATAREF_SHIFT, &skb_shinfo(skb)->dataref);
}


static inline void __skb_header_release(struct sk_buff *skb)
{
	skb->nohdr = 1;
	atomic_set(&skb_shinfo(skb)->dataref, 1 + (1 << SKB_DATAREF_SHIFT));
}



static inline int skb_shared(const struct sk_buff *skb)
{
	return atomic_read(&skb->users) != 1;
}


static inline struct sk_buff *skb_share_check(struct sk_buff *skb, gfp_t pri)
{
	might_sleep_if(gfpflags_allow_blocking(pri));
	if (skb_shared(skb)) {
		struct sk_buff *nskb = skb_clone(skb, pri);

		if (likely(nskb))
			consume_skb(skb);
		else
			kfree_skb(skb);
		skb = nskb;
	}
	return skb;
}




static inline struct sk_buff *skb_unshare(struct sk_buff *skb,
					  gfp_t pri)
{
	might_sleep_if(gfpflags_allow_blocking(pri));
	if (skb_cloned(skb)) {
		struct sk_buff *nskb = skb_copy(skb, pri);

		
		if (likely(nskb))
			consume_skb(skb);
		else
			kfree_skb(skb);
		skb = nskb;
	}
	return skb;
}


static inline struct sk_buff *skb_peek(const struct sk_buff_head *list_)
{
	struct sk_buff *skb = list_->next;

	if (skb == (struct sk_buff *)list_)
		skb = NULL;
	return skb;
}


static inline struct sk_buff *skb_peek_next(struct sk_buff *skb,
		const struct sk_buff_head *list_)
{
	struct sk_buff *next = skb->next;

	if (next == (struct sk_buff *)list_)
		next = NULL;
	return next;
}


static inline struct sk_buff *skb_peek_tail(const struct sk_buff_head *list_)
{
	struct sk_buff *skb = list_->prev;

	if (skb == (struct sk_buff *)list_)
		skb = NULL;
	return skb;

}


static inline __u32 skb_queue_len(const struct sk_buff_head *list_)
{
	return list_->qlen;
}


static inline __u32 skb_queue_len_lockless(const struct sk_buff_head *list_)
{
	return READ_ONCE(list_->qlen);
}


static inline void __skb_queue_head_init(struct sk_buff_head *list)
{
	list->prev = list->next = (struct sk_buff *)list;
	list->qlen = 0;
}


static inline void skb_queue_head_init(struct sk_buff_head *list)
{
	spin_lock_init(&list->lock);
	__skb_queue_head_init(list);
}

static inline void skb_queue_head_init_class(struct sk_buff_head *list,
		struct lock_class_key *class)
{
	skb_queue_head_init(list);
	lockdep_set_class(&list->lock, class);
}


void skb_insert(struct sk_buff *old, struct sk_buff *newsk,
		struct sk_buff_head *list);
static inline void __skb_insert(struct sk_buff *newsk,
				struct sk_buff *prev, struct sk_buff *next,
				struct sk_buff_head *list)
{
	newsk->next = next;
	newsk->prev = prev;
	next->prev  = prev->next = newsk;
	WRITE_ONCE(list->qlen, list->qlen + 1);
}

static inline void __skb_queue_splice(const struct sk_buff_head *list,
				      struct sk_buff *prev,
				      struct sk_buff *next)
{
	struct sk_buff *first = list->next;
	struct sk_buff *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}


static inline void skb_queue_splice(const struct sk_buff_head *list,
				    struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		__skb_queue_splice(list, (struct sk_buff *) head, head->next);
		head->qlen += list->qlen;
	}
}


static inline void skb_queue_splice_init(struct sk_buff_head *list,
					 struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		__skb_queue_splice(list, (struct sk_buff *) head, head->next);
		head->qlen += list->qlen;
		__skb_queue_head_init(list);
	}
}


static inline void skb_queue_splice_tail(const struct sk_buff_head *list,
					 struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		__skb_queue_splice(list, head->prev, (struct sk_buff *) head);
		head->qlen += list->qlen;
	}
}


static inline void skb_queue_splice_tail_init(struct sk_buff_head *list,
					      struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		__skb_queue_splice(list, head->prev, (struct sk_buff *) head);
		head->qlen += list->qlen;
		__skb_queue_head_init(list);
	}
}


static inline void __skb_queue_after(struct sk_buff_head *list,
				     struct sk_buff *prev,
				     struct sk_buff *newsk)
{
	__skb_insert(newsk, prev, prev->next, list);
}

void skb_append(struct sk_buff *old, struct sk_buff *newsk,
		struct sk_buff_head *list);

static inline void __skb_queue_before(struct sk_buff_head *list,
				      struct sk_buff *next,
				      struct sk_buff *newsk)
{
	__skb_insert(newsk, next->prev, next, list);
}


void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);
static inline void __skb_queue_head(struct sk_buff_head *list,
				    struct sk_buff *newsk)
{
	__skb_queue_after(list, (struct sk_buff *)list, newsk);
}


void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);
static inline void __skb_queue_tail(struct sk_buff_head *list,
				   struct sk_buff *newsk)
{
	__skb_queue_before(list, (struct sk_buff *)list, newsk);
}


void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list);
static inline void __skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
	struct sk_buff *next, *prev;

	WRITE_ONCE(list->qlen, list->qlen - 1);
	next	   = skb->next;
	prev	   = skb->prev;
	skb->next  = skb->prev = NULL;
	next->prev = prev;
	prev->next = next;
}


struct sk_buff *skb_dequeue(struct sk_buff_head *list);
static inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
	struct sk_buff *skb = skb_peek(list);
	if (skb)
		__skb_unlink(skb, list);
	return skb;
}


struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list);
static inline struct sk_buff *__skb_dequeue_tail(struct sk_buff_head *list)
{
	struct sk_buff *skb = skb_peek_tail(list);
	if (skb)
		__skb_unlink(skb, list);
	return skb;
}


static inline bool skb_is_nonlinear(const struct sk_buff *skb)
{
	return skb->data_len;
}

static inline unsigned int skb_headlen(const struct sk_buff *skb)
{
	return skb->len - skb->data_len;
}

static inline int skb_pagelen(const struct sk_buff *skb)
{
	int i, len = 0;

	for (i = (int)skb_shinfo(skb)->nr_frags - 1; i >= 0; i--)
		len += skb_frag_size(&skb_shinfo(skb)->frags[i]);
	return len + skb_headlen(skb);
}


static inline void __skb_fill_page_desc(struct sk_buff *skb, int i,
					struct page *page, int off, int size)
{
	skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

	
	frag->page.p		  = page;
	frag->page_offset	  = off;
	skb_frag_size_set(frag, size);

	page = compound_head(page);
	if (page_is_pfmemalloc(page))
		skb->pfmemalloc	= true;
}


static inline void skb_fill_page_desc(struct sk_buff *skb, int i,
				      struct page *page, int off, int size)
{
	__skb_fill_page_desc(skb, i, page, off, size);
	skb_shinfo(skb)->nr_frags = i + 1;
}

void skb_add_rx_frag(struct sk_buff *skb, int i, struct page *page, int off,
		     int size, unsigned int truesize);

void skb_coalesce_rx_frag(struct sk_buff *skb, int i, int size,
			  unsigned int truesize);

#define SKB_PAGE_ASSERT(skb) 	BUG_ON(skb_shinfo(skb)->nr_frags)
#define SKB_FRAG_ASSERT(skb) 	BUG_ON(skb_has_frag_list(skb))
#define SKB_LINEAR_ASSERT(skb)  BUG_ON(skb_is_nonlinear(skb))

#ifdef NET_SKBUFF_DATA_USES_OFFSET
static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->head + skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data - skb->head;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb_reset_tail_pointer(skb);
	skb->tail += offset;
}

#else 
static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb->tail = skb->data + offset;
}

#endif 


unsigned char *pskb_put(struct sk_buff *skb, struct sk_buff *tail, int len);
unsigned char *skb_put(struct sk_buff *skb, unsigned int len);
static inline unsigned char *__skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb_tail_pointer(skb);
	SKB_LINEAR_ASSERT(skb);
	skb->tail += len;
	skb->len  += len;
	return tmp;
}

unsigned char *skb_push(struct sk_buff *skb, unsigned int len);
static inline unsigned char *__skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len  += len;
	return skb->data;
}

unsigned char *skb_pull(struct sk_buff *skb, unsigned int len);
static inline unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;
	BUG_ON(skb->len < skb->data_len);
	return skb->data += len;
}

static inline unsigned char *skb_pull_inline(struct sk_buff *skb, unsigned int len)
{
	return unlikely(len > skb->len) ? NULL : __skb_pull(skb, len);
}

unsigned char *__pskb_pull_tail(struct sk_buff *skb, int delta);

static inline unsigned char *__pskb_pull(struct sk_buff *skb, unsigned int len)
{
	if (len > skb_headlen(skb) &&
	    !__pskb_pull_tail(skb, len - skb_headlen(skb)))
		return NULL;
	skb->len -= len;
	return skb->data += len;
}

static inline unsigned char *pskb_pull(struct sk_buff *skb, unsigned int len)
{
	return unlikely(len > skb->len) ? NULL : __pskb_pull(skb, len);
}

static inline int pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
	if (likely(len <= skb_headlen(skb)))
		return 1;
	if (unlikely(len > skb->len))
		return 0;
	return __pskb_pull_tail(skb, len - skb_headlen(skb)) != NULL;
}


static inline unsigned int skb_headroom(const struct sk_buff *skb)
{
	return skb->data - skb->head;
}


static inline int skb_tailroom(const struct sk_buff *skb)
{
	return skb_is_nonlinear(skb) ? 0 : skb->end - skb->tail;
}


static inline int skb_availroom(const struct sk_buff *skb)
{
	if (skb_is_nonlinear(skb))
		return 0;

	return skb->end - skb->tail - skb->reserved_tailroom;
}


static inline void skb_reserve(struct sk_buff *skb, int len)
{
	skb->data += len;
	skb->tail += len;
}


static inline void skb_tailroom_reserve(struct sk_buff *skb, unsigned int mtu,
					unsigned int needed_tailroom)
{
	SKB_LINEAR_ASSERT(skb);
	if (mtu < skb_tailroom(skb) - needed_tailroom)
		
		skb->reserved_tailroom = skb_tailroom(skb) - mtu;
	else
		
		skb->reserved_tailroom = needed_tailroom;
}

#define ENCAP_TYPE_ETHER	0
#define ENCAP_TYPE_IPPROTO	1

static inline void skb_set_inner_protocol(struct sk_buff *skb,
					  __be16 protocol)
{
	skb->inner_protocol = protocol;
	skb->inner_protocol_type = ENCAP_TYPE_ETHER;
}

static inline void skb_set_inner_ipproto(struct sk_buff *skb,
					 __u8 ipproto)
{
	skb->inner_ipproto = ipproto;
	skb->inner_protocol_type = ENCAP_TYPE_IPPROTO;
}

static inline void skb_reset_inner_headers(struct sk_buff *skb)
{
	skb->inner_mac_header = skb->mac_header;
	skb->inner_network_header = skb->network_header;
	skb->inner_transport_header = skb->transport_header;
}

static inline void skb_reset_mac_len(struct sk_buff *skb)
{
	skb->mac_len = skb->network_header - skb->mac_header;
}

static inline unsigned char *skb_inner_transport_header(const struct sk_buff
							*skb)
{
	return skb->head + skb->inner_transport_header;
}

static inline int skb_inner_transport_offset(const struct sk_buff *skb)
{
	return skb_inner_transport_header(skb) - skb->data;
}

static inline void skb_reset_inner_transport_header(struct sk_buff *skb)
{
	skb->inner_transport_header = skb->data - skb->head;
}

static inline void skb_set_inner_transport_header(struct sk_buff *skb,
						   const int offset)
{
	skb_reset_inner_transport_header(skb);
	skb->inner_transport_header += offset;
}

static inline unsigned char *skb_inner_network_header(const struct sk_buff *skb)
{
	return skb->head + skb->inner_network_header;
}

static inline void skb_reset_inner_network_header(struct sk_buff *skb)
{
	skb->inner_network_header = skb->data - skb->head;
}

static inline void skb_set_inner_network_header(struct sk_buff *skb,
						const int offset)
{
	skb_reset_inner_network_header(skb);
	skb->inner_network_header += offset;
}

static inline unsigned char *skb_inner_mac_header(const struct sk_buff *skb)
{
	return skb->head + skb->inner_mac_header;
}

static inline void skb_reset_inner_mac_header(struct sk_buff *skb)
{
	skb->inner_mac_header = skb->data - skb->head;
}

static inline void skb_set_inner_mac_header(struct sk_buff *skb,
					    const int offset)
{
	skb_reset_inner_mac_header(skb);
	skb->inner_mac_header += offset;
}
static inline bool skb_transport_header_was_set(const struct sk_buff *skb)
{
	return skb->transport_header != (typeof(skb->transport_header))~0U;
}

static inline unsigned char *skb_transport_header(const struct sk_buff *skb)
{
	return skb->head + skb->transport_header;
}

static inline void skb_reset_transport_header(struct sk_buff *skb)
{
	skb->transport_header = skb->data - skb->head;
}

static inline void skb_set_transport_header(struct sk_buff *skb,
					    const int offset)
{
	skb_reset_transport_header(skb);
	skb->transport_header += offset;
}

static inline unsigned char *skb_network_header(const struct sk_buff *skb)
{
	return skb->head + skb->network_header;
}

static inline void skb_reset_network_header(struct sk_buff *skb)
{
	skb->network_header = skb->data - skb->head;
}

static inline void skb_set_network_header(struct sk_buff *skb, const int offset)
{
	skb_reset_network_header(skb);
	skb->network_header += offset;
}

static inline unsigned char *skb_mac_header(const struct sk_buff *skb)
{
	return skb->head + skb->mac_header;
}

static inline u32 skb_mac_header_len(const struct sk_buff *skb)
{
	return skb->network_header - skb->mac_header;
}

static inline int skb_mac_header_was_set(const struct sk_buff *skb)
{
	return skb->mac_header != (typeof(skb->mac_header))~0U;
}

static inline void skb_reset_mac_header(struct sk_buff *skb)
{
	skb->mac_header = skb->data - skb->head;
}

static inline void skb_set_mac_header(struct sk_buff *skb, const int offset)
{
	skb_reset_mac_header(skb);
	skb->mac_header += offset;
}

static inline void skb_pop_mac_header(struct sk_buff *skb)
{
	skb->mac_header = skb->network_header;
}

static inline void skb_probe_transport_header(struct sk_buff *skb,
					      const int offset_hint)
{
	struct flow_keys keys;

	if (skb_transport_header_was_set(skb))
		return;
	else if (skb_flow_dissect_flow_keys(skb, &keys, 0))
		skb_set_transport_header(skb, keys.control.thoff);
	else
		skb_set_transport_header(skb, offset_hint);
}

static inline void skb_mac_header_rebuild(struct sk_buff *skb)
{
	if (skb_mac_header_was_set(skb)) {
		const unsigned char *old_mac = skb_mac_header(skb);

		skb_set_mac_header(skb, -skb->mac_len);
		memmove(skb_mac_header(skb), old_mac, skb->mac_len);
	}
}

static inline int skb_checksum_start_offset(const struct sk_buff *skb)
{
	return skb->csum_start - skb_headroom(skb);
}

static inline unsigned char *skb_checksum_start(const struct sk_buff *skb)
{
	return skb->head + skb->csum_start;
}

static inline int skb_transport_offset(const struct sk_buff *skb)
{
	return skb_transport_header(skb) - skb->data;
}

static inline u32 skb_network_header_len(const struct sk_buff *skb)
{
	return skb->transport_header - skb->network_header;
}

static inline u32 skb_inner_network_header_len(const struct sk_buff *skb)
{
	return skb->inner_transport_header - skb->inner_network_header;
}

static inline int skb_network_offset(const struct sk_buff *skb)
{
	return skb_network_header(skb) - skb->data;
}

static inline int skb_inner_network_offset(const struct sk_buff *skb)
{
	return skb_inner_network_header(skb) - skb->data;
}

static inline int pskb_network_may_pull(struct sk_buff *skb, unsigned int len)
{
	return pskb_may_pull(skb, skb_network_offset(skb) + len);
}


#ifndef NET_IP_ALIGN
#define NET_IP_ALIGN	2
#endif


#ifndef NET_SKB_PAD
#define NET_SKB_PAD	max(32, L1_CACHE_BYTES)
#endif

int ___pskb_trim(struct sk_buff *skb, unsigned int len);

static inline void __skb_set_length(struct sk_buff *skb, unsigned int len)
{
	if (unlikely(skb_is_nonlinear(skb))) {
		WARN_ON(1);
		return;
	}
	skb->len = len;
	skb_set_tail_pointer(skb, len);
}

static inline void __skb_trim(struct sk_buff *skb, unsigned int len)
{
	__skb_set_length(skb, len);
}

void skb_trim(struct sk_buff *skb, unsigned int len);

static inline int __pskb_trim(struct sk_buff *skb, unsigned int len)
{
	if (skb->data_len)
		return ___pskb_trim(skb, len);
	__skb_trim(skb, len);
	return 0;
}

static inline int pskb_trim(struct sk_buff *skb, unsigned int len)
{
	return (len < skb->len) ? __pskb_trim(skb, len) : 0;
}


static inline void pskb_trim_unique(struct sk_buff *skb, unsigned int len)
{
	int err = pskb_trim(skb, len);
	BUG_ON(err);
}

static inline int __skb_grow(struct sk_buff *skb, unsigned int len)
{
	unsigned int diff = len - skb->len;

	if (skb_tailroom(skb) < diff) {
		int ret = pskb_expand_head(skb, 0, diff - skb_tailroom(skb),
					   GFP_ATOMIC);
		if (ret)
			return ret;
	}
	__skb_set_length(skb, len);
	return 0;
}


static inline void skb_orphan(struct sk_buff *skb)
{
	if (skb->destructor) {
		skb->destructor(skb);
		skb->destructor = NULL;
		skb->sk		= NULL;
	} else {
		BUG_ON(skb->sk);
	}
}


static inline int skb_orphan_frags(struct sk_buff *skb, gfp_t gfp_mask)
{
	if (likely(!(skb_shinfo(skb)->tx_flags & SKBTX_DEV_ZEROCOPY)))
		return 0;
	return skb_copy_ubufs(skb, gfp_mask);
}


void skb_queue_purge(struct sk_buff_head *list);
static inline void __skb_queue_purge(struct sk_buff_head *list)
{
	struct sk_buff *skb;
	while ((skb = __skb_dequeue(list)) != NULL)
		kfree_skb(skb);
}

unsigned int skb_rbtree_purge(struct rb_root *root);

void *netdev_alloc_frag(unsigned int fragsz);

struct sk_buff *__netdev_alloc_skb(struct net_device *dev, unsigned int length,
				   gfp_t gfp_mask);


static inline struct sk_buff *netdev_alloc_skb(struct net_device *dev,
					       unsigned int length)
{
	return __netdev_alloc_skb(dev, length, GFP_ATOMIC);
}


static inline struct sk_buff *__dev_alloc_skb(unsigned int length,
					      gfp_t gfp_mask)
{
	return __netdev_alloc_skb(NULL, length, gfp_mask);
}


static inline struct sk_buff *dev_alloc_skb(unsigned int length)
{
	return netdev_alloc_skb(NULL, length);
}


static inline struct sk_buff *__netdev_alloc_skb_ip_align(struct net_device *dev,
		unsigned int length, gfp_t gfp)
{
	struct sk_buff *skb = __netdev_alloc_skb(dev, length + NET_IP_ALIGN, gfp);

	if (NET_IP_ALIGN && skb)
		skb_reserve(skb, NET_IP_ALIGN);
	return skb;
}

static inline struct sk_buff *netdev_alloc_skb_ip_align(struct net_device *dev,
		unsigned int length)
{
	return __netdev_alloc_skb_ip_align(dev, length, GFP_ATOMIC);
}

static inline void skb_free_frag(void *addr)
{
	page_frag_free(addr);
}

void *napi_alloc_frag(unsigned int fragsz);
struct sk_buff *__napi_alloc_skb(struct napi_struct *napi,
				 unsigned int length, gfp_t gfp_mask);
static inline struct sk_buff *napi_alloc_skb(struct napi_struct *napi,
					     unsigned int length)
{
	return __napi_alloc_skb(napi, length, GFP_ATOMIC);
}
void napi_consume_skb(struct sk_buff *skb, int budget);

void __kfree_skb_flush(void);
void __kfree_skb_defer(struct sk_buff *skb);


static inline struct page *__dev_alloc_pages(gfp_t gfp_mask,
					     unsigned int order)
{
	
	gfp_mask |= __GFP_COLD | __GFP_COMP | __GFP_MEMALLOC;

	return alloc_pages_node(NUMA_NO_NODE, gfp_mask, order);
}

static inline struct page *dev_alloc_pages(unsigned int order)
{
	return __dev_alloc_pages(GFP_ATOMIC | __GFP_NOWARN, order);
}


static inline struct page *__dev_alloc_page(gfp_t gfp_mask)
{
	return __dev_alloc_pages(gfp_mask, 0);
}

static inline struct page *dev_alloc_page(void)
{
	return dev_alloc_pages(0);
}


static inline void skb_propagate_pfmemalloc(struct page *page,
					     struct sk_buff *skb)
{
	if (page_is_pfmemalloc(page))
		skb->pfmemalloc = true;
}


static inline struct page *skb_frag_page(const skb_frag_t *frag)
{
	return frag->page.p;
}


static inline void __skb_frag_ref(skb_frag_t *frag)
{
	get_page(skb_frag_page(frag));
}


static inline void skb_frag_ref(struct sk_buff *skb, int f)
{
	__skb_frag_ref(&skb_shinfo(skb)->frags[f]);
}


static inline void __skb_frag_unref(skb_frag_t *frag)
{
	put_page(skb_frag_page(frag));
}


static inline void skb_frag_unref(struct sk_buff *skb, int f)
{
	__skb_frag_unref(&skb_shinfo(skb)->frags[f]);
}


static inline void *skb_frag_address(const skb_frag_t *frag)
{
	return page_address(skb_frag_page(frag)) + frag->page_offset;
}


static inline void *skb_frag_address_safe(const skb_frag_t *frag)
{
	void *ptr = page_address(skb_frag_page(frag));
	if (unlikely(!ptr))
		return NULL;

	return ptr + frag->page_offset;
}


static inline void __skb_frag_set_page(skb_frag_t *frag, struct page *page)
{
	frag->page.p = page;
}


static inline void skb_frag_set_page(struct sk_buff *skb, int f,
				     struct page *page)
{
	__skb_frag_set_page(&skb_shinfo(skb)->frags[f], page);
}

bool skb_page_frag_refill(unsigned int sz, struct page_frag *pfrag, gfp_t prio);


static inline dma_addr_t skb_frag_dma_map(struct device *dev,
					  const skb_frag_t *frag,
					  size_t offset, size_t size,
					  enum dma_data_direction dir)
{
	return dma_map_page(dev, skb_frag_page(frag),
			    frag->page_offset + offset, size, dir);
}

static inline struct sk_buff *pskb_copy(struct sk_buff *skb,
					gfp_t gfp_mask)
{
	return __pskb_copy(skb, skb_headroom(skb), gfp_mask);
}


static inline struct sk_buff *pskb_copy_for_clone(struct sk_buff *skb,
						  gfp_t gfp_mask)
{
	return __pskb_copy_fclone(skb, skb_headroom(skb), gfp_mask, true);
}



static inline int skb_clone_writable(const struct sk_buff *skb, unsigned int len)
{
	return !skb_header_cloned(skb) &&
	       skb_headroom(skb) + len <= skb->hdr_len;
}

static inline int skb_try_make_writable(struct sk_buff *skb,
					unsigned int write_len)
{
	return skb_cloned(skb) && !skb_clone_writable(skb, write_len) &&
	       pskb_expand_head(skb, 0, 0, GFP_ATOMIC);
}

static inline int __skb_cow(struct sk_buff *skb, unsigned int headroom,
			    int cloned)
{
	int delta = 0;

	if (headroom > skb_headroom(skb))
		delta = headroom - skb_headroom(skb);

	if (delta || cloned)
		return pskb_expand_head(skb, ALIGN(delta, NET_SKB_PAD), 0,
					GFP_ATOMIC);
	return 0;
}


static inline int skb_cow(struct sk_buff *skb, unsigned int headroom)
{
	return __skb_cow(skb, headroom, skb_cloned(skb));
}


static inline int skb_cow_head(struct sk_buff *skb, unsigned int headroom)
{
	return __skb_cow(skb, headroom, skb_header_cloned(skb));
}


static inline int skb_padto(struct sk_buff *skb, unsigned int len)
{
	unsigned int size = skb->len;
	if (likely(size >= len))
		return 0;
	return skb_pad(skb, len - size);
}


static inline int __must_check skb_put_padto(struct sk_buff *skb, unsigned int len)
{
	unsigned int size = skb->len;

	if (unlikely(size < len)) {
		len -= size;
		if (skb_pad(skb, len))
			return -ENOMEM;
		__skb_put(skb, len);
	}
	return 0;
}

static inline int skb_add_data(struct sk_buff *skb,
			       struct iov_iter *from, int copy)
{
	const int off = skb->len;

	if (skb->ip_summed == CHECKSUM_NONE) {
		__wsum csum = 0;
		if (csum_and_copy_from_iter(skb_put(skb, copy), copy,
					    &csum, from) == copy) {
			skb->csum = csum_block_add(skb->csum, csum, off);
			return 0;
		}
	} else if (copy_from_iter(skb_put(skb, copy), copy, from) == copy)
		return 0;

	__skb_trim(skb, off);
	return -EFAULT;
}

static inline bool skb_can_coalesce(struct sk_buff *skb, int i,
				    const struct page *page, int off)
{
	if (i) {
		const struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[i - 1];

		return page == skb_frag_page(frag) &&
		       off == frag->page_offset + skb_frag_size(frag);
	}
	return false;
}

static inline int __skb_linearize(struct sk_buff *skb)
{
	return __pskb_pull_tail(skb, skb->data_len) ? 0 : -ENOMEM;
}


static inline int skb_linearize(struct sk_buff *skb)
{
	return skb_is_nonlinear(skb) ? __skb_linearize(skb) : 0;
}


static inline bool skb_has_shared_frag(const struct sk_buff *skb)
{
	return skb_is_nonlinear(skb) &&
	       skb_shinfo(skb)->tx_flags & SKBTX_SHARED_FRAG;
}


static inline int skb_linearize_cow(struct sk_buff *skb)
{
	return skb_is_nonlinear(skb) || skb_cloned(skb) ?
	       __skb_linearize(skb) : 0;
}

static __always_inline void
__skb_postpull_rcsum(struct sk_buff *skb, const void *start, unsigned int len,
		     unsigned int off)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->csum = csum_block_sub(skb->csum,
					   csum_partial(start, len, 0), off);
	else if (skb->ip_summed == CHECKSUM_PARTIAL &&
		 skb_checksum_start_offset(skb) < 0)
		skb->ip_summed = CHECKSUM_NONE;
}


static inline void skb_postpull_rcsum(struct sk_buff *skb,
				      const void *start, unsigned int len)
{
	__skb_postpull_rcsum(skb, start, len, 0);
}

static __always_inline void
__skb_postpush_rcsum(struct sk_buff *skb, const void *start, unsigned int len,
		     unsigned int off)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->csum = csum_block_add(skb->csum,
					   csum_partial(start, len, 0), off);
}


static inline void skb_postpush_rcsum(struct sk_buff *skb,
				      const void *start, unsigned int len)
{
	__skb_postpush_rcsum(skb, start, len, 0);
}

unsigned char *skb_pull_rcsum(struct sk_buff *skb, unsigned int len);


static inline unsigned char *skb_push_rcsum(struct sk_buff *skb,
					    unsigned int len)
{
	skb_push(skb, len);
	skb_postpush_rcsum(skb, skb->data, len);
	return skb->data;
}

int pskb_trim_rcsum_slow(struct sk_buff *skb, unsigned int len);


static inline int pskb_trim_rcsum(struct sk_buff *skb, unsigned int len)
{
	if (likely(len >= skb->len))
		return 0;
	return pskb_trim_rcsum_slow(skb, len);
}

static inline int __skb_trim_rcsum(struct sk_buff *skb, unsigned int len)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->ip_summed = CHECKSUM_NONE;
	__skb_trim(skb, len);
	return 0;
}

static inline int __skb_grow_rcsum(struct sk_buff *skb, unsigned int len)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->ip_summed = CHECKSUM_NONE;
	return __skb_grow(skb, len);
}

#define rb_to_skb(rb) rb_entry_safe(rb, struct sk_buff, rbnode)

#define rb_to_skb(rb) rb_entry_safe(rb, struct sk_buff, rbnode)
#define skb_rb_first(root) rb_to_skb(rb_first(root))
#define skb_rb_last(root)  rb_to_skb(rb_last(root))
#define skb_rb_next(skb)   rb_to_skb(rb_next(&(skb)->rbnode))
#define skb_rb_prev(skb)   rb_to_skb(rb_prev(&(skb)->rbnode))

#define skb_queue_walk(queue, skb) \
		for (skb = (queue)->next;					\
		     skb != (struct sk_buff *)(queue);				\
		     skb = skb->next)

#define skb_queue_walk_safe(queue, skb, tmp)					\
		for (skb = (queue)->next, tmp = skb->next;			\
		     skb != (struct sk_buff *)(queue);				\
		     skb = tmp, tmp = skb->next)

#define skb_queue_walk_from(queue, skb)						\
		for (; skb != (struct sk_buff *)(queue);			\
		     skb = skb->next)

#define skb_rbtree_walk(skb, root)						\
		for (skb = skb_rb_first(root); skb != NULL;			\
		     skb = skb_rb_next(skb))

#define skb_rbtree_walk_from(skb)						\
		for (; skb != NULL;						\
		     skb = skb_rb_next(skb))

#define skb_rbtree_walk_from_safe(skb, tmp)					\
		for (; tmp = skb ? skb_rb_next(skb) : NULL, (skb != NULL);	\
		     skb = tmp)

#define skb_queue_walk_from_safe(queue, skb, tmp)				\
		for (tmp = skb->next;						\
		     skb != (struct sk_buff *)(queue);				\
		     skb = tmp, tmp = skb->next)

#define skb_queue_reverse_walk(queue, skb) \
		for (skb = (queue)->prev;					\
		     skb != (struct sk_buff *)(queue);				\
		     skb = skb->prev)

#define skb_queue_reverse_walk_safe(queue, skb, tmp)				\
		for (skb = (queue)->prev, tmp = skb->prev;			\
		     skb != (struct sk_buff *)(queue);				\
		     skb = tmp, tmp = skb->prev)

#define skb_queue_reverse_walk_from_safe(queue, skb, tmp)			\
		for (tmp = skb->prev;						\
		     skb != (struct sk_buff *)(queue);				\
		     skb = tmp, tmp = skb->prev)

static inline bool skb_has_frag_list(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->frag_list != NULL;
}

static inline void skb_frag_list_init(struct sk_buff *skb)
{
	skb_shinfo(skb)->frag_list = NULL;
}

#define skb_walk_frags(skb, iter)	\
	for (iter = skb_shinfo(skb)->frag_list; iter; iter = iter->next)


int __skb_wait_for_more_packets(struct sock *sk, int *err, long *timeo_p,
				const struct sk_buff *skb);
struct sk_buff *__skb_try_recv_datagram(struct sock *sk, unsigned flags,
					int *peeked, int *off, int *err,
					struct sk_buff **last);
struct sk_buff *__skb_recv_datagram(struct sock *sk, unsigned flags,
				    int *peeked, int *off, int *err);
struct sk_buff *skb_recv_datagram(struct sock *sk, unsigned flags, int noblock,
				  int *err);
unsigned int datagram_poll(struct file *file, struct socket *sock,
			   struct poll_table_struct *wait);
int skb_copy_datagram_iter(const struct sk_buff *from, int offset,
			   struct iov_iter *to, int size);
static inline int skb_copy_datagram_msg(const struct sk_buff *from, int offset,
					struct msghdr *msg, int size)
{
	return skb_copy_datagram_iter(from, offset, &msg->msg_iter, size);
}
int skb_copy_and_csum_datagram_msg(struct sk_buff *skb, int hlen,
				   struct msghdr *msg);
int skb_copy_datagram_from_iter(struct sk_buff *skb, int offset,
				 struct iov_iter *from, int len);
int zerocopy_sg_from_iter(struct sk_buff *skb, struct iov_iter *frm);
void skb_free_datagram(struct sock *sk, struct sk_buff *skb);
void __skb_free_datagram_locked(struct sock *sk, struct sk_buff *skb, int len);
static inline void skb_free_datagram_locked(struct sock *sk,
					    struct sk_buff *skb)
{
	__skb_free_datagram_locked(sk, skb, 0);
}
int skb_kill_datagram(struct sock *sk, struct sk_buff *skb, unsigned int flags);
int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len);
int skb_store_bits(struct sk_buff *skb, int offset, const void *from, int len);
__wsum skb_copy_and_csum_bits(const struct sk_buff *skb, int offset, u8 *to,
			      int len, __wsum csum);
int skb_splice_bits(struct sk_buff *skb, struct sock *sk, unsigned int offset,
		    struct pipe_inode_info *pipe, unsigned int len,
		    unsigned int flags);
int skb_send_sock_locked(struct sock *sk, struct sk_buff *skb, int offset,
			 int len);
int skb_send_sock(struct sock *sk, struct sk_buff *skb, int offset, int len);
void skb_copy_and_csum_dev(const struct sk_buff *skb, u8 *to);
unsigned int skb_zerocopy_headlen(const struct sk_buff *from);
int skb_zerocopy(struct sk_buff *to, struct sk_buff *from,
		 int len, int hlen);
void skb_split(struct sk_buff *skb, struct sk_buff *skb1, const u32 len);
int skb_shift(struct sk_buff *tgt, struct sk_buff *skb, int shiftlen);
void skb_scrub_packet(struct sk_buff *skb, bool xnet);
unsigned int skb_gso_transport_seglen(const struct sk_buff *skb);
bool skb_gso_validate_mtu(const struct sk_buff *skb, unsigned int mtu);
bool skb_gso_validate_mac_len(const struct sk_buff *skb, unsigned int len);
struct sk_buff *skb_segment(struct sk_buff *skb, netdev_features_t features);
struct sk_buff *skb_vlan_untag(struct sk_buff *skb);
int skb_ensure_writable(struct sk_buff *skb, int write_len);
int __skb_vlan_pop(struct sk_buff *skb, u16 *vlan_tci);
int skb_vlan_pop(struct sk_buff *skb);
int skb_vlan_push(struct sk_buff *skb, __be16 vlan_proto, u16 vlan_tci);
struct sk_buff *pskb_extract(struct sk_buff *skb, int off, int to_copy,
			     gfp_t gfp);

static inline int memcpy_from_msg(void *data, struct msghdr *msg, int len)
{
	return copy_from_iter(data, len, &msg->msg_iter) == len ? 0 : -EFAULT;
}

static inline int memcpy_to_msg(struct msghdr *msg, void *data, int len)
{
	return copy_to_iter(data, len, &msg->msg_iter) == len ? 0 : -EFAULT;
}

struct skb_checksum_ops {
	__wsum (*update)(const void *mem, int len, __wsum wsum);
	__wsum (*combine)(__wsum csum, __wsum csum2, int offset, int len);
};

__wsum __skb_checksum(const struct sk_buff *skb, int offset, int len,
		      __wsum csum, const struct skb_checksum_ops *ops);
__wsum skb_checksum(const struct sk_buff *skb, int offset, int len,
		    __wsum csum);

static inline void * __must_check
__skb_header_pointer(const struct sk_buff *skb, int offset,
		     int len, void *data, int hlen, void *buffer)
{
	if (hlen - offset >= len)
		return data + offset;

	if (!skb ||
	    skb_copy_bits(skb, offset, buffer, len) < 0)
		return NULL;

	return buffer;
}

static inline void * __must_check
skb_header_pointer(const struct sk_buff *skb, int offset, int len, void *buffer)
{
	return __skb_header_pointer(skb, offset, len, skb->data,
				    skb_headlen(skb), buffer);
}


static inline bool skb_needs_linearize(struct sk_buff *skb,
				       netdev_features_t features)
{
	return skb_is_nonlinear(skb) &&
	       ((skb_has_frag_list(skb) && !(features & NETIF_F_FRAGLIST)) ||
		(skb_shinfo(skb)->nr_frags && !(features & NETIF_F_SG)));
}

static inline void skb_copy_from_linear_data(const struct sk_buff *skb,
					     void *to,
					     const unsigned int len)
{
	memcpy(to, skb->data, len);
}

static inline void skb_copy_from_linear_data_offset(const struct sk_buff *skb,
						    const int offset, void *to,
						    const unsigned int len)
{
	memcpy(to, skb->data + offset, len);
}

static inline void skb_copy_to_linear_data(struct sk_buff *skb,
					   const void *from,
					   const unsigned int len)
{
	memcpy(skb->data, from, len);
}

static inline void skb_copy_to_linear_data_offset(struct sk_buff *skb,
						  const int offset,
						  const void *from,
						  const unsigned int len)
{
	memcpy(skb->data + offset, from, len);
}

void skb_init(void);

static inline ktime_t skb_get_ktime(const struct sk_buff *skb)
{
	return skb->tstamp;
}


static inline void skb_get_timestamp(const struct sk_buff *skb,
				     struct timeval *stamp)
{
	*stamp = ktime_to_timeval(skb->tstamp);
}

static inline void skb_get_timestampns(const struct sk_buff *skb,
				       struct timespec *stamp)
{
	*stamp = ktime_to_timespec(skb->tstamp);
}

static inline void __net_timestamp(struct sk_buff *skb)
{
	skb->tstamp = ktime_get_real();
}

static inline ktime_t net_timedelta(ktime_t t)
{
	return ktime_sub(ktime_get_real(), t);
}

static inline ktime_t net_invalid_timestamp(void)
{
	return ktime_set(0, 0);
}

static inline u8 skb_metadata_len(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->meta_len;
}

static inline void *skb_metadata_end(const struct sk_buff *skb)
{
	return skb_mac_header(skb);
}

static inline bool __skb_metadata_differs(const struct sk_buff *skb_a,
					  const struct sk_buff *skb_b,
					  u8 meta_len)
{
	const void *a = skb_metadata_end(skb_a);
	const void *b = skb_metadata_end(skb_b);
	
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
	u64 diffs = 0;

	switch (meta_len) {
#define __it(x, op) (x -= sizeof(u##op))
#define __it_diff(a, b, op) (*(u##op *)__it(a, op)) ^ (*(u##op *)__it(b, op))
	case 32: diffs |= __it_diff(a, b, 64);
	case 24: diffs |= __it_diff(a, b, 64);
	case 16: diffs |= __it_diff(a, b, 64);
	case  8: diffs |= __it_diff(a, b, 64);
		break;
	case 28: diffs |= __it_diff(a, b, 64);
	case 20: diffs |= __it_diff(a, b, 64);
	case 12: diffs |= __it_diff(a, b, 64);
	case  4: diffs |= __it_diff(a, b, 32);
		break;
	}
	return diffs;
#else
	return memcmp(a - meta_len, b - meta_len, meta_len);
#endif
}

static inline bool skb_metadata_differs(const struct sk_buff *skb_a,
					const struct sk_buff *skb_b)
{
	u8 len_a = skb_metadata_len(skb_a);
	u8 len_b = skb_metadata_len(skb_b);

	if (!(len_a | len_b))
		return false;

	return len_a != len_b ?
	       true : __skb_metadata_differs(skb_a, skb_b, len_a);
}

static inline void skb_metadata_set(struct sk_buff *skb, u8 meta_len)
{
	skb_shinfo(skb)->meta_len = meta_len;
}

static inline void skb_metadata_clear(struct sk_buff *skb)
{
	skb_metadata_set(skb, 0);
}

struct sk_buff *skb_clone_sk(struct sk_buff *skb);

#ifdef CONFIG_NETWORK_PHY_TIMESTAMPING

void skb_clone_tx_timestamp(struct sk_buff *skb);
bool skb_defer_rx_timestamp(struct sk_buff *skb);

#else 

static inline void skb_clone_tx_timestamp(struct sk_buff *skb)
{
}

static inline bool skb_defer_rx_timestamp(struct sk_buff *skb)
{
	return false;
}

#endif 


void skb_complete_tx_timestamp(struct sk_buff *skb,
			       struct skb_shared_hwtstamps *hwtstamps);

void __skb_tstamp_tx(struct sk_buff *orig_skb,
		     struct skb_shared_hwtstamps *hwtstamps,
		     struct sock *sk, int tstype);


void skb_tstamp_tx(struct sk_buff *orig_skb,
		   struct skb_shared_hwtstamps *hwtstamps);

static inline void sw_tx_timestamp(struct sk_buff *skb)
{
	if (skb_shinfo(skb)->tx_flags & SKBTX_SW_TSTAMP &&
	    !(skb_shinfo(skb)->tx_flags & SKBTX_IN_PROGRESS))
		skb_tstamp_tx(skb, NULL);
}


static inline void skb_tx_timestamp(struct sk_buff *skb)
{
	skb_clone_tx_timestamp(skb);
	sw_tx_timestamp(skb);
}


void skb_complete_wifi_ack(struct sk_buff *skb, bool acked);

__sum16 __skb_checksum_complete_head(struct sk_buff *skb, int len);
__sum16 __skb_checksum_complete(struct sk_buff *skb);

static inline int skb_csum_unnecessary(const struct sk_buff *skb)
{
	return ((skb->ip_summed == CHECKSUM_UNNECESSARY) ||
		skb->csum_valid ||
		(skb->ip_summed == CHECKSUM_PARTIAL &&
		 skb_checksum_start_offset(skb) >= 0));
}


static inline __sum16 skb_checksum_complete(struct sk_buff *skb)
{
	return skb_csum_unnecessary(skb) ?
	       0 : __skb_checksum_complete(skb);
}

static inline void __skb_decr_checksum_unnecessary(struct sk_buff *skb)
{
	if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
		if (skb->csum_level == 0)
			skb->ip_summed = CHECKSUM_NONE;
		else
			skb->csum_level--;
	}
}

static inline void __skb_incr_checksum_unnecessary(struct sk_buff *skb)
{
	if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
		if (skb->csum_level < SKB_MAX_CSUM_LEVEL)
			skb->csum_level++;
	} else if (skb->ip_summed == CHECKSUM_NONE) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		skb->csum_level = 0;
	}
}

static inline void __skb_mark_checksum_bad(struct sk_buff *skb)
{
	

	if (skb->ip_summed == CHECKSUM_NONE ||
	    skb->ip_summed == CHECKSUM_UNNECESSARY)
		skb->csum_bad = 1;
}


static inline bool __skb_checksum_validate_needed(struct sk_buff *skb,
						  bool zero_okay,
						  __sum16 check)
{
	if (skb_csum_unnecessary(skb) || (zero_okay && !check)) {
		skb->csum_valid = 1;
		__skb_decr_checksum_unnecessary(skb);
		return false;
	}

	return true;
}


#define CHECKSUM_BREAK 76


static inline void skb_checksum_complete_unset(struct sk_buff *skb)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->ip_summed = CHECKSUM_NONE;
}


static inline __sum16 __skb_checksum_validate_complete(struct sk_buff *skb,
						       bool complete,
						       __wsum psum)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE) {
		if (!csum_fold(csum_add(psum, skb->csum))) {
			skb->csum_valid = 1;
			return 0;
		}
	} else if (skb->csum_bad) {
		
		return (__force __sum16)1;
	}

	skb->csum = psum;

	if (complete || skb->len <= CHECKSUM_BREAK) {
		__sum16 csum;

		csum = __skb_checksum_complete(skb);
		skb->csum_valid = !csum;
		return csum;
	}

	return 0;
}

static inline __wsum null_compute_pseudo(struct sk_buff *skb, int proto)
{
	return 0;
}


#define __skb_checksum_validate(skb, proto, complete,			\
				zero_okay, check, compute_pseudo)	\
({									\
	__sum16 __ret = 0;						\
	skb->csum_valid = 0;						\
	if (__skb_checksum_validate_needed(skb, zero_okay, check))	\
		__ret = __skb_checksum_validate_complete(skb,		\
				complete, compute_pseudo(skb, proto));	\
	__ret;								\
})

#define skb_checksum_init(skb, proto, compute_pseudo)			\
	__skb_checksum_validate(skb, proto, false, false, 0, compute_pseudo)

#define skb_checksum_init_zero_check(skb, proto, check, compute_pseudo)	\
	__skb_checksum_validate(skb, proto, false, true, check, compute_pseudo)

#define skb_checksum_validate(skb, proto, compute_pseudo)		\
	__skb_checksum_validate(skb, proto, true, false, 0, compute_pseudo)

#define skb_checksum_validate_zero_check(skb, proto, check,		\
					 compute_pseudo)		\
	__skb_checksum_validate(skb, proto, true, true, check, compute_pseudo)

#define skb_checksum_simple_validate(skb)				\
	__skb_checksum_validate(skb, 0, true, false, 0, null_compute_pseudo)

static inline bool __skb_checksum_convert_check(struct sk_buff *skb)
{
	return (skb->ip_summed == CHECKSUM_NONE &&
		skb->csum_valid && !skb->csum_bad);
}

static inline void __skb_checksum_convert(struct sk_buff *skb,
					  __sum16 check, __wsum pseudo)
{
	skb->csum = ~pseudo;
	skb->ip_summed = CHECKSUM_COMPLETE;
}

#define skb_checksum_try_convert(skb, proto, check, compute_pseudo)	\
do {									\
	if (__skb_checksum_convert_check(skb))				\
		__skb_checksum_convert(skb, check,			\
				       compute_pseudo(skb, proto));	\
} while (0)

static inline void skb_remcsum_adjust_partial(struct sk_buff *skb, void *ptr,
					      u16 start, u16 offset)
{
	skb->ip_summed = CHECKSUM_PARTIAL;
	skb->csum_start = ((unsigned char *)ptr + start) - skb->head;
	skb->csum_offset = offset - start;
}


static inline void skb_remcsum_process(struct sk_buff *skb, void *ptr,
				       int start, int offset, bool nopartial)
{
	__wsum delta;

	if (!nopartial) {
		skb_remcsum_adjust_partial(skb, ptr, start, offset);
		return;
	}

	 if (unlikely(skb->ip_summed != CHECKSUM_COMPLETE)) {
		__skb_checksum_complete(skb);
		skb_postpull_rcsum(skb, skb->data, ptr - (void *)skb->data);
	}

	delta = remcsum_adjust(ptr, skb->csum, start, offset);

	
	skb->csum = csum_add(skb->csum, delta);
}

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
void nf_conntrack_destroy(struct nf_conntrack *nfct);
static inline void nf_conntrack_put(struct nf_conntrack *nfct)
{
	if (nfct && atomic_dec_and_test(&nfct->use))
		nf_conntrack_destroy(nfct);
}
static inline void nf_conntrack_get(struct nf_conntrack *nfct)
{
	if (nfct)
		atomic_inc(&nfct->use);
}
#endif
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
static inline void nf_bridge_put(struct nf_bridge_info *nf_bridge)
{
	if (nf_bridge && atomic_dec_and_test(&nf_bridge->use))
		kfree(nf_bridge);
}
static inline void nf_bridge_get(struct nf_bridge_info *nf_bridge)
{
	if (nf_bridge)
		atomic_inc(&nf_bridge->use);
}
#endif 
static inline void nf_reset(struct sk_buff *skb)
{
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	nf_conntrack_put(skb->nfct);
	skb->nfct = NULL;
#endif
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	nf_bridge_put(skb->nf_bridge);
	skb->nf_bridge = NULL;
#endif
}

static inline void nf_reset_trace(struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_NETFILTER_XT_TARGET_TRACE) || defined(CONFIG_NF_TABLES)
	skb->nf_trace = 0;
#endif
}

static inline void ipvs_reset(struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_IP_VS)
	skb->ipvs_property = 0;
#endif
}


static inline void __nf_copy(struct sk_buff *dst, const struct sk_buff *src,
			     bool copy)
{
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	dst->nfct = src->nfct;
	nf_conntrack_get(src->nfct);
	if (copy)
		dst->nfctinfo = src->nfctinfo;
#endif
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	dst->nf_bridge  = src->nf_bridge;
	nf_bridge_get(src->nf_bridge);
#endif
#if IS_ENABLED(CONFIG_NETFILTER_XT_TARGET_TRACE) || defined(CONFIG_NF_TABLES)
	if (copy)
		dst->nf_trace = src->nf_trace;
#endif
}

static inline void nf_copy(struct sk_buff *dst, const struct sk_buff *src)
{
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	nf_conntrack_put(dst->nfct);
#endif
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	nf_bridge_put(dst->nf_bridge);
#endif
	__nf_copy(dst, src, true);
}

#ifdef CONFIG_NETWORK_SECMARK
static inline void skb_copy_secmark(struct sk_buff *to, const struct sk_buff *from)
{
	to->secmark = from->secmark;
}

static inline void skb_init_secmark(struct sk_buff *skb)
{
	skb->secmark = 0;
}
#else
static inline void skb_copy_secmark(struct sk_buff *to, const struct sk_buff *from)
{ }

static inline void skb_init_secmark(struct sk_buff *skb)
{ }
#endif

static inline bool skb_irq_freeable(const struct sk_buff *skb)
{
	return !skb->destructor &&
#if IS_ENABLED(CONFIG_XFRM)
		!skb->sp &&
#endif
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
		!skb->nfct &&
#endif
		!skb->_skb_refdst &&
		!skb_has_frag_list(skb);
}

static inline void skb_set_queue_mapping(struct sk_buff *skb, u16 queue_mapping)
{
	skb->queue_mapping = queue_mapping;
}

static inline u16 skb_get_queue_mapping(const struct sk_buff *skb)
{
	return skb->queue_mapping;
}

static inline void skb_copy_queue_mapping(struct sk_buff *to, const struct sk_buff *from)
{
	to->queue_mapping = from->queue_mapping;
}

static inline void skb_record_rx_queue(struct sk_buff *skb, u16 rx_queue)
{
	skb->queue_mapping = rx_queue + 1;
}

static inline u16 skb_get_rx_queue(const struct sk_buff *skb)
{
	return skb->queue_mapping - 1;
}

static inline bool skb_rx_queue_recorded(const struct sk_buff *skb)
{
	return skb->queue_mapping != 0;
}

static inline struct sec_path *skb_sec_path(struct sk_buff *skb)
{
#ifdef CONFIG_XFRM
	return skb->sp;
#else
	return NULL;
#endif
}


struct skb_gso_cb {
	union {
		int	mac_offset;
		int	data_offset;
	};
	int	encap_level;
	__wsum	csum;
	__u16	csum_start;
};
#define SKB_SGO_CB_OFFSET	32
#define SKB_GSO_CB(skb) ((struct skb_gso_cb *)((skb)->cb + SKB_SGO_CB_OFFSET))

static inline int skb_tnl_header_len(const struct sk_buff *inner_skb)
{
	return (skb_mac_header(inner_skb) - inner_skb->head) -
		SKB_GSO_CB(inner_skb)->mac_offset;
}

static inline int gso_pskb_expand_head(struct sk_buff *skb, int extra)
{
	int new_headroom, headroom;
	int ret;

	headroom = skb_headroom(skb);
	ret = pskb_expand_head(skb, extra, 0, GFP_ATOMIC);
	if (ret)
		return ret;

	new_headroom = skb_headroom(skb);
	SKB_GSO_CB(skb)->mac_offset += (new_headroom - headroom);
	return 0;
}

static inline void gso_reset_checksum(struct sk_buff *skb, __wsum res)
{
	
	if (skb->remcsum_offload)
		return;

	SKB_GSO_CB(skb)->csum = res;
	SKB_GSO_CB(skb)->csum_start = skb_checksum_start(skb) - skb->head;
}


static inline __sum16 gso_make_checksum(struct sk_buff *skb, __wsum res)
{
	unsigned char *csum_start = skb_transport_header(skb);
	int plen = (skb->head + SKB_GSO_CB(skb)->csum_start) - csum_start;
	__wsum partial = SKB_GSO_CB(skb)->csum;

	SKB_GSO_CB(skb)->csum = res;
	SKB_GSO_CB(skb)->csum_start = csum_start - skb->head;

	return csum_fold(csum_partial(csum_start, plen, partial));
}

static inline bool skb_is_gso(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->gso_size;
}


static inline bool skb_is_gso_v6(const struct sk_buff *skb)
{
	return skb_shinfo(skb)->gso_type & SKB_GSO_TCPV6;
}

static inline void skb_gso_reset(struct sk_buff *skb)
{
	skb_shinfo(skb)->gso_size = 0;
	skb_shinfo(skb)->gso_segs = 0;
	skb_shinfo(skb)->gso_type = 0;
}

void __skb_warn_lro_forwarding(const struct sk_buff *skb);

static inline bool skb_warn_if_lro(const struct sk_buff *skb)
{
	
	const struct skb_shared_info *shinfo = skb_shinfo(skb);

	if (skb_is_nonlinear(skb) && shinfo->gso_size != 0 &&
	    unlikely(shinfo->gso_type == 0)) {
		__skb_warn_lro_forwarding(skb);
		return true;
	}
	return false;
}

static inline void skb_forward_csum(struct sk_buff *skb)
{
	
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->ip_summed = CHECKSUM_NONE;
}


static inline void skb_checksum_none_assert(const struct sk_buff *skb)
{
#ifdef DEBUG
	BUG_ON(skb->ip_summed != CHECKSUM_NONE);
#endif
}

bool skb_partial_csum_set(struct sk_buff *skb, u16 start, u16 off);

int skb_checksum_setup(struct sk_buff *skb, bool recalculate);
struct sk_buff *skb_checksum_trimmed(struct sk_buff *skb,
				     unsigned int transport_len,
				     __sum16(*skb_chkf)(struct sk_buff *skb));


static inline bool skb_head_is_locked(const struct sk_buff *skb)
{
	return !skb->head_frag || skb_cloned(skb);
}


static inline unsigned int skb_gso_network_seglen(const struct sk_buff *skb)
{
	unsigned int hdr_len = skb_transport_header(skb) -
			       skb_network_header(skb);
	return hdr_len + skb_gso_transport_seglen(skb);
}


static inline unsigned int skb_gso_mac_seglen(const struct sk_buff *skb)
{
	unsigned int hdr_len = skb_transport_header(skb) - skb_mac_header(skb);
	return hdr_len + skb_gso_transport_seglen(skb);
}


static inline __wsum lco_csum(struct sk_buff *skb)
{
	unsigned char *csum_start = skb_checksum_start(skb);
	unsigned char *l4_hdr = skb_transport_header(skb);
	__wsum partial;

	
	partial = ~csum_unfold(*(__force __sum16 *)(csum_start +
						    skb->csum_offset));

	
	return csum_partial(l4_hdr, csum_start - l4_hdr, partial);
}

#endif	
#endif	
