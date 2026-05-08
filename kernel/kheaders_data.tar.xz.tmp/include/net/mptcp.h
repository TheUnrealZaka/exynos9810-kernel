

#ifndef _MPTCP_H
#define _MPTCP_H

#include <linux/inetdevice.h>
#include <linux/ipv6.h>
#include <linux/list.h>
#include <linux/net.h>
#include <linux/netpoll.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <linux/kernel.h>

#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <crypto/hash.h>
#include <net/tcp.h>

#if defined(__LITTLE_ENDIAN_BITFIELD)
	#define ntohll(x)  be64_to_cpu(x)
	#define htonll(x)  cpu_to_be64(x)
#elif defined(__BIG_ENDIAN_BITFIELD)
	#define ntohll(x) (x)
	#define htonll(x) (x)
#endif

struct mptcp_loc4 {
	u8		loc4_id;
	u8		low_prio:1;
	int		if_idx;
	struct in_addr	addr;
};

struct mptcp_rem4 {
	u8		rem4_id;
	__be16		port;
	struct in_addr	addr;
};

struct mptcp_loc6 {
	u8		loc6_id;
	u8		low_prio:1;
	int		if_idx;
	struct in6_addr	addr;
};

struct mptcp_rem6 {
	u8		rem6_id;
	__be16		port;
	struct in6_addr	addr;
};

struct mptcp_request_sock {
	struct tcp_request_sock		req;
	struct hlist_nulls_node		hash_entry;

	union {
		struct {
			
			u64		mptcp_loc_key;
			u64		mptcp_rem_key;
			u32		mptcp_loc_token;
		};

		struct {
			
			u32		mptcp_rem_nonce;
			u32		mptcp_loc_nonce;
			u64		mptcp_hash_tmac;
		};
	};

	u8				loc_id;
	u8				rem_id; 
	u8				dss_csum:1,
					is_sub:1, 
					low_prio:1, 
					rcv_low_prio:1,
					mptcp_ver:4;
};

struct mptcp_options_received {
	u16	saw_mpc:1,
		dss_csum:1,
		drop_me:1,

		is_mp_join:1,
		join_ack:1,

		saw_low_prio:2, 
		low_prio:1,

		saw_add_addr:2, 
		more_add_addr:1, 

		saw_rem_addr:1, 
		more_rem_addr:1, 

		mp_fail:1,
		mp_fclose:1;
	u8	rem_id;		
	u8	prio_addr_id;	

	const unsigned char *add_addr_ptr; 
	const unsigned char *rem_addr_ptr; 

	u32	data_ack;
	u32	data_seq;
	u16	data_len;

	u8	mptcp_ver; 

	
	u64	mptcp_sender_key;
	u64	mptcp_receiver_key;

	u32	mptcp_rem_token; 

	u32	mptcp_recv_nonce;
	u64	mptcp_recv_tmac;
	u8	mptcp_recv_mac[20];
};

struct mptcp_tcp_sock {
	struct tcp_sock	*next;		
	struct hlist_node cb_list;
	struct mptcp_options_received rx_opt;

	 
	u64	map_data_seq;
	u32	map_subseq;
	u16	map_data_len;
	u16	slave_sk:1,
		fully_established:1,
		establish_increased:1,
		second_packet:1,
		attached:1,
		send_mp_fail:1,
		include_mpc:1,
		mapping_present:1,
		map_data_fin:1,
		low_prio:1, 
		rcv_low_prio:1, 
		send_mp_prio:1, 
		pre_established:1; 

	
	u32	snt_isn;
	u32	rcv_isn;
	u8	path_index;
	u8	loc_id;
	u8	rem_id;

#define MPTCP_SCHED_SIZE 16
	u8	mptcp_sched[MPTCP_SCHED_SIZE] __aligned(8);

	int	init_rcv_wnd;
	u32	infinite_cutoff_seq;
	struct delayed_work work;
	u32	mptcp_loc_nonce;
	struct tcp_sock *tp; 
	u32	last_end_data_seq;

	
	struct timer_list mptcp_ack_timer;

	
	char sender_mac[20];
};

struct mptcp_tw {
	struct list_head list;
	u64 loc_key;
	u64 rcv_nxt;
	struct mptcp_cb __rcu *mpcb;
	u8 meta_tw:1,
	   in_list:1;
};

#define MPTCP_PM_NAME_MAX 16
struct mptcp_pm_ops {
	struct list_head list;

	
	void (*new_session)(const struct sock *meta_sk);
	void (*release_sock)(struct sock *meta_sk);
	void (*fully_established)(struct sock *meta_sk);
	void (*new_remote_address)(struct sock *meta_sk);
	void (*subflow_error)(struct sock *meta_sk, struct sock *sk);
	int  (*get_local_id)(sa_family_t family, union inet_addr *addr,
			     struct net *net, bool *low_prio);
	void (*addr_signal)(struct sock *sk, unsigned int *size,
			    struct tcp_out_options *opts, struct sk_buff *skb);
	void (*add_raddr)(struct mptcp_cb *mpcb, const union inet_addr *addr,
			  sa_family_t family, __be16 port, u8 id);
	void (*rem_raddr)(struct mptcp_cb *mpcb, u8 rem_id);
	void (*init_subsocket_v4)(struct sock *sk, struct in_addr addr);
	void (*init_subsocket_v6)(struct sock *sk, struct in6_addr addr);
	void (*delete_subflow)(struct sock *sk);

	char		name[MPTCP_PM_NAME_MAX];
	struct module	*owner;
};

#define MPTCP_SCHED_NAME_MAX 16
struct mptcp_sched_ops {
	struct list_head list;

	struct sock *		(*get_subflow)(struct sock *meta_sk,
					       struct sk_buff *skb,
					       bool zero_wnd_test);
	struct sk_buff *	(*next_segment)(struct sock *meta_sk,
						int *reinject,
						struct sock **subsk,
						unsigned int *limit);
	void			(*init)(struct sock *sk);
	void			(*release)(struct sock *sk);

	char			name[MPTCP_SCHED_NAME_MAX];
	struct module		*owner;
};

