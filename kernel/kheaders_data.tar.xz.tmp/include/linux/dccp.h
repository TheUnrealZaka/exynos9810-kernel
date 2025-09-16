#ifndef _LINUX_DCCP_H
#define _LINUX_DCCP_H


#include <linux/in.h>
#include <linux/interrupt.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/uio.h>
#include <linux/workqueue.h>

#include <net/inet_connection_sock.h>
#include <net/inet_sock.h>
#include <net/inet_timewait_sock.h>
#include <net/tcp_states.h>
#include <uapi/linux/dccp.h>

enum dccp_state {
	DCCP_OPEN	     = TCP_ESTABLISHED,
	DCCP_REQUESTING	     = TCP_SYN_SENT,
	DCCP_LISTEN	     = TCP_LISTEN,
	DCCP_RESPOND	     = TCP_SYN_RECV,
	
	DCCP_ACTIVE_CLOSEREQ = TCP_FIN_WAIT1,
	DCCP_PASSIVE_CLOSE   = TCP_CLOSE_WAIT,	
	DCCP_CLOSING	     = TCP_CLOSING,
	DCCP_TIME_WAIT	     = TCP_TIME_WAIT,
	DCCP_CLOSED	     = TCP_CLOSE,
	DCCP_NEW_SYN_RECV    = TCP_NEW_SYN_RECV,
	DCCP_PARTOPEN	     = TCP_MAX_STATES,
	DCCP_PASSIVE_CLOSEREQ,			
	DCCP_MAX_STATES
};

enum {
	DCCPF_OPEN	      = TCPF_ESTABLISHED,
	DCCPF_REQUESTING      = TCPF_SYN_SENT,
	DCCPF_LISTEN	      = TCPF_LISTEN,
	DCCPF_RESPOND	      = TCPF_SYN_RECV,
	DCCPF_ACTIVE_CLOSEREQ = TCPF_FIN_WAIT1,
	DCCPF_CLOSING	      = TCPF_CLOSING,
	DCCPF_TIME_WAIT	      = TCPF_TIME_WAIT,
	DCCPF_CLOSED	      = TCPF_CLOSE,
	DCCPF_NEW_SYN_RECV    = TCPF_NEW_SYN_RECV,
	DCCPF_PARTOPEN	      = (1 << DCCP_PARTOPEN),
};

static inline struct dccp_hdr *dccp_hdr(const struct sk_buff *skb)
{
	return (struct dccp_hdr *)skb_transport_header(skb);
}

static inline struct dccp_hdr *dccp_zeroed_hdr(struct sk_buff *skb, int headlen)
{
	skb_push(skb, headlen);
	skb_reset_transport_header(skb);
	return memset(skb_transport_header(skb), 0, headlen);
}

static inline struct dccp_hdr_ext *dccp_hdrx(const struct dccp_hdr *dh)
{
	return (struct dccp_hdr_ext *)((unsigned char *)dh + sizeof(*dh));
}

static inline unsigned int __dccp_basic_hdr_len(const struct dccp_hdr *dh)
{
	return sizeof(*dh) + (dh->dccph_x ? sizeof(struct dccp_hdr_ext) : 0);
}

static inline unsigned int dccp_basic_hdr_len(const struct sk_buff *skb)
{
	const struct dccp_hdr *dh = dccp_hdr(skb);
	return __dccp_basic_hdr_len(dh);
}

static inline __u64 dccp_hdr_seq(const struct dccp_hdr *dh)
{
	__u64 seq_nr =  ntohs(dh->dccph_seq);

	if (dh->dccph_x != 0)
		seq_nr = (seq_nr << 32) + ntohl(dccp_hdrx(dh)->dccph_seq_low);
	else
		seq_nr += (u32)dh->dccph_seq2 << 16;

	return seq_nr;
}

static inline struct dccp_hdr_request *dccp_hdr_request(struct sk_buff *skb)
{
	return (struct dccp_hdr_request *)(skb_transport_header(skb) +
					   dccp_basic_hdr_len(skb));
}

static inline struct dccp_hdr_ack_bits *dccp_hdr_ack_bits(const struct sk_buff *skb)
{
	return (struct dccp_hdr_ack_bits *)(skb_transport_header(skb) +
					    dccp_basic_hdr_len(skb));
}

static inline u64 dccp_hdr_ack_seq(const struct sk_buff *skb)
{
	const struct dccp_hdr_ack_bits *dhack = dccp_hdr_ack_bits(skb);
	return ((u64)ntohs(dhack->dccph_ack_nr_high) << 32) + ntohl(dhack->dccph_ack_nr_low);
}

static inline struct dccp_hdr_response *dccp_hdr_response(struct sk_buff *skb)
{
	return (struct dccp_hdr_response *)(skb_transport_header(skb) +
					    dccp_basic_hdr_len(skb));
}

