
#ifndef _LINUX_TCP_H
#define _LINUX_TCP_H


#include <linux/skbuff.h>
#include <linux/win_minmax.h>
#include <net/sock.h>
#include <net/inet_connection_sock.h>
#include <net/inet_timewait_sock.h>
#include <uapi/linux/tcp.h>

static inline struct tcphdr *tcp_hdr(const struct sk_buff *skb)
{
	return (struct tcphdr *)skb_transport_header(skb);
}

static inline unsigned int __tcp_hdrlen(const struct tcphdr *th)
{
	return th->doff * 4;
}

static inline unsigned int tcp_hdrlen(const struct sk_buff *skb)
{
	return __tcp_hdrlen(tcp_hdr(skb));
}

static inline struct tcphdr *inner_tcp_hdr(const struct sk_buff *skb)
{
	return (struct tcphdr *)skb_inner_transport_header(skb);
}

static inline unsigned int inner_tcp_hdrlen(const struct sk_buff *skb)
{
	return inner_tcp_hdr(skb)->doff * 4;
}

static inline unsigned int tcp_optlen(const struct sk_buff *skb)
{
	return (tcp_hdr(skb)->doff - 5) * 4;
}


#define TCP_FASTOPEN_COOKIE_MIN	4	
#define TCP_FASTOPEN_COOKIE_MAX	16	
#ifdef CONFIG_MPTCP
	#define TCP_FASTOPEN_COOKIE_SIZE 4	
#else
	#define TCP_FASTOPEN_COOKIE_SIZE 8	
#endif


struct tcp_fastopen_cookie {
	union {
		u8	val[TCP_FASTOPEN_COOKIE_MAX];
#if IS_ENABLED(CONFIG_IPV6)
		struct in6_addr addr;
#endif
	};
	s8	len;
	bool	exp;	
};


struct tcp_sack_block_wire {
	__be32	start_seq;
	__be32	end_seq;
};

struct tcp_sack_block {
	u32	start_seq;
	u32	end_seq;
};

#ifdef CONFIG_MPTCP
struct tcp_out_options {
	u16	options;	
	u8	ws;		
	u8	num_sack_blocks;
	u8	hash_size;	
	u16	mss;		
	__u8	*hash_location;	
	__u32	tsval, tsecr;	
	struct tcp_fastopen_cookie *fastopen_cookie;	
	u16	mptcp_options;	
	u8	dss_csum:1,	
		add_addr_v4:1,
		add_addr_v6:1,
		mptcp_ver:4;

	union {
		struct {
			__u64	sender_key;	
			__u64	receiver_key;	
		} mp_capable;

		struct {
			__u64	sender_truncated_mac;
			__u32	sender_nonce;
					
			__u32	token;	
			u8	low_prio:1;
		} mp_join_syns;
	};

	struct {
		__u64 trunc_mac;
		struct in_addr addr;
		u16 port;
		u8 addr_id;
	} add_addr4;

	struct {
		__u64 trunc_mac;
		struct in6_addr addr;
		u16 port;
		u8 addr_id;
	} add_addr6;

	u16	remove_addrs;	
	u8	addr_id;	
};
#endif


#define TCP_SACK_SEEN     (1 << 0)   
#define TCP_FACK_ENABLED  (1 << 1)   
#define TCP_DSACK_SEEN    (1 << 2)   

struct tcp_options_received {

	long	ts_recent_stamp;
	u32	ts_recent;	
	u32	rcv_tsval;	
	u32	rcv_tsecr;	
	u16 	saw_tstamp : 1,	
		tstamp_ok : 1,	
		dsack : 1,	
		wscale_ok : 1,	
		sack_ok : 4,	
		snd_wscale : 4,	
		rcv_wscale : 4;	
	u8	num_sacks;	
	u16	user_mss;	
	u16	mss_clamp;	
};

#ifdef CONFIG_MPTCP
	struct mptcp_cb;
	struct mptcp_tcp_sock;
#endif

static inline void tcp_clear_options(struct tcp_options_received *rx_opt)
{
	rx_opt->tstamp_ok = rx_opt->sack_ok = 0;
	rx_opt->wscale_ok = rx_opt->snd_wscale = 0;
}


#define TCP_NUM_SACKS 4

struct tcp_request_sock_ops;

struct tcp_request_sock {
	struct inet_request_sock 	req;
	const struct tcp_request_sock_ops *af_specific;
	struct skb_mstamp		snt_synack; 
	bool				tfo_listener;
	u32				txhash;
	u32				rcv_isn;
	u32				snt_isn;
	u32				last_oow_ack_time; 
	u32				rcv_nxt; 
};

static inline struct tcp_request_sock *tcp_rsk(const struct request_sock *req)
{
	return (struct tcp_request_sock *)req;
}

#ifdef CONFIG_MPTCP
	struct tcp_md5sig_key;
#endif

struct tcp_sock {
	