struct mptcp_cb {
	
	struct tcp_sock *connection_list;
	
	struct hlist_head callback_list;

	
	u32 snd_high_order[2];
	u32 rcv_high_order[2];

	u16	send_infinite_mapping:1,
		in_time_wait:1,
		list_rcvd:1, 
		addr_signal:1, 
		dss_csum:1,
		server_side:1,
		infinite_mapping_rcv:1,
		infinite_mapping_snd:1,
		dfin_combined:1,   
		passive_close:1,
		snd_hiseq_index:1, 
		rcv_hiseq_index:1; 

	
	u8 cnt_subflows;
	u8 cnt_established;

#define MPTCP_SCHED_DATA_SIZE 8
	u8 mptcp_sched[MPTCP_SCHED_DATA_SIZE] __aligned(8);
	struct mptcp_sched_ops *sched_ops;

	struct sk_buff_head reinject_queue;
	

	u64	csum_cutoff_seq;
	u64	infinite_rcv_seq;

	
	spinlock_t	 tw_lock;
	unsigned char	 mptw_state;
	u8		 dfin_path_index;

	struct list_head tw_list;

	
	atomic_t	mpcb_refcnt;

	
	struct mutex	mpcb_mutex;

	
	struct sock *meta_sk;

	
	struct sock *master_sk;

	__u64	mptcp_loc_key;
	__u64	mptcp_rem_key;
	__u32	mptcp_loc_token;
	__u32	mptcp_rem_token;

#define MPTCP_PM_SIZE 608
	u8 mptcp_pm[MPTCP_PM_SIZE] __aligned(8);
	struct mptcp_pm_ops *pm_ops;

	u32 path_index_bits;
	
	u8 next_path_index;

	__u8	mptcp_ver;

	
	int orig_sk_rcvbuf;
	int orig_sk_sndbuf;
	u32 orig_window_clamp;

	struct tcp_info	*master_info;
};

#define MPTCP_VERSION_0 0
#define MPTCP_VERSION_1 1

#define MPTCP_SUB_CAPABLE			0
#define MPTCP_SUB_LEN_CAPABLE_SYN		12
#define MPTCP_SUB_LEN_CAPABLE_SYN_ALIGN		12
#define MPTCP_SUB_LEN_CAPABLE_ACK		20
#define MPTCP_SUB_LEN_CAPABLE_ACK_ALIGN		20

#define MPTCP_SUB_JOIN			1
#define MPTCP_SUB_LEN_JOIN_SYN		12
#define MPTCP_SUB_LEN_JOIN_SYN_ALIGN	12
#define MPTCP_SUB_LEN_JOIN_SYNACK	16
#define MPTCP_SUB_LEN_JOIN_SYNACK_ALIGN	16
#define MPTCP_SUB_LEN_JOIN_ACK		24
#define MPTCP_SUB_LEN_JOIN_ACK_ALIGN	24

#define MPTCP_SUB_DSS		2
#define MPTCP_SUB_LEN_DSS	4
#define MPTCP_SUB_LEN_DSS_ALIGN	4


#define MPTCP_SUB_LEN_SEQ	10
#define MPTCP_SUB_LEN_SEQ_CSUM	12
#define MPTCP_SUB_LEN_SEQ_ALIGN	12

#define MPTCP_SUB_LEN_SEQ_64		14
#define MPTCP_SUB_LEN_SEQ_CSUM_64	16
#define MPTCP_SUB_LEN_SEQ_64_ALIGN	16

#define MPTCP_SUB_LEN_ACK	4
#define MPTCP_SUB_LEN_ACK_ALIGN	4

#define MPTCP_SUB_LEN_ACK_64		8
#define MPTCP_SUB_LEN_ACK_64_ALIGN	8


#define MPTCP_SUB_LEN_DSM_ALIGN  (MPTCP_SUB_LEN_DSS_ALIGN +		\
				  MPTCP_SUB_LEN_SEQ_ALIGN +		\
				  MPTCP_SUB_LEN_ACK_ALIGN)

#define MPTCP_SUB_ADD_ADDR		3
#define MPTCP_SUB_LEN_ADD_ADDR4		8
#define MPTCP_SUB_LEN_ADD_ADDR4_VER1	16
#define MPTCP_SUB_LEN_ADD_ADDR6		20
#define MPTCP_SUB_LEN_ADD_ADDR6_VER1	28
#define MPTCP_SUB_LEN_ADD_ADDR4_ALIGN	8
#define MPTCP_SUB_LEN_ADD_ADDR4_ALIGN_VER1	16
#define MPTCP_SUB_LEN_ADD_ADDR6_ALIGN	20
#define MPTCP_SUB_LEN_ADD_ADDR6_ALIGN_VER1	28

#define MPTCP_SUB_REMOVE_ADDR	4
#define MPTCP_SUB_LEN_REMOVE_ADDR	4

#define MPTCP_SUB_PRIO		5
#define MPTCP_SUB_LEN_PRIO	3
#define MPTCP_SUB_LEN_PRIO_ADDR	4
#define MPTCP_SUB_LEN_PRIO_ALIGN	4

#define MPTCP_SUB_FAIL		6
#define MPTCP_SUB_LEN_FAIL	12
#define MPTCP_SUB_LEN_FAIL_ALIGN	12

#define MPTCP_SUB_FCLOSE	7
#define MPTCP_SUB_LEN_FCLOSE	12
#define MPTCP_SUB_LEN_FCLOSE_ALIGN	12


#define OPTION_MPTCP		(1 << 5)


#define MPTCP_FASTCLOSE_RETRIES 3

#ifdef CONFIG_MPTCP


extern bool mptcp_init_failed;


#define OPTION_TYPE_SYN		(1 << 0)
#define OPTION_TYPE_SYNACK	(1 << 1)
#define OPTION_TYPE_ACK		(1 << 2)
#define OPTION_MP_CAPABLE	(1 << 3)
#define OPTION_DATA_ACK		(1 << 4)
#define OPTION_ADD_ADDR		(1 << 5)
#define OPTION_MP_JOIN		(1 << 6)
#define OPTION_MP_FAIL		(1 << 7)
#define OPTION_MP_FCLOSE	(1 << 8)
#define OPTION_REMOVE_ADDR	(1 << 9)
#define OPTION_MP_PRIO		(1 << 10)