static inline struct dccp_hdr_reset *dccp_hdr_reset(struct sk_buff *skb)
{
	return (struct dccp_hdr_reset *)(skb_transport_header(skb) +
					 dccp_basic_hdr_len(skb));
}

static inline unsigned int __dccp_hdr_len(const struct dccp_hdr *dh)
{
	return __dccp_basic_hdr_len(dh) +
	       dccp_packet_hdr_len(dh->dccph_type);
}

static inline unsigned int dccp_hdr_len(const struct sk_buff *skb)
{
	return __dccp_hdr_len(dccp_hdr(skb));
}


struct dccp_request_sock {
	struct inet_request_sock dreq_inet_rsk;
	__u64			 dreq_iss;
	__u64			 dreq_gss;
	__u64			 dreq_isr;
	__u64			 dreq_gsr;
	__be32			 dreq_service;
	spinlock_t		 dreq_lock;
	struct list_head	 dreq_featneg;
	__u32			 dreq_timestamp_echo;
	__u32			 dreq_timestamp_time;
};

static inline struct dccp_request_sock *dccp_rsk(const struct request_sock *req)
{
	return (struct dccp_request_sock *)req;
}

extern struct inet_timewait_death_row dccp_death_row;

extern int dccp_parse_options(struct sock *sk, struct dccp_request_sock *dreq,
			      struct sk_buff *skb);

struct dccp_options_received {
	u64	dccpor_ndp:48;
	u32	dccpor_timestamp;
	u32	dccpor_timestamp_echo;
	u32	dccpor_elapsed_time;
};

struct ccid;

enum dccp_role {
	DCCP_ROLE_UNDEFINED,
	DCCP_ROLE_LISTEN,
	DCCP_ROLE_CLIENT,
	DCCP_ROLE_SERVER,
};

struct dccp_service_list {
	__u32	dccpsl_nr;
	__be32	dccpsl_list[0];
};

#define DCCP_SERVICE_INVALID_VALUE htonl((__u32)-1)
#define DCCP_SERVICE_CODE_IS_ABSENT		0

static inline bool dccp_list_has_service(const struct dccp_service_list *sl,
					const __be32 service)
{
	if (likely(sl != NULL)) {
		u32 i = sl->dccpsl_nr;
		while (i--)
			if (sl->dccpsl_list[i] == service)
				return true;
	}
	return false;
}

struct dccp_ackvec;


struct dccp_sock {
	
	struct inet_connection_sock	dccps_inet_connection;
#define dccps_syn_rtt			dccps_inet_connection.icsk_ack.lrcvtime
	__u64				dccps_swl;
	__u64				dccps_swh;
	__u64				dccps_awl;
	__u64				dccps_awh;
	__u64				dccps_iss;
	__u64				dccps_isr;
	__u64				dccps_osr;
	__u64				dccps_gss;
	__u64				dccps_gsr;
	__u64				dccps_gar;
	__be32				dccps_service;
	__u32				dccps_mss_cache;
	struct dccp_service_list	*dccps_service_list;
	__u32				dccps_timestamp_echo;
	__u32				dccps_timestamp_time;
	__u16				dccps_l_ack_ratio;
	__u16				dccps_r_ack_ratio;
	__u64				dccps_l_seq_win:48;
	__u64				dccps_r_seq_win:48;
	__u8				dccps_pcslen:4;
	__u8				dccps_pcrlen:4;
	__u8				dccps_send_ndp_count:1;
	__u64				dccps_ndp_count:48;
	unsigned long			dccps_rate_last;
	struct list_head		dccps_featneg;
	struct dccp_ackvec		*dccps_hc_rx_ackvec;
	struct ccid			*dccps_hc_rx_ccid;
	struct ccid			*dccps_hc_tx_ccid;
	struct dccp_options_received	dccps_options_received;
	__u8				dccps_qpolicy;
	__u32				dccps_tx_qlen;
	enum dccp_role			dccps_role:2;
	__u8				dccps_hc_rx_insert_options:1;
	__u8				dccps_hc_tx_insert_options:1;
	__u8				dccps_server_timewait:1;
	__u8				dccps_sync_scheduled:1;
	struct tasklet_struct		dccps_xmitlet;
	struct timer_list		dccps_xmit_timer;
};

static inline struct dccp_sock *dccp_sk(const struct sock *sk)
{
	return (struct dccp_sock *)sk;
}

static inline const char *dccp_role(const struct sock *sk)
{
	switch (dccp_sk(sk)->dccps_role) {
	case DCCP_ROLE_UNDEFINED: return "undefined";
	case DCCP_ROLE_LISTEN:	  return "listen";
	case DCCP_ROLE_SERVER:	  return "server";
	case DCCP_ROLE_CLIENT:	  return "client";
	}
	return NULL;
}

extern void dccp_syn_ack_timeout(const struct request_sock *req);

#endif 