	struct inet_connection_sock	inet_conn;
	u16	tcp_header_len;	
	u16	gso_segs;	


	__be32	pred_flags;


	u64	bytes_received;	
	u32	segs_in;	
	u32	data_segs_in;	
 	u32	rcv_nxt;	
	u32	copied_seq;	
	u32	rcv_wup;	
 	u32	snd_nxt;	
	u32	segs_out;	
	u32	data_segs_out;	
	u64	bytes_acked;	
	struct u64_stats_sync syncp; 

 	u32	snd_una;	
 	u32	snd_sml;	
	u32	rcv_tstamp;	
	u32	lsndtime;	
	u32	last_oow_ack_time;  

	u32	tsoffset;	

	struct list_head tsq_node; 
	unsigned long	tsq_flags;

	
	struct {
		struct sk_buff_head	prequeue;
		struct task_struct	*task;
		struct msghdr		*msg;
		int			memory;
		int			len;
	} ucopy;

	u32	snd_wl1;	
	u32	snd_wnd;	
	u32	max_window;	
	u32	mss_cache;	

	u32	window_clamp;	
	u32	rcv_ssthresh;	

	
	struct tcp_rack {
		struct skb_mstamp mstamp; 
		u8 advanced; 
		u8 reord;    
	} rack;
	u16	advmss;		
	u8	tlp_retrans:1,	
		unused_1:7;
	u32	chrono_start;	
	u32	chrono_stat[3];	
	u8	chrono_type:2,	
		rate_app_limited:1,  
                fastopen_connect:1, 
                is_sack_reneg:1,    
		unused:4;
	u8	nonagle     : 4,
		thin_lto    : 1,
		thin_dupack : 1,
		repair      : 1,
		frto        : 1;
	u8	repair_queue;
	u8	do_early_retrans:1,
		syn_data:1,	
		syn_fastopen:1,	
		syn_fastopen_exp:1,
		syn_data_acked:1,
		save_syn:1,	
		is_cwnd_limited:1;
	u32	tlp_high_seq;	


    u64	tcp_mstamp;	
	u32	srtt_us;	
	u32	mdev_us;	
	u32	mdev_max_us;	
	u32	rttvar_us;	
	u32	rtt_seq;	
	struct  minmax rtt_min;

	u32	packets_out;	
	u32	retrans_out;	
	u32	max_packets_out;  
	u32	cwnd_usage_seq;  

	u16	urg_data;	
	u8	ecn_flags;	
	u8	keepalive_probes; 
	u32	reordering;	
	u32	snd_up;		


	struct tcp_options_received rx_opt;


 	u32	snd_ssthresh;	
 	u32	snd_cwnd;	
	u32	snd_cwnd_cnt;	
	u32	snd_cwnd_clamp; 
	u32	snd_cwnd_used;
	u32	snd_cwnd_stamp;
	u32	prior_cwnd;	
	u32	prr_delivered;	
	u32	prr_out;	
	u32	delivered;	
	u32	lost;		
	u32	app_limited;	
	struct skb_mstamp first_tx_mstamp;  
	struct skb_mstamp delivered_mstamp; 
	u32	rate_delivered;    
	u32	rate_interval_us;  

 	u32	rcv_wnd;	
	u32	write_seq;	
	u32	notsent_lowat;	
	u32	pushed_seq;	
	u32	lost_out;	
	u32	sacked_out;	
	u32	fackets_out;	

	struct hrtimer	pacing_timer;

	
	struct sk_buff* lost_skb_hint;
	struct sk_buff *retransmit_skb_hint;

	
	struct rb_root	out_of_order_queue;
	struct sk_buff	*ooo_last_skb; 

	
	struct tcp_sack_block duplicate_sack[1]; 
	struct tcp_sack_block selective_acks[4]; 

	struct tcp_sack_block recv_sack_cache[4];

	struct sk_buff *highest_sack;   

	int     lost_cnt_hint;
	u32     retransmit_high;	

	u32	prior_ssthresh; 
	u32	high_seq;	

	u32	retrans_stamp;	
	u32	undo_marker;	
	int	undo_retrans;	
	u32	total_retrans;	

	u32	urg_seq;	
	unsigned int		keepalive_time;	  
	unsigned int		keepalive_intvl;  

	int			linger2;



#ifdef CONFIG_BPF
	u8	bpf_sock_ops_cb_flags;  
#define BPF_SOCK_OPS_TEST_FLAG(TP, ARG) (TP->bpf_sock_ops_cb_flags & ARG)
#else
#define BPF_SOCK_OPS_TEST_FLAG(TP, ARG) 0
#endif


#ifdef CONFIG_NETPM
	u8 netpm_netif;
	u8 netpm_rbuf_flag;
	u32 netpm_rtt_min;
	u32 netpm_srtt;
	u32 netpm_rttvar;
	int netpm_cwnd_est;
	int netpm_tcp_rmem_max;
	int netpm_max_tput;
	int netpm_rmem_max_curbdp;
#endif