#define MPTCPHDR_SEQ		0x01 
#define MPTCPHDR_FIN		0x02 
#define MPTCPHDR_SEQ64_INDEX	0x04 

#define MPTCPHDR_ACK		0x08
#define MPTCPHDR_SEQ64_SET	0x10 
#define MPTCPHDR_SEQ64_OFO	0x20 
#define MPTCPHDR_DSS_CSUM	0x40

#define MPTCPHDR_INF		0x08
#define MPTCP_REINJECT		0x10 

struct mptcp_option {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ver:4,
		sub:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		ver:4;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
};

struct mp_capable {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ver:4,
		sub:4;
	__u8	h:1,
		rsv:5,
		b:1,
		a:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		ver:4;
	__u8	a:1,
		b:1,
		rsv:5,
		h:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__u64	sender_key;
	__u64	receiver_key;
} __attribute__((__packed__));

struct mp_join {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	b:1,
		rsv:3,
		sub:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		rsv:3,
		b:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__u8	addr_id;
	union {
		struct {
			u32	token;
			u32	nonce;
		} syn;
		struct {
			__u64	mac;
			u32	nonce;
		} synack;
		struct {
			__u8	mac[20];
		} ack;
	} u;
} __attribute__((__packed__));

struct mp_dss {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	rsv1:4,
		sub:4,
		A:1,
		a:1,
		M:1,
		m:1,
		F:1,
		rsv2:3;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	sub:4,
		rsv1:4,
		rsv2:3,
		F:1,
		m:1,
		M:1,
		a:1,
		A:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
};

struct mp_add_addr {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ipver:4,
		sub:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		ipver:4;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__u8	addr_id;
	union {
		struct {
			struct in_addr	addr;
			__be16		port;
			__u8		mac[8];
		} v4;
		struct {
			struct in6_addr	addr;
			__be16		port;
			__u8		mac[8];
		} v6;
	} u;
} __attribute__((__packed__));

struct mp_remove_addr {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	rsv:4,
		sub:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		rsv:4;
#else
#error "Adjust your <asm/byteorder.h> defines"
#endif
	
	__u8	addrs_id;
};

struct mp_fail {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	rsv1:4,
		sub:4,
		rsv2:8;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	sub:4,
		rsv1:4,
		rsv2:8;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__be64	data_seq;
} __attribute__((__packed__));

struct mp_fclose {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	rsv1:4,
		sub:4,
		rsv2:8;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	sub:4,
		rsv1:4,
		rsv2:8;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__u64	key;
} __attribute__((__packed__));

struct mp_prio {
	__u8	kind;
	__u8	len;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	b:1,
		rsv:3,
		sub:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	sub:4,
		rsv:3,
		b:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif
	__u8	addr_id;
} __attribute__((__packed__));

static inline int mptcp_sub_len_dss(const struct mp_dss *m, const int csum)
{
	return 4 + m->A * (4 + m->a * 4) + m->M * (10 + m->m * 4 + csum * 2);
}

#define MPTCP_SYSCTL	1

extern int sysctl_mptcp_enabled;
extern int sysctl_mptcp_version;
extern int sysctl_mptcp_checksum;
extern int sysctl_mptcp_debug;
extern int sysctl_mptcp_syn_retries;

extern struct workqueue_struct *mptcp_wq;

#define mptcp_debug(fmt, args...)					\
	do {								\
		if (unlikely(sysctl_mptcp_debug))			\
			pr_err(__FILE__ ": " fmt, ##args);	\
	} while (0)


#define mptcp_for_each_tp(mpcb, tp)					\
	for ((tp) = (mpcb)->connection_list; (tp); (tp) = (tp)->mptcp->next)

#define mptcp_for_each_sk(mpcb, sk)					\
	for ((sk) = (struct sock *)(mpcb)->connection_list;		\
	     sk;							\
	     sk = (struct sock *)tcp_sk(sk)->mptcp->next)

#define mptcp_for_each_sk_safe(__mpcb, __sk, __temp)			\
	for (__sk = (struct sock *)(__mpcb)->connection_list,		\
	     __temp = __sk ? (struct sock *)tcp_sk(__sk)->mptcp->next : NULL; \
	     __sk;							\
	     __sk = __temp,						\
	     __temp = __sk ? (struct sock *)tcp_sk(__sk)->mptcp->next : NULL)


#define mptcp_for_each_bit_set(b, i)					\
	for (i = ffs(b) - 1; i >= 0; i = ffs(b >> (i + 1) << (i + 1)) - 1)

#define mptcp_for_each_bit_unset(b, i)					\
	mptcp_for_each_bit_set(~b, i)

#define MPTCP_INC_STATS(net, field)	SNMP_INC_STATS((net)->mptcp.mptcp_statistics, field)
#define MPTCP_INC_STATS_BH(net, field)	__SNMP_INC_STATS((net)->mptcp.mptcp_statistics, field)

enum {
	MPTCP_MIB_NUM = 0,
	MPTCP_MIB_MPCAPABLEPASSIVE,	
	MPTCP_MIB_MPCAPABLEACTIVE,	
	MPTCP_MIB_MPCAPABLEACTIVEACK,	
	MPTCP_MIB_MPCAPABLEPASSIVEACK,	
	MPTCP_MIB_MPCAPABLEPASSIVEFALLBACK,
	MPTCP_MIB_MPCAPABLEACTIVEFALLBACK, 
	MPTCP_MIB_MPCAPABLERETRANSFALLBACK,
	MPTCP_MIB_CSUMENABLED,		
	MPTCP_MIB_RETRANSSEGS,		
	MPTCP_MIB_MPFAILRX,		
	MPTCP_MIB_CSUMFAIL,		
	MPTCP_MIB_FASTCLOSERX,		
	MPTCP_MIB_FASTCLOSETX,		
	MPTCP_MIB_FBACKSUB,		
	MPTCP_MIB_FBACKINIT,		
	MPTCP_MIB_FBDATASUB,		
	MPTCP_MIB_FBDATAINIT,		
	MPTCP_MIB_REMADDRSUB,		
	MPTCP_MIB_JOINNOTOKEN,		
	MPTCP_MIB_JOINFALLBACK,		
	MPTCP_MIB_JOINSYNTX,		
	MPTCP_MIB_JOINSYNRX,		
	MPTCP_MIB_JOINSYNACKRX,		
	MPTCP_MIB_JOINSYNACKMAC,	
	MPTCP_MIB_JOINACKRX,		
	MPTCP_MIB_JOINACKMAC,		
	MPTCP_MIB_JOINACKFAIL,		
	MPTCP_MIB_JOINACKRTO,		
	MPTCP_MIB_JOINACKRXMIT,		
	MPTCP_MIB_NODSSWINDOW,		
	MPTCP_MIB_DSSNOMATCH,		
	MPTCP_MIB_INFINITEMAPRX,	
	MPTCP_MIB_DSSTCPMISMATCH,	
	MPTCP_MIB_DSSTRIMHEAD,		
	MPTCP_MIB_DSSSPLITTAIL,		
	MPTCP_MIB_PURGEOLD,		
	MPTCP_MIB_ADDADDRRX,		
	MPTCP_MIB_ADDADDRTX,		
	MPTCP_MIB_REMADDRRX,		
	MPTCP_MIB_REMADDRTX,		
	__MPTCP_MIB_MAX
};

#define MPTCP_MIB_MAX __MPTCP_MIB_MAX
struct mptcp_mib {
	unsigned long	mibs[MPTCP_MIB_MAX];
};

extern struct lock_class_key meta_key;
extern char *meta_key_name;
extern struct lock_class_key meta_slock_key;
extern char *meta_slock_key_name;

extern u32 mptcp_secret[MD5_MESSAGE_BYTES / 4];


extern u32 mptcp_seed;

#define MPTCP_HASH_SIZE                1024

extern struct hlist_nulls_head tk_hashtable[MPTCP_HASH_SIZE];


#define MPTCP_REQSK_NULLS_BASE (1U << 29)


void mptcp_data_ready(struct sock *sk);
void mptcp_write_space(struct sock *sk);

void mptcp_add_meta_ofo_queue(const struct sock *meta_sk, struct sk_buff *skb,
			      struct sock *sk);
void mptcp_cleanup_rbuf(struct sock *meta_sk, int copied);
int mptcp_add_sock(struct sock *meta_sk, struct sock *sk, u8 loc_id, u8 rem_id,
		   gfp_t flags);
void mptcp_del_sock(struct sock *sk);
void mptcp_update_metasocket(const struct sock *meta_sk);
void mptcp_reinject_data(struct sock *orig_sk, int clone_it);
void mptcp_update_sndbuf(const struct tcp_sock *tp);
void mptcp_send_fin(struct sock *meta_sk);
void mptcp_send_active_reset(struct sock *meta_sk, gfp_t priority);
bool mptcp_write_xmit(struct sock *sk, unsigned int mss_now, int nonagle,
		      int push_one, gfp_t gfp);
void tcp_parse_mptcp_options(const struct sk_buff *skb,
			     struct mptcp_options_received *mopt);
void mptcp_parse_options(const uint8_t *ptr, int opsize,
			 struct mptcp_options_received *mopt,
			 const struct sk_buff *skb,
			 struct tcp_sock *tp);
void mptcp_syn_options(const struct sock *sk, struct tcp_out_options *opts,
		       unsigned int *remaining);
void mptcp_synack_options(struct request_sock *req,
			  struct tcp_out_options *opts,
			  unsigned int *remaining);
void mptcp_established_options(struct sock *sk, struct sk_buff *skb,
			       struct tcp_out_options *opts, unsigned int *size);
void mptcp_options_write(__be32 *ptr, struct tcp_sock *tp,
			 const struct tcp_out_options *opts,
			 struct sk_buff *skb);
void mptcp_close(struct sock *meta_sk, long timeout);
int mptcp_doit(struct sock *sk);
int mptcp_create_master_sk(struct sock *meta_sk, __u64 remote_key,
			   __u8 mptcp_ver, u32 window);
int mptcp_check_req_fastopen(struct sock *child, struct request_sock *req);
int mptcp_check_req_master(struct sock *sk, struct sock *child,
			   struct request_sock *req, const struct sk_buff *skb,
			   int drop);
struct sock *mptcp_check_req_child(struct sock *meta_sk,
				   struct sock *child,
				   struct request_sock *req,
				   struct sk_buff *skb,
				   const struct mptcp_options_received *mopt);
u32 __mptcp_select_window(struct sock *sk);
void mptcp_select_initial_window(int __space, __u32 mss, __u32 *rcv_wnd,
					__u32 *window_clamp, int wscale_ok,
					__u8 *rcv_wscale, __u32 init_rcv_wnd,
					const struct sock *sk);
unsigned int mptcp_current_mss(struct sock *meta_sk);
int mptcp_select_size(const struct sock *meta_sk, bool sg, bool first_skb);
void mptcp_key_sha1(u64 key, u32 *token, u64 *idsn);
void mptcp_hmac_sha1(const u8 *key_1, const u8 *key_2, u32 *hash_out,
		     int arg_num, ...);
void mptcp_clean_rtx_infinite(const struct sk_buff *skb, struct sock *sk);
void mptcp_fin(struct sock *meta_sk);
void mptcp_meta_retransmit_timer(struct sock *meta_sk);
void mptcp_sub_retransmit_timer(struct sock *sk);
int mptcp_write_wakeup(struct sock *meta_sk, int mib);
void mptcp_sub_close_wq(struct work_struct *work);
void mptcp_sub_close(struct sock *sk, unsigned long delay);
struct sock *mptcp_select_ack_sock(const struct sock *meta_sk);
void mptcp_fallback_meta_sk(struct sock *meta_sk);
int mptcp_backlog_rcv(struct sock *meta_sk, struct sk_buff *skb);
void mptcp_ack_handler(unsigned long data);
bool mptcp_check_rtt(const struct tcp_sock *tp, int time);
int mptcp_check_snd_buf(const struct tcp_sock *tp);
bool mptcp_handle_options(struct sock *sk, const struct tcphdr *th,
			  const struct sk_buff *skb);
void __init mptcp_init(void);
void mptcp_destroy_sock(struct sock *sk);
int mptcp_rcv_synsent_state_process(struct sock *sk, struct sock **skptr,
				    const struct sk_buff *skb,
				    const struct mptcp_options_received *mopt);
unsigned int mptcp_xmit_size_goal(const struct sock *meta_sk, u32 mss_now,
				  int large_allowed);
int mptcp_init_tw_sock(struct sock *sk, struct tcp_timewait_sock *tw);
void mptcp_twsk_destructor(struct tcp_timewait_sock *tw);
void mptcp_time_wait(struct sock *sk, int state, int timeo);
void mptcp_disconnect(struct sock *sk);
bool mptcp_should_expand_sndbuf(const struct sock *sk);
int mptcp_retransmit_skb(struct sock *meta_sk, struct sk_buff *skb);
void mptcp_tsq_flags(struct sock *sk);
void mptcp_tsq_sub_deferred(struct sock *meta_sk);
struct mp_join *mptcp_find_join(const struct sk_buff *skb);
void mptcp_hash_remove_bh(struct tcp_sock *meta_tp);
struct sock *mptcp_hash_find(const struct net *net, const u32 token);
int mptcp_lookup_join(struct sk_buff *skb, struct inet_timewait_sock *tw);
int mptcp_do_join_short(struct sk_buff *skb,
			const struct mptcp_options_received *mopt,
			struct net *net);
void mptcp_reqsk_destructor(struct request_sock *req);
void mptcp_connect_init(struct sock *sk);
void mptcp_sub_force_close(struct sock *sk);
int mptcp_sub_len_remove_addr_align(u16 bitfield);
void mptcp_init_buffer_space(struct sock *sk);
void mptcp_join_reqsk_init(const struct mptcp_cb *mpcb,
			   const struct request_sock *req,
			   struct sk_buff *skb);
void mptcp_reqsk_init(struct request_sock *req, const struct sock *sk,
		      const struct sk_buff *skb, bool want_cookie);
int mptcp_conn_request(struct sock *sk, struct sk_buff *skb);
void mptcp_enable_sock(struct sock *sk);
void mptcp_disable_sock(struct sock *sk);
void mptcp_enable_static_key(void);
void mptcp_disable_static_key(void);
void mptcp_cookies_reqsk_init(struct request_sock *req,
			      struct mptcp_options_received *mopt,
			      struct sk_buff *skb);
void mptcp_sock_destruct(struct sock *sk);
int mptcp_finish_handshake(struct sock *child, struct sk_buff *skb);
int mptcp_get_info(const struct sock *meta_sk, char __user *optval, int optlen);
void mptcp_clear_sk(struct sock *sk, int size);


int mptcp_register_path_manager(struct mptcp_pm_ops *pm);
void mptcp_unregister_path_manager(struct mptcp_pm_ops *pm);
void mptcp_init_path_manager(struct mptcp_cb *mpcb);
void mptcp_cleanup_path_manager(struct mptcp_cb *mpcb);
void mptcp_fallback_default(struct mptcp_cb *mpcb);
void mptcp_get_default_path_manager(char *name);
int mptcp_set_scheduler(struct sock *sk, const char *name);
int mptcp_set_path_manager(struct sock *sk, const char *name);
int mptcp_set_default_path_manager(const char *name);
extern struct mptcp_pm_ops mptcp_pm_default;


int mptcp_register_scheduler(struct mptcp_sched_ops *sched);
void mptcp_unregister_scheduler(struct mptcp_sched_ops *sched);
void mptcp_init_scheduler(struct mptcp_cb *mpcb);
void mptcp_cleanup_scheduler(struct mptcp_cb *mpcb);
void mptcp_get_default_scheduler(char *name);
int mptcp_set_default_scheduler(const char *name);
bool mptcp_is_available(struct sock *sk, const struct sk_buff *skb,
			bool zero_wnd_test);
bool mptcp_is_def_unavailable(struct sock *sk);
bool subflow_is_active(const struct tcp_sock *tp);
bool subflow_is_backup(const struct tcp_sock *tp);
struct sock *get_available_subflow(struct sock *meta_sk, struct sk_buff *skb,
				   bool zero_wnd_test);
extern struct mptcp_sched_ops mptcp_sched_default;


static inline void mptcp_init_tcp_sock(struct sock *sk)
{
	if (!mptcp_init_failed && sysctl_mptcp_enabled == MPTCP_SYSCTL)
		mptcp_enable_sock(sk);
}

static inline int mptcp_pi_to_flag(int pi)
{
	return 1 << (pi - 1);
}

static inline
struct mptcp_request_sock *mptcp_rsk(const struct request_sock *req)
{
	return (struct mptcp_request_sock *)req;
}

static inline
struct request_sock *rev_mptcp_rsk(const struct mptcp_request_sock *req)
{
	return (struct request_sock *)req;
}

static inline bool mptcp_can_sendpage(struct sock *sk)
{
	struct sock *sk_it;

	if (tcp_sk(sk)->mpcb->dss_csum)
		return false;

	mptcp_for_each_sk(tcp_sk(sk)->mpcb, sk_it) {
		if (!(sk_it->sk_route_caps & NETIF_F_SG) ||
		    !sk_check_csum_caps(sk_it))
			return false;
	}

	return true;
}

static inline void mptcp_push_pending_frames(struct sock *meta_sk)
{
	
	if (tcp_sk(meta_sk)->packets_out || tcp_send_head(meta_sk)) {
		struct tcp_sock *tp = tcp_sk(meta_sk);

		
		__tcp_push_pending_frames(meta_sk, 0, tp->nonagle);
	}
}

static inline void mptcp_send_reset(struct sock *sk)
{
	if (tcp_need_reset(sk->sk_state))
		tcp_sk(sk)->ops->send_active_reset(sk, GFP_ATOMIC);
	mptcp_sub_force_close(sk);
}

static inline void mptcp_sub_force_close_all(struct mptcp_cb *mpcb,
					     struct sock *except)
{
	struct sock *sk_it, *tmp;

	mptcp_for_each_sk_safe(mpcb, sk_it, tmp) {
		if (sk_it != except)
			mptcp_send_reset(sk_it);
	}
}

static inline bool mptcp_is_data_seq(const struct sk_buff *skb)
{
	return TCP_SKB_CB(skb)->mptcp_flags & MPTCPHDR_SEQ;
}

static inline bool mptcp_is_data_fin(const struct sk_buff *skb)
{
	return TCP_SKB_CB(skb)->mptcp_flags & MPTCPHDR_FIN;
}


static inline bool mptcp_is_data_fin2(const struct sk_buff *skb,
				     const struct tcp_sock *tp)
{
	return mptcp_is_data_fin(skb) ||
	       (tp->mpcb->infinite_mapping_rcv &&
		   (TCP_SKB_CB(skb)->tcp_flags & TCPHDR_FIN));
}

static inline u8 mptcp_get_64_bit(u64 data_seq, struct mptcp_cb *mpcb)
{
	u64 data_seq_high = (u32)(data_seq >> 32);

	if (mpcb->rcv_high_order[0] == data_seq_high)
		return 0;
	else if (mpcb->rcv_high_order[1] == data_seq_high)
		return MPTCPHDR_SEQ64_INDEX;
	else
		return MPTCPHDR_SEQ64_OFO;
}


static inline __u32 *mptcp_skb_set_data_seq(const struct sk_buff *skb,
					    u32 *data_seq,
					    struct mptcp_cb *mpcb)
{
	__u32 *ptr = (__u32 *)(skb_transport_header(skb) + TCP_SKB_CB(skb)->dss_off);

	if (TCP_SKB_CB(skb)->mptcp_flags & MPTCPHDR_SEQ64_SET) {
		u64 data_seq64 = get_unaligned_be64(ptr);

		if (mpcb)
			TCP_SKB_CB(skb)->mptcp_flags |= mptcp_get_64_bit(data_seq64, mpcb);

		*data_seq = (u32)data_seq64;
		ptr++;
	} else {
		*data_seq = get_unaligned_be32(ptr);
	}

	return ptr;
}

static inline struct sock *mptcp_meta_sk(const struct sock *sk)
{
	return tcp_sk(sk)->meta_sk;
}

static inline struct tcp_sock *mptcp_meta_tp(const struct tcp_sock *tp)
{
	return tcp_sk(tp->meta_sk);
}

static inline int is_meta_tp(const struct tcp_sock *tp)
{
	return tp->mpcb && mptcp_meta_tp(tp) == tp;
}

static inline int is_meta_sk(const struct sock *sk)
{
	return sk->sk_state != TCP_NEW_SYN_RECV &&
	       sk->sk_type == SOCK_STREAM && sk->sk_protocol == IPPROTO_TCP &&
	       mptcp(tcp_sk(sk)) && mptcp_meta_sk(sk) == sk;
}

static inline int is_master_tp(const struct tcp_sock *tp)
{
	return !mptcp(tp) || (!tp->mptcp->slave_sk && !is_meta_tp(tp));
}

static inline void mptcp_init_mp_opt(struct mptcp_options_received *mopt)
{
	mopt->saw_mpc = 0;
	mopt->dss_csum = 0;
	mopt->drop_me = 0;

	mopt->is_mp_join = 0;
	mopt->join_ack = 0;

	mopt->saw_low_prio = 0;
	mopt->low_prio = 0;

	mopt->saw_add_addr = 0;
	mopt->more_add_addr = 0;

	mopt->saw_rem_addr = 0;
	mopt->more_rem_addr = 0;

	mopt->mp_fail = 0;
	mopt->mp_fclose = 0;
}

static inline void mptcp_reset_mopt(struct tcp_sock *tp)
{
	struct mptcp_options_received *mopt = &tp->mptcp->rx_opt;

	mopt->saw_low_prio = 0;
	mopt->saw_add_addr = 0;
	mopt->more_add_addr = 0;
	mopt->saw_rem_addr = 0;
	mopt->more_rem_addr = 0;
	mopt->join_ack = 0;
	mopt->mp_fail = 0;
	mopt->mp_fclose = 0;
}

static inline __be32 mptcp_get_highorder_sndbits(const struct sk_buff *skb,
						 const struct mptcp_cb *mpcb)
{
	return htonl(mpcb->snd_high_order[(TCP_SKB_CB(skb)->mptcp_flags &
			MPTCPHDR_SEQ64_INDEX) ? 1 : 0]);
}

static inline u64 mptcp_get_data_seq_64(const struct mptcp_cb *mpcb, int index,
					u32 data_seq_32)
{
	return ((u64)mpcb->rcv_high_order[index] << 32) | data_seq_32;
}

static inline u64 mptcp_get_rcv_nxt_64(const struct tcp_sock *meta_tp)
{
	struct mptcp_cb *mpcb = meta_tp->mpcb;
	return mptcp_get_data_seq_64(mpcb, mpcb->rcv_hiseq_index,
				     meta_tp->rcv_nxt);
}

static inline void mptcp_check_sndseq_wrap(struct tcp_sock *meta_tp, int inc)
{
	if (unlikely(meta_tp->snd_nxt > meta_tp->snd_nxt + inc)) {
		struct mptcp_cb *mpcb = meta_tp->mpcb;
		mpcb->snd_hiseq_index = mpcb->snd_hiseq_index ? 0 : 1;
		mpcb->snd_high_order[mpcb->snd_hiseq_index] += 2;
	}
}

static inline void mptcp_check_rcvseq_wrap(struct tcp_sock *meta_tp,
					   u32 old_rcv_nxt)
{
	if (unlikely(old_rcv_nxt > meta_tp->rcv_nxt)) {
		struct mptcp_cb *mpcb = meta_tp->mpcb;
		mpcb->rcv_high_order[mpcb->rcv_hiseq_index] += 2;
		mpcb->rcv_hiseq_index = mpcb->rcv_hiseq_index ? 0 : 1;
	}
}

static inline int mptcp_sk_can_send(const struct sock *sk)
{
	return tcp_passive_fastopen(sk) ||
	       ((1 << sk->sk_state) & (TCPF_ESTABLISHED | TCPF_CLOSE_WAIT) &&
		!tcp_sk(sk)->mptcp->pre_established);
}

static inline int mptcp_sk_can_recv(const struct sock *sk)
{
	return (1 << sk->sk_state) & (TCPF_ESTABLISHED | TCPF_FIN_WAIT1 | TCPF_FIN_WAIT2);
}

static inline int mptcp_sk_can_send_ack(const struct sock *sk)
{
	return !((1 << sk->sk_state) & (TCPF_SYN_SENT | TCPF_SYN_RECV |
					TCPF_CLOSE | TCPF_LISTEN)) &&
	       !tcp_sk(sk)->mptcp->pre_established;
}


static inline bool mptcp_sk_can_gso(const struct sock *meta_sk)
{
	struct sock *sk;

	if (tcp_sk(meta_sk)->mpcb->dss_csum)
		return false;

	mptcp_for_each_sk(tcp_sk(meta_sk)->mpcb, sk) {
		if (!mptcp_sk_can_send(sk))
			continue;
		if (!sk_can_gso(sk))
			return false;
	}
	return true;
}

static inline bool mptcp_can_sg(const struct sock *meta_sk)
{
	struct sock *sk;

	if (tcp_sk(meta_sk)->mpcb->dss_csum)
		return false;

	mptcp_for_each_sk(tcp_sk(meta_sk)->mpcb, sk) {
		if (!mptcp_sk_can_send(sk))
			continue;
		if (!(sk->sk_route_caps & NETIF_F_SG))
			return false;
	}
	return true;
}

static inline void mptcp_set_rto(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct sock *sk_it;
	struct inet_connection_sock *micsk = inet_csk(mptcp_meta_sk(sk));
	__u32 max_rto = 0;

	
	if (micsk->icsk_retransmits)
		return;

	mptcp_for_each_sk(tp->mpcb, sk_it) {
		if ((mptcp_sk_can_send(sk_it) || sk->sk_state == TCP_SYN_RECV) &&
		    inet_csk(sk_it)->icsk_rto > max_rto)
			max_rto = inet_csk(sk_it)->icsk_rto;
	}
	if (max_rto) {
		micsk->icsk_rto = max_rto << 1;

		
		micsk->icsk_backoff = 0;
	}
}

static inline void mptcp_sub_close_passive(struct sock *sk)
{
	struct sock *meta_sk = mptcp_meta_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk), *meta_tp = tcp_sk(meta_sk);

	
	if (tp->mpcb->passive_close && meta_tp->snd_una == meta_tp->write_seq)
		mptcp_sub_close(sk, 0);
}