	struct {
		u32	rtt;
		u32	seq;
		u32	time;
	} rcv_rtt_est;


	struct {
		u32	space;
		u32	seq;
		u32	time;
	} rcvq_space;


	struct {
		u32		  probe_seq_start;
		u32		  probe_seq_end;
	} mtu_probe;
	u32	mtu_info; 

#ifdef CONFIG_TCP_MD5SIG

	const struct tcp_sock_af_ops	*af_specific;


	struct tcp_md5sig_info	__rcu *md5sig_info;
#endif


	struct tcp_fastopen_request *fastopen_req;
	
	struct request_sock *fastopen_rsk;
	u32	*saved_syn;
#ifdef CONFIG_MPTCP
	
	const struct tcp_sock_ops	*ops;

	struct mptcp_cb		*mpcb;
	struct sock		*meta_sk;
	
	u32     mpc:1,          
		inside_tk_table:1, 
		send_mp_fclose:1,
		request_mptcp:1, 
		pf:1, 
		mp_killed:1, 
		was_meta_sk:1,	
		is_master_sk:1,
		close_it:1,	
		closing:1,
		mptcp_ver:4,
		mptcp_sched_setsockopt:1,
		mptcp_pm_setsockopt:1,
		record_master_info:1;
	struct mptcp_tcp_sock *mptcp;
#define MPTCP_SCHED_NAME_MAX 16
#define MPTCP_PM_NAME_MAX 16
	struct hlist_nulls_node tk_table;
	u32		mptcp_loc_token;
	u64		mptcp_loc_key;
	char		mptcp_sched_name[MPTCP_SCHED_NAME_MAX];
	char		mptcp_pm_name[MPTCP_PM_NAME_MAX];
#endif 
};

enum tsq_enum {
	TSQ_THROTTLED,
	TSQ_QUEUED,
	TCP_TSQ_DEFERRED,	   
	TCP_WRITE_TIMER_DEFERRED,  
	TCP_DELACK_TIMER_DEFERRED, 
	TCP_MTU_REDUCED_DEFERRED,  
#ifdef CONFIG_MPTCP
	MPTCP_PATH_MANAGER_DEFERRED, 
	MPTCP_SUB_DEFERRED, 
#endif
};

enum tsq_flags {
	TSQF_THROTTLED			= (1UL << TSQ_THROTTLED),
	TSQF_QUEUED			= (1UL << TSQ_QUEUED),
	TCPF_TSQ_DEFERRED		= (1UL << TCP_TSQ_DEFERRED),
	TCPF_WRITE_TIMER_DEFERRED	= (1UL << TCP_WRITE_TIMER_DEFERRED),
	TCPF_DELACK_TIMER_DEFERRED	= (1UL << TCP_DELACK_TIMER_DEFERRED),
	TCPF_MTU_REDUCED_DEFERRED	= (1UL << TCP_MTU_REDUCED_DEFERRED),
};

static inline struct tcp_sock *tcp_sk(const struct sock *sk)
{
	return (struct tcp_sock *)sk;
}

struct tcp_timewait_sock {
	struct inet_timewait_sock tw_sk;
#define tw_rcv_nxt tw_sk.__tw_common.skc_tw_rcv_nxt
#define tw_snd_nxt tw_sk.__tw_common.skc_tw_snd_nxt
	u32			  tw_rcv_wnd;
	u32			  tw_ts_offset;
	u32			  tw_ts_recent;

	
	u32			  tw_last_oow_ack_time;

	long			  tw_ts_recent_stamp;
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key	  *tw_md5_key;
#endif
#ifdef CONFIG_MPTCP
	struct mptcp_tw		  *mptcp_tw;
#endif
};

static inline struct tcp_timewait_sock *tcp_twsk(const struct sock *sk)
{
	return (struct tcp_timewait_sock *)sk;
}

static inline bool tcp_passive_fastopen(const struct sock *sk)
{
	return (sk->sk_state == TCP_SYN_RECV &&
		tcp_sk(sk)->fastopen_rsk != NULL);
}

static inline void fastopen_queue_tune(struct sock *sk, int backlog)
{
	struct request_sock_queue *queue = &inet_csk(sk)->icsk_accept_queue;
	int somaxconn = READ_ONCE(sock_net(sk)->core.sysctl_somaxconn);

	queue->fastopenq.max_qlen = min_t(unsigned int, backlog, somaxconn);
}

static inline void tcp_move_syn(struct tcp_sock *tp,
				struct request_sock *req)
{
	tp->saved_syn = req->saved_syn;
	req->saved_syn = NULL;
}

static inline void tcp_saved_syn_free(struct tcp_sock *tp)
{
	kfree(tp->saved_syn);
	tp->saved_syn = NULL;
}

int tcp_skb_shift(struct sk_buff *to, struct sk_buff *from, int pcount,
		  int shiftlen);

#endif	