static inline bool mptcp_fallback_infinite(struct sock *sk, int flag)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct mptcp_cb *mpcb = tp->mpcb;

	
	if (likely(tp->mptcp->fully_established))
		return false;

	if (!(flag & MPTCP_FLAG_DATA_ACKED))
		return false;

	
	if (mpcb->infinite_mapping_snd)
		return false;

	pr_err("%s %#x will fallback - pi %d, src %pI4:%u dst %pI4:%u rcv_nxt %u from %pS\n",
	       __func__, mpcb->mptcp_loc_token, tp->mptcp->path_index,
	       &inet_sk(sk)->inet_saddr, ntohs(inet_sk(sk)->inet_sport),
	       &inet_sk(sk)->inet_daddr, ntohs(inet_sk(sk)->inet_dport),
	       tp->rcv_nxt, __builtin_return_address(0));
	if (!is_master_tp(tp)) {
		MPTCP_INC_STATS(sock_net(sk), MPTCP_MIB_FBACKSUB);
		return true;
	}

	mpcb->infinite_mapping_snd = 1;
	mpcb->infinite_mapping_rcv = 1;
	mpcb->infinite_rcv_seq = mptcp_get_rcv_nxt_64(mptcp_meta_tp(tp));
	tp->mptcp->fully_established = 1;

	mptcp_sub_force_close_all(mpcb, sk);

	MPTCP_INC_STATS(sock_net(sk), MPTCP_MIB_FBACKINIT);

	return false;
}


static inline u8 mptcp_set_new_pathindex(struct mptcp_cb *mpcb)
{
	u8 base = mpcb->next_path_index;
	int i;

	
	mptcp_for_each_bit_unset(mpcb->path_index_bits >> base, i) {
		if (i + base < 1)
			continue;
		if (i + base >= sizeof(mpcb->path_index_bits) * 8)
			break;
		i += base;
		mpcb->path_index_bits |= (1 << i);
		mpcb->next_path_index = i + 1;
		return i;
	}
	mptcp_for_each_bit_unset(mpcb->path_index_bits, i) {
		if (i >= sizeof(mpcb->path_index_bits) * 8)
			break;
		if (i < 1)
			continue;
		mpcb->path_index_bits |= (1 << i);
		mpcb->next_path_index = i + 1;
		return i;
	}

	return 0;
}

static inline bool mptcp_v6_is_v4_mapped(const struct sock *sk)
{
	return sk->sk_family == AF_INET6 &&
	       ipv6_addr_type(&inet6_sk(sk)->saddr) == IPV6_ADDR_MAPPED;
}

static inline bool mptcp_can_new_subflow(const struct sock *meta_sk)
{
	
	return meta_sk->sk_state != TCP_CLOSE &&
	       tcp_sk(meta_sk)->inside_tk_table &&
	       !tcp_sk(meta_sk)->mpcb->infinite_mapping_rcv &&
	       !tcp_sk(meta_sk)->mpcb->send_infinite_mapping;
}


u16 mptcp_select_window(struct sock *sk);
void mptcp_init_buffer_space(struct sock *sk);
void mptcp_tcp_set_rto(struct sock *sk);


bool mptcp_prune_ofo_queue(struct sock *sk);

#else 
#define mptcp_debug(fmt, args...)	\
	do {				\
	} while (0)


#define mptcp_for_each_sk(mpcb, sk)
#define mptcp_for_each_sk_safe(__mpcb, __sk, __temp)

#define MPTCP_INC_STATS(net, field)	\
	do {				\
	} while (0)

static inline bool mptcp_is_data_fin(const struct sk_buff *skb)
{
	return false;
}
static inline bool mptcp_is_data_seq(const struct sk_buff *skb)
{
	return false;
}
static inline struct sock *mptcp_meta_sk(const struct sock *sk)
{
	return NULL;
}
static inline struct tcp_sock *mptcp_meta_tp(const struct tcp_sock *tp)
{
	return NULL;
}
static inline int is_meta_sk(const struct sock *sk)
{
	return 0;
}
static inline int is_master_tp(const struct tcp_sock *tp)
{
	return 0;
}
static inline void mptcp_del_sock(const struct sock *sk) {}
static inline void mptcp_update_metasocket(const struct sock *meta_sk) {}
static inline void mptcp_reinject_data(struct sock *orig_sk, int clone_it) {}
static inline void mptcp_update_sndbuf(const struct tcp_sock *tp) {}
static inline void mptcp_clean_rtx_infinite(const struct sk_buff *skb,
					    const struct sock *sk) {}
static inline void mptcp_sub_close(struct sock *sk, unsigned long delay) {}
static inline void mptcp_set_rto(const struct sock *sk) {}
static inline void mptcp_send_fin(const struct sock *meta_sk) {}
static inline void mptcp_parse_options(const uint8_t *ptr, const int opsize,
				       struct mptcp_options_received *mopt,
				       const struct sk_buff *skb,
				       const struct tcp_sock *tp) {}
static inline void mptcp_syn_options(const struct sock *sk,
				     struct tcp_out_options *opts,
				     unsigned int *remaining) {}
static inline void mptcp_synack_options(struct request_sock *req,
					struct tcp_out_options *opts,
					unsigned int *remaining) {}

static inline void mptcp_established_options(struct sock *sk,
					     struct sk_buff *skb,
					     struct tcp_out_options *opts,
					     unsigned int *size) {}
static inline void mptcp_options_write(__be32 *ptr, struct tcp_sock *tp,
				       const struct tcp_out_options *opts,
				       struct sk_buff *skb) {}
static inline void mptcp_close(struct sock *meta_sk, long timeout) {}
static inline int mptcp_doit(struct sock *sk)
{
	return 0;
}
static inline int mptcp_check_req_fastopen(struct sock *child,
					   struct request_sock *req)
{
	return 1;
}
static inline int mptcp_check_req_master(const struct sock *sk,
					 const struct sock *child,
					 const struct request_sock *req,
					 const struct sk_buff *skb,
					 int drop)
{
	return 1;
}
static inline struct sock *mptcp_check_req_child(const struct sock *meta_sk,
						 const struct sock *child,
						 const struct request_sock *req,
						 struct sk_buff *skb,
						 const struct mptcp_options_received *mopt)
{
	return NULL;
}
static inline unsigned int mptcp_current_mss(struct sock *meta_sk)
{
	return 0;
}
static inline void mptcp_sub_close_passive(struct sock *sk) {}
static inline bool mptcp_fallback_infinite(const struct sock *sk, int flag)
{
	return false;
}
static inline void mptcp_init_mp_opt(const struct mptcp_options_received *mopt) {}
static inline bool mptcp_check_rtt(const struct tcp_sock *tp, int time)
{
	return false;
}
static inline int mptcp_check_snd_buf(const struct tcp_sock *tp)
{
	return 0;
}
static inline void mptcp_send_reset(const struct sock *sk) {}
static inline bool mptcp_handle_options(struct sock *sk,
					const struct tcphdr *th,
					struct sk_buff *skb)
{
	return false;
}
static inline void mptcp_reset_mopt(struct tcp_sock *tp) {}
static inline void  __init mptcp_init(void) {}
static inline bool mptcp_sk_can_gso(const struct sock *sk)
{
	return false;
}
static inline bool mptcp_can_sg(const struct sock *meta_sk)
{
	return false;
}
static inline unsigned int mptcp_xmit_size_goal(const struct sock *meta_sk,
						u32 mss_now, int large_allowed)
{
	return 0;
}
static inline void mptcp_destroy_sock(struct sock *sk) {}
static inline int mptcp_rcv_synsent_state_process(struct sock *sk,
						  struct sock **skptr,
						  struct sk_buff *skb,
						  const struct mptcp_options_received *mopt)
{
	return 0;
}
static inline bool mptcp_can_sendpage(struct sock *sk)
{
	return false;
}
static inline int mptcp_init_tw_sock(struct sock *sk,
				     struct tcp_timewait_sock *tw)
{
	return 0;
}
static inline void mptcp_twsk_destructor(struct tcp_timewait_sock *tw) {}
static inline void mptcp_disconnect(struct sock *sk) {}
static inline void mptcp_tsq_flags(struct sock *sk) {}
static inline void mptcp_tsq_sub_deferred(struct sock *meta_sk) {}
static inline void mptcp_hash_remove_bh(struct tcp_sock *meta_tp) {}
static inline void mptcp_remove_shortcuts(const struct mptcp_cb *mpcb,
					  const struct sk_buff *skb) {}
static inline void mptcp_init_tcp_sock(struct sock *sk) {}
static inline void mptcp_disable_static_key(void) {}
static inline void mptcp_cookies_reqsk_init(struct request_sock *req,
					    struct mptcp_options_received *mopt,
					    struct sk_buff *skb) {}
static inline void mptcp_fin(struct sock *meta_sk) {}
static inline bool mptcp_can_new_subflow(const struct sock *meta_sk)
{
	return false;
}

#endif 

#endif 
