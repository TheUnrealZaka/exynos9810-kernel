

#ifndef _NET_IP_VS_H
#define _NET_IP_VS_H

#include <linux/ip_vs.h>                

#include <asm/types.h>                  

#include <linux/list.h>                 
#include <linux/spinlock.h>             
#include <linux/atomic.h>               
#include <linux/compiler.h>
#include <linux/timer.h>
#include <linux/bug.h>

#include <net/checksum.h>
#include <linux/netfilter.h>		
#include <linux/ip.h>
#include <linux/ipv6.h>			
#include <net/ipv6.h>
#if IS_ENABLED(CONFIG_IP_VS_IPV6)
#include <linux/netfilter_ipv6/ip6_tables.h>
#endif
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#include <net/netfilter/nf_conntrack.h>
#endif
#include <net/net_namespace.h>		

#define IP_VS_HDR_INVERSE	1
#define IP_VS_HDR_ICMP		2


static inline struct netns_ipvs *net_ipvs(struct net* net)
{
	return net->ipvs;
}


static inline struct net *seq_file_single_net(struct seq_file *seq)
{
#ifdef CONFIG_NET_NS
	return (struct net *)seq->private;
#else
	return &init_net;
#endif
}


extern int ip_vs_conn_tab_size;

struct ip_vs_iphdr {
	int hdr_flags;	
	__u32 off;	
	__u32 len;	
	__u16 fragoffs; 
	__s16 protocol;
	__s32 flags;
	union nf_inet_addr saddr;
	union nf_inet_addr daddr;
};

static inline void *frag_safe_skb_hp(const struct sk_buff *skb, int offset,
				      int len, void *buffer,
				      const struct ip_vs_iphdr *ipvsh)
{
	return skb_header_pointer(skb, offset, len, buffer);
}


static inline int
ip_vs_fill_iph_skb_off(int af, const struct sk_buff *skb, int offset,
		       int hdr_flags, struct ip_vs_iphdr *iphdr)
{
	iphdr->hdr_flags = hdr_flags;
	iphdr->off = offset;

#ifdef CONFIG_IP_VS_IPV6
	if (af == AF_INET6) {
		struct ipv6hdr _iph;
		const struct ipv6hdr *iph = skb_header_pointer(
			skb, offset, sizeof(_iph), &_iph);
		if (!iph)
			return 0;

		iphdr->saddr.in6 = iph->saddr;
		iphdr->daddr.in6 = iph->daddr;
		
		iphdr->len	 = offset;
		iphdr->flags	 = 0;
		iphdr->protocol  = ipv6_find_hdr(skb, &iphdr->len, -1,
						 &iphdr->fragoffs,
						 &iphdr->flags);
		if (iphdr->protocol < 0)
			return 0;
	} else
#endif
	{
		struct iphdr _iph;
		const struct iphdr *iph = skb_header_pointer(
			skb, offset, sizeof(_iph), &_iph);
		if (!iph)
			return 0;

		iphdr->len	= offset + iph->ihl * 4;
		iphdr->fragoffs	= 0;
		iphdr->protocol	= iph->protocol;
		iphdr->saddr.ip	= iph->saddr;
		iphdr->daddr.ip	= iph->daddr;
	}

	return 1;
}

static inline int
ip_vs_fill_iph_skb_icmp(int af, const struct sk_buff *skb, int offset,
			bool inverse, struct ip_vs_iphdr *iphdr)
{
	int hdr_flags = IP_VS_HDR_ICMP;

	if (inverse)
		hdr_flags |= IP_VS_HDR_INVERSE;

	return ip_vs_fill_iph_skb_off(af, skb, offset, hdr_flags, iphdr);
}

static inline int
ip_vs_fill_iph_skb(int af, const struct sk_buff *skb, bool inverse,
		   struct ip_vs_iphdr *iphdr)
{
	int hdr_flags = 0;

	if (inverse)
		hdr_flags |= IP_VS_HDR_INVERSE;

	return ip_vs_fill_iph_skb_off(af, skb, skb_network_offset(skb),
				      hdr_flags, iphdr);
}

static inline bool
ip_vs_iph_inverse(const struct ip_vs_iphdr *iph)
{
	return !!(iph->hdr_flags & IP_VS_HDR_INVERSE);
}

static inline bool
ip_vs_iph_icmp(const struct ip_vs_iphdr *iph)
{
	return !!(iph->hdr_flags & IP_VS_HDR_ICMP);
}

static inline void ip_vs_addr_copy(int af, union nf_inet_addr *dst,
				   const union nf_inet_addr *src)
{
#ifdef CONFIG_IP_VS_IPV6
	if (af == AF_INET6)
		dst->in6 = src->in6;
	else
#endif
	dst->ip = src->ip;
}

static inline void ip_vs_addr_set(int af, union nf_inet_addr *dst,
				  const union nf_inet_addr *src)
{
#ifdef CONFIG_IP_VS_IPV6
	if (af == AF_INET6) {
		dst->in6 = src->in6;
		return;
	}
#endif
	dst->ip = src->ip;
	dst->all[1] = 0;
	dst->all[2] = 0;
	dst->all[3] = 0;
}

static inline int ip_vs_addr_equal(int af, const union nf_inet_addr *a,
				   const union nf_inet_addr *b)
{
#ifdef CONFIG_IP_VS_IPV6
	if (af == AF_INET6)
		return ipv6_addr_equal(&a->in6, &b->in6);
#endif
	return a->ip == b->ip;
}

#ifdef CONFIG_IP_VS_DEBUG
#include <linux/net.h>

int ip_vs_get_debug_level(void);

static inline const char *ip_vs_dbg_addr(int af, char *buf, size_t buf_len,
					 const union nf_inet_addr *addr,
					 int *idx)
{
	int len;
#ifdef CONFIG_IP_VS_IPV6
	if (af == AF_INET6)
		len = snprintf(&buf[*idx], buf_len - *idx, "[%pI6c]",
			       &addr->in6) + 1;
	else
#endif
		len = snprintf(&buf[*idx], buf_len - *idx, "%pI4",
			       &addr->ip) + 1;

	*idx += len;
	BUG_ON(*idx > buf_len + 1);
	return &buf[*idx - len];
}

#define IP_VS_DBG_BUF(level, msg, ...)					\
	do {								\
		char ip_vs_dbg_buf[160];				\
		int ip_vs_dbg_idx = 0;					\
		if (level <= ip_vs_get_debug_level())			\
			printk(KERN_DEBUG pr_fmt(msg), ##__VA_ARGS__);	\
	} while (0)
#define IP_VS_ERR_BUF(msg...)						\
	do {								\
		char ip_vs_dbg_buf[160];				\
		int ip_vs_dbg_idx = 0;					\
		pr_err(msg);						\
	} while (0)


#define IP_VS_DBG_ADDR(af, addr)					\
	ip_vs_dbg_addr(af, ip_vs_dbg_buf,				\
		       sizeof(ip_vs_dbg_buf), addr,			\
		       &ip_vs_dbg_idx)

#define IP_VS_DBG(level, msg, ...)					\
	do {								\
		if (level <= ip_vs_get_debug_level())			\
			printk(KERN_DEBUG pr_fmt(msg), ##__VA_ARGS__);	\
	} while (0)
#define IP_VS_DBG_RL(msg, ...)						\
	do {								\
		if (net_ratelimit())					\
			printk(KERN_DEBUG pr_fmt(msg), ##__VA_ARGS__);	\
	} while (0)
#define IP_VS_DBG_PKT(level, af, pp, skb, ofs, msg)			\
	do {								\
		if (level <= ip_vs_get_debug_level())			\
			pp->debug_packet(af, pp, skb, ofs, msg);	\
	} while (0)
#define IP_VS_DBG_RL_PKT(level, af, pp, skb, ofs, msg)			\
	do {								\
		if (level <= ip_vs_get_debug_level() &&			\
		    net_ratelimit())					\
			pp->debug_packet(af, pp, skb, ofs, msg);	\
	} while (0)
#else	
#define IP_VS_DBG_BUF(level, msg...)  do {} while (0)
#define IP_VS_ERR_BUF(msg...)  do {} while (0)
#define IP_VS_DBG(level, msg...)  do {} while (0)
#define IP_VS_DBG_RL(msg...)  do {} while (0)
#define IP_VS_DBG_PKT(level, af, pp, skb, ofs, msg)	do {} while (0)
#define IP_VS_DBG_RL_PKT(level, af, pp, skb, ofs, msg)	do {} while (0)
#endif

#define IP_VS_BUG() BUG()
#define IP_VS_ERR_RL(msg, ...)						\
	do {								\
		if (net_ratelimit())					\
			pr_err(msg, ##__VA_ARGS__);			\
	} while (0)

#ifdef CONFIG_IP_VS_DEBUG
#define EnterFunction(level)						\
	do {								\
		if (level <= ip_vs_get_debug_level())			\
			printk(KERN_DEBUG				\
			       pr_fmt("Enter: %s, %s line %i\n"),	\
			       __func__, __FILE__, __LINE__);		\
	} while (0)
#define LeaveFunction(level)						\
	do {								\
		if (level <= ip_vs_get_debug_level())			\
			printk(KERN_DEBUG				\
			       pr_fmt("Leave: %s, %s line %i\n"),	\
			       __func__, __FILE__, __LINE__);		\
	} while (0)
#else
#define EnterFunction(level)   do {} while (0)
#define LeaveFunction(level)   do {} while (0)
#endif


#define FTPPORT  cpu_to_be16(21)
#define FTPDATA  cpu_to_be16(20)


enum {
	IP_VS_TCP_S_NONE = 0,
	IP_VS_TCP_S_ESTABLISHED,
	IP_VS_TCP_S_SYN_SENT,
	IP_VS_TCP_S_SYN_RECV,
	IP_VS_TCP_S_FIN_WAIT,
	IP_VS_TCP_S_TIME_WAIT,
	IP_VS_TCP_S_CLOSE,
	IP_VS_TCP_S_CLOSE_WAIT,
	IP_VS_TCP_S_LAST_ACK,
	IP_VS_TCP_S_LISTEN,
	IP_VS_TCP_S_SYNACK,
	IP_VS_TCP_S_LAST
};


enum {
	IP_VS_UDP_S_NORMAL,
	IP_VS_UDP_S_LAST,
};


enum {
	IP_VS_ICMP_S_NORMAL,
	IP_VS_ICMP_S_LAST,
};


enum ip_vs_sctp_states {
	IP_VS_SCTP_S_NONE,
	IP_VS_SCTP_S_INIT1,
	IP_VS_SCTP_S_INIT,
	IP_VS_SCTP_S_COOKIE_SENT,
	IP_VS_SCTP_S_COOKIE_REPLIED,
	IP_VS_SCTP_S_COOKIE_WAIT,
	IP_VS_SCTP_S_COOKIE,
	IP_VS_SCTP_S_COOKIE_ECHOED,
	IP_VS_SCTP_S_ESTABLISHED,
	IP_VS_SCTP_S_SHUTDOWN_SENT,
	IP_VS_SCTP_S_SHUTDOWN_RECEIVED,
	IP_VS_SCTP_S_SHUTDOWN_ACK_SENT,
	IP_VS_SCTP_S_REJECTED,
	IP_VS_SCTP_S_CLOSED,
	IP_VS_SCTP_S_LAST
};


struct ip_vs_seq {
	__u32			init_seq;	
	__u32			delta;		
	__u32			previous_delta;	
};


struct ip_vs_counters {
	__u64		conns;		
	__u64		inpkts;		
	__u64		outpkts;	
	__u64		inbytes;	
	__u64		outbytes;	
};

struct ip_vs_cpu_stats {
	struct ip_vs_counters   cnt;
	struct u64_stats_sync   syncp;
};


struct ip_vs_estimator {
	struct list_head	list;

	u64			last_inbytes;
	u64			last_outbytes;
	u64			last_conns;
	u64			last_inpkts;
	u64			last_outpkts;

	u64			cps;
	u64			inpps;
	u64			outpps;
	u64			inbps;
	u64			outbps;
};


struct ip_vs_kstats {
	u64			conns;		
	u64			inpkts;		
	u64			outpkts;	
	u64			inbytes;	
	u64			outbytes;	

	u64			cps;		
	u64			inpps;		
	u64			outpps;		
	u64			inbps;		
	u64			outbps;		
};

struct ip_vs_stats {
	struct ip_vs_kstats	kstats;		
	struct ip_vs_estimator	est;		
	struct ip_vs_cpu_stats __percpu	*cpustats;	
	spinlock_t		lock;		
	struct ip_vs_kstats	kstats0;	
};

struct dst_entry;
struct iphdr;
struct ip_vs_conn;
struct ip_vs_app;
struct sk_buff;
struct ip_vs_proto_data;

struct ip_vs_protocol {
	struct ip_vs_protocol	*next;
	char			*name;
	u16			protocol;
	u16			num_states;
	int			dont_defrag;

	void (*init)(struct ip_vs_protocol *pp);

	void (*exit)(struct ip_vs_protocol *pp);

	int (*init_netns)(struct netns_ipvs *ipvs, struct ip_vs_proto_data *pd);

	void (*exit_netns)(struct netns_ipvs *ipvs, struct ip_vs_proto_data *pd);

	int (*conn_schedule)(struct netns_ipvs *ipvs,
			     int af, struct sk_buff *skb,
			     struct ip_vs_proto_data *pd,
			     int *verdict, struct ip_vs_conn **cpp,
			     struct ip_vs_iphdr *iph);

	struct ip_vs_conn *
	(*conn_in_get)(struct netns_ipvs *ipvs,
		       int af,
		       const struct sk_buff *skb,
		       const struct ip_vs_iphdr *iph);

	struct ip_vs_conn *
	(*conn_out_get)(struct netns_ipvs *ipvs,
			int af,
			const struct sk_buff *skb,
			const struct ip_vs_iphdr *iph);

	int (*snat_handler)(struct sk_buff *skb, struct ip_vs_protocol *pp,
			    struct ip_vs_conn *cp, struct ip_vs_iphdr *iph);

	int (*dnat_handler)(struct sk_buff *skb, struct ip_vs_protocol *pp,
			    struct ip_vs_conn *cp, struct ip_vs_iphdr *iph);

	int (*csum_check)(int af, struct sk_buff *skb,
			  struct ip_vs_protocol *pp);

	const char *(*state_name)(int state);

	void (*state_transition)(struct ip_vs_conn *cp, int direction,
				 const struct sk_buff *skb,
				 struct ip_vs_proto_data *pd);

	int (*register_app)(struct netns_ipvs *ipvs, struct ip_vs_app *inc);

	void (*unregister_app)(struct netns_ipvs *ipvs, struct ip_vs_app *inc);

	int (*app_conn_bind)(struct ip_vs_conn *cp);

	void (*debug_packet)(int af, struct ip_vs_protocol *pp,
			     const struct sk_buff *skb,
			     int offset,
			     const char *msg);

	void (*timeout_change)(struct ip_vs_proto_data *pd, int flags);
};


struct ip_vs_proto_data {
	struct ip_vs_proto_data	*next;
	struct ip_vs_protocol	*pp;
	int			*timeout_table;	
	atomic_t		appcnt;		
	struct tcp_states_t	*tcp_state_table;
};

struct ip_vs_protocol   *ip_vs_proto_get(unsigned short proto);
struct ip_vs_proto_data *ip_vs_proto_data_get(struct netns_ipvs *ipvs,
					      unsigned short proto);

struct ip_vs_conn_param {
	struct netns_ipvs		*ipvs;
	const union nf_inet_addr	*caddr;
	const union nf_inet_addr	*vaddr;
	__be16				cport;
	__be16				vport;
	__u16				protocol;
	u16				af;

	const struct ip_vs_pe		*pe;
	char				*pe_data;
	__u8				pe_data_len;
};


struct ip_vs_conn {
	struct hlist_node	c_list;         
	
	__be16                  cport;
	__be16                  dport;
	__be16                  vport;
	u16			af;		
	union nf_inet_addr      caddr;          
	union nf_inet_addr      vaddr;          
	union nf_inet_addr      daddr;          
	volatile __u32          flags;          
	__u16                   protocol;       
	__u16			daf;		
	struct netns_ipvs	*ipvs;

	
	atomic_t		refcnt;		
	struct timer_list	timer;		
	volatile unsigned long	timeout;	

	
	spinlock_t              lock;           
	volatile __u16          state;          
	volatile __u16          old_state;      
	__u32			fwmark;		
	unsigned long		sync_endtime;	

	
	struct ip_vs_conn       *control;       
	atomic_t                n_control;      
	struct ip_vs_dest       *dest;          
	atomic_t                in_pkts;        

	
	int (*packet_xmit)(struct sk_buff *skb, struct ip_vs_conn *cp,
			   struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);

	
	struct ip_vs_app        *app;           
	void                    *app_data;      
	struct ip_vs_seq        in_seq;         
	struct ip_vs_seq        out_seq;        

	const struct ip_vs_pe	*pe;
	char			*pe_data;
	__u8			pe_data_len;

	struct rcu_head		rcu_head;
};


struct ip_vs_service_user_kern {
	
	u16			af;
	u16			protocol;
	union nf_inet_addr	addr;		
	__be16			port;
	u32			fwmark;		

	
	char			*sched_name;
	char			*pe_name;
	unsigned int		flags;		
	unsigned int		timeout;	
	__be32			netmask;	
};


struct ip_vs_dest_user_kern {
	
	union nf_inet_addr	addr;
	__be16			port;

	
	unsigned int		conn_flags;	
	int			weight;		

	
	u32			u_threshold;	
	u32			l_threshold;	

	
	u16			af;
};



struct ip_vs_service {
	struct hlist_node	s_list;   
	struct hlist_node	f_list;   
	atomic_t		refcnt;   

	u16			af;       
	__u16			protocol; 
	union nf_inet_addr	addr;	  
	__be16			port;	  
	__u32                   fwmark;   
	unsigned int		flags;	  
	unsigned int		timeout;  
	__be32			netmask;  
	struct netns_ipvs	*ipvs;

	struct list_head	destinations;  
	__u32			num_dests;     
	struct ip_vs_stats      stats;         

	
	struct ip_vs_scheduler __rcu *scheduler; 
	spinlock_t		sched_lock;    
	void			*sched_data;   

	
	struct ip_vs_pe __rcu	*pe;

	struct rcu_head		rcu_head;
};


struct ip_vs_dest_dst {
	struct dst_entry	*dst_cache;	
	u32			dst_cookie;
	union nf_inet_addr	dst_saddr;
	struct rcu_head		rcu_head;
};


struct ip_vs_dest {
	struct list_head	n_list;   
	struct hlist_node	d_list;   

	u16			af;		
	__be16			port;		
	union nf_inet_addr	addr;		
	volatile unsigned int	flags;		
	atomic_t		conn_flags;	
	atomic_t		weight;		

	atomic_t		refcnt;		
	struct ip_vs_stats      stats;          
	unsigned long		idle_start;	

	
	atomic_t		activeconns;	
	atomic_t		inactconns;	
	atomic_t		persistconns;	
	__u32			u_threshold;	
	__u32			l_threshold;	

	
	spinlock_t		dst_lock;	
	struct ip_vs_dest_dst __rcu *dest_dst;	

	
	struct ip_vs_service __rcu *svc;	
	__u16			protocol;	
	__be16			vport;		
	union nf_inet_addr	vaddr;		
	__u32			vfwmark;	

	struct list_head	t_list;		
	unsigned int		in_rs_table:1;	
};


struct ip_vs_scheduler {
	struct list_head	n_list;		
	char			*name;		
	atomic_t		refcnt;		
	struct module		*module;	

	
	int (*init_service)(struct ip_vs_service *svc);
	
	void (*done_service)(struct ip_vs_service *svc);
	
	int (*add_dest)(struct ip_vs_service *svc, struct ip_vs_dest *dest);
	
	int (*del_dest)(struct ip_vs_service *svc, struct ip_vs_dest *dest);
	
	int (*upd_dest)(struct ip_vs_service *svc, struct ip_vs_dest *dest);

	
	struct ip_vs_dest* (*schedule)(struct ip_vs_service *svc,
				       const struct sk_buff *skb,
				       struct ip_vs_iphdr *iph);
};


struct ip_vs_pe {
	struct list_head	n_list;		
	char			*name;		
	atomic_t		refcnt;		
	struct module		*module;	

	
	int (*fill_param)(struct ip_vs_conn_param *p, struct sk_buff *skb);
	bool (*ct_match)(const struct ip_vs_conn_param *p,
			 struct ip_vs_conn *ct);
	u32 (*hashkey_raw)(const struct ip_vs_conn_param *p, u32 initval,
			   bool inverse);
	int (*show_pe_data)(const struct ip_vs_conn *cp, char *buf);
	
	struct ip_vs_conn* (*conn_out)(struct ip_vs_service *svc,
				       struct ip_vs_dest *dest,
				       struct sk_buff *skb,
				       const struct ip_vs_iphdr *iph,
				       __be16 dport, __be16 cport);
};


struct ip_vs_app {
	struct list_head	a_list;		
	int			type;		
	char			*name;		
	__u16			protocol;
	struct module		*module;	
	struct list_head	incs_list;	

	
	struct list_head	p_list;		
	struct ip_vs_app	*app;		
	__be16			port;		
	atomic_t		usecnt;		
	struct rcu_head		rcu_head;

	
	int (*pkt_out)(struct ip_vs_app *, struct ip_vs_conn *,
		       struct sk_buff *, int *diff);

	
	int (*pkt_in)(struct ip_vs_app *, struct ip_vs_conn *,
		      struct sk_buff *, int *diff);

	
	int (*init_conn)(struct ip_vs_app *, struct ip_vs_conn *);

	
	int (*done_conn)(struct ip_vs_app *, struct ip_vs_conn *);


	
	int (*bind_conn)(struct ip_vs_app *, struct ip_vs_conn *,
			 struct ip_vs_protocol *);

	void (*unbind_conn)(struct ip_vs_app *, struct ip_vs_conn *);

	int *			timeout_table;
	int *			timeouts;
	int			timeouts_size;

	int (*conn_schedule)(struct sk_buff *skb, struct ip_vs_app *app,
			     int *verdict, struct ip_vs_conn **cpp);

	struct ip_vs_conn *
	(*conn_in_get)(const struct sk_buff *skb, struct ip_vs_app *app,
		       const struct iphdr *iph, int inverse);

	struct ip_vs_conn *
	(*conn_out_get)(const struct sk_buff *skb, struct ip_vs_app *app,
			const struct iphdr *iph, int inverse);

	int (*state_transition)(struct ip_vs_conn *cp, int direction,
				const struct sk_buff *skb,
				struct ip_vs_app *app);

	void (*timeout_change)(struct ip_vs_app *app, int flags);
};

struct ipvs_master_sync_state {
	struct list_head	sync_queue;
	struct ip_vs_sync_buff	*sync_buff;
	unsigned long		sync_queue_len;
	unsigned int		sync_queue_delay;
	struct task_struct	*master_thread;
	struct delayed_work	master_wakeup_work;
	struct netns_ipvs	*ipvs;
};


#define IP_VS_DEST_TRASH_PERIOD		(120 * HZ)

struct ipvs_sync_daemon_cfg {
	union nf_inet_addr	mcast_group;
	int			syncid;
	u16			sync_maxlen;
	u16			mcast_port;
	u8			mcast_af;
	u8			mcast_ttl;
	
	char			mcast_ifn[IP_VS_IFNAME_MAXLEN];
};


struct netns_ipvs {
	int			gen;		
	int			enable;		
	
	#define IP_VS_RTAB_BITS 4
	#define IP_VS_RTAB_SIZE (1 << IP_VS_RTAB_BITS)
	#define IP_VS_RTAB_MASK (IP_VS_RTAB_SIZE - 1)

	struct hlist_head	rs_table[IP_VS_RTAB_SIZE];
	
	struct list_head	app_list;
	
	#define IP_VS_PROTO_TAB_SIZE	32	
	struct ip_vs_proto_data *proto_data_table[IP_VS_PROTO_TAB_SIZE];
	
#ifdef CONFIG_IP_VS_PROTO_TCP
	#define	TCP_APP_TAB_BITS	4
	#define	TCP_APP_TAB_SIZE	(1 << TCP_APP_TAB_BITS)
	#define	TCP_APP_TAB_MASK	(TCP_APP_TAB_SIZE - 1)
	struct list_head	tcp_apps[TCP_APP_TAB_SIZE];
#endif
	
#ifdef CONFIG_IP_VS_PROTO_UDP
	#define	UDP_APP_TAB_BITS	4
	#define	UDP_APP_TAB_SIZE	(1 << UDP_APP_TAB_BITS)
	#define	UDP_APP_TAB_MASK	(UDP_APP_TAB_SIZE - 1)
	struct list_head	udp_apps[UDP_APP_TAB_SIZE];
#endif
	
#ifdef CONFIG_IP_VS_PROTO_SCTP
	#define SCTP_APP_TAB_BITS	4
	#define SCTP_APP_TAB_SIZE	(1 << SCTP_APP_TAB_BITS)
	#define SCTP_APP_TAB_MASK	(SCTP_APP_TAB_SIZE - 1)
	
	struct list_head	sctp_apps[SCTP_APP_TAB_SIZE];
#endif
	
	atomic_t		conn_count;      

	
	struct ip_vs_stats		tot_stats;  

	int			num_services;    

	
	struct list_head	dest_trash;
	spinlock_t		dest_trash_lock;
	struct timer_list	dest_trash_timer; 
	
	atomic_t		ftpsvc_counter;
	atomic_t		nullsvc_counter;
	atomic_t		conn_out_counter;

#ifdef CONFIG_SYSCTL
	
	struct delayed_work	defense_work;   
	int			drop_rate;
	int			drop_counter;
	int			old_secure_tcp;
	atomic_t		dropentry;
	
	spinlock_t		dropentry_lock;  
	spinlock_t		droppacket_lock; 
	spinlock_t		securetcp_lock;  

	
	struct ctl_table_header	*sysctl_hdr;
	struct ctl_table	*sysctl_tbl;
#endif

	
	int			sysctl_amemthresh;
	int			sysctl_am_droprate;
	int			sysctl_drop_entry;
	int			sysctl_drop_packet;
	int			sysctl_secure_tcp;
#ifdef CONFIG_IP_VS_NFCT
	int			sysctl_conntrack;
#endif
	int			sysctl_snat_reroute;
	int			sysctl_sync_ver;
	int			sysctl_sync_ports;
	int			sysctl_sync_persist_mode;
	unsigned long		sysctl_sync_qlen_max;
	int			sysctl_sync_sock_size;
	int			sysctl_cache_bypass;
	int			sysctl_expire_nodest_conn;
	int			sysctl_sloppy_tcp;
	int			sysctl_sloppy_sctp;
	int			sysctl_expire_quiescent_template;
	int			sysctl_sync_threshold[2];
	unsigned int		sysctl_sync_refresh_period;
	int			sysctl_sync_retries;
	int			sysctl_nat_icmp_send;
	int			sysctl_pmtu_disc;
	int			sysctl_backup_only;
	int			sysctl_conn_reuse_mode;
	int			sysctl_schedule_icmp;
	int			sysctl_ignore_tunneled;

	
	int			sysctl_lblc_expiration;
	struct ctl_table_header	*lblc_ctl_header;
	struct ctl_table	*lblc_ctl_table;
	
	int			sysctl_lblcr_expiration;
	struct ctl_table_header	*lblcr_ctl_header;
	struct ctl_table	*lblcr_ctl_table;
	
	struct list_head	est_list;	
	spinlock_t		est_lock;
	struct timer_list	est_timer;	
	
	spinlock_t		sync_lock;
	struct ipvs_master_sync_state *ms;
	spinlock_t		sync_buff_lock;
	struct task_struct	**backup_threads;
	int			threads_mask;
	volatile int		sync_state;
	struct mutex		sync_mutex;
	struct ipvs_sync_daemon_cfg	mcfg;	
	struct ipvs_sync_daemon_cfg	bcfg;	
	
	struct net		*net;            
	
	unsigned int		mixed_address_family_dests;
};

#define DEFAULT_SYNC_THRESHOLD	3
#define DEFAULT_SYNC_PERIOD	50
#define DEFAULT_SYNC_VER	1
#define DEFAULT_SLOPPY_TCP	0
#define DEFAULT_SLOPPY_SCTP	0
#define DEFAULT_SYNC_REFRESH_PERIOD	(0U * HZ)
#define DEFAULT_SYNC_RETRIES		0
#define IPVS_SYNC_WAKEUP_RATE	8
#define IPVS_SYNC_QLEN_MAX	(IPVS_SYNC_WAKEUP_RATE * 4)
#define IPVS_SYNC_SEND_DELAY	(HZ / 50)
#define IPVS_SYNC_CHECK_PERIOD	HZ
#define IPVS_SYNC_FLUSH_TIME	(HZ * 2)
#define IPVS_SYNC_PORTS_MAX	(1 << 6)

#ifdef CONFIG_SYSCTL

static inline int sysctl_sync_threshold(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_threshold[0];
}

static inline int sysctl_sync_period(struct netns_ipvs *ipvs)
{
	return ACCESS_ONCE(ipvs->sysctl_sync_threshold[1]);
}

static inline unsigned int sysctl_sync_refresh_period(struct netns_ipvs *ipvs)
{
	return ACCESS_ONCE(ipvs->sysctl_sync_refresh_period);
}

static inline int sysctl_sync_retries(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_retries;
}

static inline int sysctl_sync_ver(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_ver;
}

static inline int sysctl_sloppy_tcp(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sloppy_tcp;
}

static inline int sysctl_sloppy_sctp(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sloppy_sctp;
}

static inline int sysctl_sync_ports(struct netns_ipvs *ipvs)
{
	return ACCESS_ONCE(ipvs->sysctl_sync_ports);
}

static inline int sysctl_sync_persist_mode(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_persist_mode;
}

static inline unsigned long sysctl_sync_qlen_max(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_qlen_max;
}

static inline int sysctl_sync_sock_size(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_sync_sock_size;
}

static inline int sysctl_pmtu_disc(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_pmtu_disc;
}

static inline int sysctl_backup_only(struct netns_ipvs *ipvs)
{
	return ipvs->sync_state & IP_VS_STATE_BACKUP &&
	       ipvs->sysctl_backup_only;
}

static inline int sysctl_conn_reuse_mode(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_conn_reuse_mode;
}

static inline int sysctl_schedule_icmp(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_schedule_icmp;
}

static inline int sysctl_ignore_tunneled(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_ignore_tunneled;
}

static inline int sysctl_cache_bypass(struct netns_ipvs *ipvs)
{
	return ipvs->sysctl_cache_bypass;
}

#else

static inline int sysctl_sync_threshold(struct netns_ipvs *ipvs)
{
	return DEFAULT_SYNC_THRESHOLD;
}

static inline int sysctl_sync_period(struct netns_ipvs *ipvs)
{
	return DEFAULT_SYNC_PERIOD;
}

static inline unsigned int sysctl_sync_refresh_period(struct netns_ipvs *ipvs)
{
	return DEFAULT_SYNC_REFRESH_PERIOD;
}

static inline int sysctl_sync_retries(struct netns_ipvs *ipvs)
{
	return DEFAULT_SYNC_RETRIES & 3;
}

static inline int sysctl_sync_ver(struct netns_ipvs *ipvs)
{
	return DEFAULT_SYNC_VER;
}

static inline int sysctl_sloppy_tcp(struct netns_ipvs *ipvs)
{
	return DEFAULT_SLOPPY_TCP;
}

static inline int sysctl_sloppy_sctp(struct netns_ipvs *ipvs)
{
	return DEFAULT_SLOPPY_SCTP;
}

static inline int sysctl_sync_ports(struct netns_ipvs *ipvs)
{
	return 1;
}

static inline int sysctl_sync_persist_mode(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline unsigned long sysctl_sync_qlen_max(struct netns_ipvs *ipvs)
{
	return IPVS_SYNC_QLEN_MAX;
}

static inline int sysctl_sync_sock_size(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline int sysctl_pmtu_disc(struct netns_ipvs *ipvs)
{
	return 1;
}

static inline int sysctl_backup_only(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline int sysctl_conn_reuse_mode(struct netns_ipvs *ipvs)
{
	return 1;
}

static inline int sysctl_schedule_icmp(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline int sysctl_ignore_tunneled(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline int sysctl_cache_bypass(struct netns_ipvs *ipvs)
{
	return 0;
}

#endif


const char *ip_vs_proto_name(unsigned int proto);
void ip_vs_init_hash_table(struct list_head *table, int rows);
struct ip_vs_conn *ip_vs_new_conn_out(struct ip_vs_service *svc,
				      struct ip_vs_dest *dest,
				      struct sk_buff *skb,
				      const struct ip_vs_iphdr *iph,
				      __be16 dport,
				      __be16 cport);
#define IP_VS_INIT_HASH_TABLE(t) ip_vs_init_hash_table((t), ARRAY_SIZE((t)))

#define IP_VS_APP_TYPE_FTP	1


enum {
	IP_VS_DIR_INPUT = 0,
	IP_VS_DIR_OUTPUT,
	IP_VS_DIR_INPUT_ONLY,
	IP_VS_DIR_LAST,
};

static inline void ip_vs_conn_fill_param(struct netns_ipvs *ipvs, int af, int protocol,
					 const union nf_inet_addr *caddr,
					 __be16 cport,
					 const union nf_inet_addr *vaddr,
					 __be16 vport,
					 struct ip_vs_conn_param *p)
{
	p->ipvs = ipvs;
	p->af = af;
	p->protocol = protocol;
	p->caddr = caddr;
	p->cport = cport;
	p->vaddr = vaddr;
	p->vport = vport;
	p->pe = NULL;
	p->pe_data = NULL;
}

struct ip_vs_conn *ip_vs_conn_in_get(const struct ip_vs_conn_param *p);
struct ip_vs_conn *ip_vs_ct_in_get(const struct ip_vs_conn_param *p);

struct ip_vs_conn * ip_vs_conn_in_get_proto(struct netns_ipvs *ipvs, int af,
					    const struct sk_buff *skb,
					    const struct ip_vs_iphdr *iph);

struct ip_vs_conn *ip_vs_conn_out_get(const struct ip_vs_conn_param *p);

struct ip_vs_conn * ip_vs_conn_out_get_proto(struct netns_ipvs *ipvs, int af,
					     const struct sk_buff *skb,
					     const struct ip_vs_iphdr *iph);


static inline bool __ip_vs_conn_get(struct ip_vs_conn *cp)
{
	return atomic_inc_not_zero(&cp->refcnt);
}


static inline void __ip_vs_conn_put(struct ip_vs_conn *cp)
{
	smp_mb__before_atomic();
	atomic_dec(&cp->refcnt);
}
void ip_vs_conn_put(struct ip_vs_conn *cp);
void ip_vs_conn_fill_cport(struct ip_vs_conn *cp, __be16 cport);

struct ip_vs_conn *ip_vs_conn_new(const struct ip_vs_conn_param *p, int dest_af,
				  const union nf_inet_addr *daddr,
				  __be16 dport, unsigned int flags,
				  struct ip_vs_dest *dest, __u32 fwmark);
void ip_vs_conn_expire_now(struct ip_vs_conn *cp);

const char *ip_vs_state_name(__u16 proto, int state);

void ip_vs_tcp_conn_listen(struct ip_vs_conn *cp);
int ip_vs_check_template(struct ip_vs_conn *ct, struct ip_vs_dest *cdest);
void ip_vs_random_dropentry(struct netns_ipvs *ipvs);
int ip_vs_conn_init(void);
void ip_vs_conn_cleanup(void);

static inline void ip_vs_control_del(struct ip_vs_conn *cp)
{
	struct ip_vs_conn *ctl_cp = cp->control;
	if (!ctl_cp) {
		IP_VS_ERR_BUF("request control DEL for uncontrolled: "
			      "%s:%d to %s:%d\n",
			      IP_VS_DBG_ADDR(cp->af, &cp->caddr),
			      ntohs(cp->cport),
			      IP_VS_DBG_ADDR(cp->af, &cp->vaddr),
			      ntohs(cp->vport));

		return;
	}

	IP_VS_DBG_BUF(7, "DELeting control for: "
		      "cp.dst=%s:%d ctl_cp.dst=%s:%d\n",
		      IP_VS_DBG_ADDR(cp->af, &cp->caddr),
		      ntohs(cp->cport),
		      IP_VS_DBG_ADDR(cp->af, &ctl_cp->caddr),
		      ntohs(ctl_cp->cport));

	cp->control = NULL;
	if (atomic_read(&ctl_cp->n_control) == 0) {
		IP_VS_ERR_BUF("BUG control DEL with n=0 : "
			      "%s:%d to %s:%d\n",
			      IP_VS_DBG_ADDR(cp->af, &cp->caddr),
			      ntohs(cp->cport),
			      IP_VS_DBG_ADDR(cp->af, &cp->vaddr),
			      ntohs(cp->vport));

		return;
	}
	atomic_dec(&ctl_cp->n_control);
}

static inline void
ip_vs_control_add(struct ip_vs_conn *cp, struct ip_vs_conn *ctl_cp)
{
	if (cp->control) {
		IP_VS_ERR_BUF("request control ADD for already controlled: "
			      "%s:%d to %s:%d\n",
			      IP_VS_DBG_ADDR(cp->af, &cp->caddr),
			      ntohs(cp->cport),
			      IP_VS_DBG_ADDR(cp->af, &cp->vaddr),
			      ntohs(cp->vport));

		ip_vs_control_del(cp);
	}

	IP_VS_DBG_BUF(7, "ADDing control for: "
		      "cp.dst=%s:%d ctl_cp.dst=%s:%d\n",
		      IP_VS_DBG_ADDR(cp->af, &cp->caddr),
		      ntohs(cp->cport),
		      IP_VS_DBG_ADDR(cp->af, &ctl_cp->caddr),
		      ntohs(ctl_cp->cport));

	cp->control = ctl_cp;
	atomic_inc(&ctl_cp->n_control);
}


int ip_vs_estimator_net_init(struct netns_ipvs *ipvs);
int ip_vs_control_net_init(struct netns_ipvs *ipvs);
int ip_vs_protocol_net_init(struct netns_ipvs *ipvs);
int ip_vs_app_net_init(struct netns_ipvs *ipvs);
int ip_vs_conn_net_init(struct netns_ipvs *ipvs);
int ip_vs_sync_net_init(struct netns_ipvs *ipvs);
void ip_vs_conn_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_app_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_protocol_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_control_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_estimator_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_sync_net_cleanup(struct netns_ipvs *ipvs);
void ip_vs_service_net_cleanup(struct netns_ipvs *ipvs);


#define IP_VS_APP_MAX_PORTS  8
struct ip_vs_app *register_ip_vs_app(struct netns_ipvs *ipvs, struct ip_vs_app *app);
void unregister_ip_vs_app(struct netns_ipvs *ipvs, struct ip_vs_app *app);
int ip_vs_bind_app(struct ip_vs_conn *cp, struct ip_vs_protocol *pp);
void ip_vs_unbind_app(struct ip_vs_conn *cp);
int register_ip_vs_app_inc(struct netns_ipvs *ipvs, struct ip_vs_app *app, __u16 proto,
			   __u16 port);
int ip_vs_app_inc_get(struct ip_vs_app *inc);
void ip_vs_app_inc_put(struct ip_vs_app *inc);

int ip_vs_app_pkt_out(struct ip_vs_conn *, struct sk_buff *skb);
int ip_vs_app_pkt_in(struct ip_vs_conn *, struct sk_buff *skb);

int register_ip_vs_pe(struct ip_vs_pe *pe);
int unregister_ip_vs_pe(struct ip_vs_pe *pe);
struct ip_vs_pe *ip_vs_pe_getbyname(const char *name);
struct ip_vs_pe *__ip_vs_pe_getbyname(const char *pe_name);


#define ip_vs_pe_get(pe)			\
	if (pe && pe->module)			\
		__module_get(pe->module);

#define ip_vs_pe_put(pe)			\
	if (pe && pe->module)			\
		module_put(pe->module);


int ip_vs_protocol_init(void);
void ip_vs_protocol_cleanup(void);
void ip_vs_protocol_timeout_change(struct netns_ipvs *ipvs, int flags);
int *ip_vs_create_timeout_table(int *table, int size);
int ip_vs_set_state_timeout(int *table, int num, const char *const *names,
			    const char *name, int to);
void ip_vs_tcpudp_debug_packet(int af, struct ip_vs_protocol *pp,
			       const struct sk_buff *skb, int offset,
			       const char *msg);

extern struct ip_vs_protocol ip_vs_protocol_tcp;
extern struct ip_vs_protocol ip_vs_protocol_udp;
extern struct ip_vs_protocol ip_vs_protocol_icmp;
extern struct ip_vs_protocol ip_vs_protocol_esp;
extern struct ip_vs_protocol ip_vs_protocol_ah;
extern struct ip_vs_protocol ip_vs_protocol_sctp;


int register_ip_vs_scheduler(struct ip_vs_scheduler *scheduler);
int unregister_ip_vs_scheduler(struct ip_vs_scheduler *scheduler);
int ip_vs_bind_scheduler(struct ip_vs_service *svc,
			 struct ip_vs_scheduler *scheduler);
void ip_vs_unbind_scheduler(struct ip_vs_service *svc,
			    struct ip_vs_scheduler *sched);
struct ip_vs_scheduler *ip_vs_scheduler_get(const char *sched_name);
void ip_vs_scheduler_put(struct ip_vs_scheduler *scheduler);
struct ip_vs_conn *
ip_vs_schedule(struct ip_vs_service *svc, struct sk_buff *skb,
	       struct ip_vs_proto_data *pd, int *ignored,
	       struct ip_vs_iphdr *iph);
int ip_vs_leave(struct ip_vs_service *svc, struct sk_buff *skb,
		struct ip_vs_proto_data *pd, struct ip_vs_iphdr *iph);

void ip_vs_scheduler_err(struct ip_vs_service *svc, const char *msg);


extern struct ip_vs_stats ip_vs_stats;
extern int sysctl_ip_vs_sync_ver;

struct ip_vs_service *
ip_vs_service_find(struct netns_ipvs *ipvs, int af, __u32 fwmark, __u16 protocol,
		  const union nf_inet_addr *vaddr, __be16 vport);

bool ip_vs_has_real_service(struct netns_ipvs *ipvs, int af, __u16 protocol,
			    const union nf_inet_addr *daddr, __be16 dport);

struct ip_vs_dest *
ip_vs_find_real_service(struct netns_ipvs *ipvs, int af, __u16 protocol,
			const union nf_inet_addr *daddr, __be16 dport);

int ip_vs_use_count_inc(void);
void ip_vs_use_count_dec(void);
int ip_vs_register_nl_ioctl(void);
void ip_vs_unregister_nl_ioctl(void);
int ip_vs_control_init(void);
void ip_vs_control_cleanup(void);
struct ip_vs_dest *
ip_vs_find_dest(struct netns_ipvs *ipvs, int svc_af, int dest_af,
		const union nf_inet_addr *daddr, __be16 dport,
		const union nf_inet_addr *vaddr, __be16 vport,
		__u16 protocol, __u32 fwmark, __u32 flags);
void ip_vs_try_bind_dest(struct ip_vs_conn *cp);

static inline void ip_vs_dest_hold(struct ip_vs_dest *dest)
{
	atomic_inc(&dest->refcnt);
}

static inline void ip_vs_dest_put(struct ip_vs_dest *dest)
{
	smp_mb__before_atomic();
	atomic_dec(&dest->refcnt);
}

static inline void ip_vs_dest_put_and_free(struct ip_vs_dest *dest)
{
	if (atomic_dec_return(&dest->refcnt) < 0)
		kfree(dest);
}


int start_sync_thread(struct netns_ipvs *ipvs, struct ipvs_sync_daemon_cfg *cfg,
		      int state);
int stop_sync_thread(struct netns_ipvs *ipvs, int state);
void ip_vs_sync_conn(struct netns_ipvs *ipvs, struct ip_vs_conn *cp, int pkts);


void ip_vs_start_estimator(struct netns_ipvs *ipvs, struct ip_vs_stats *stats);
void ip_vs_stop_estimator(struct netns_ipvs *ipvs, struct ip_vs_stats *stats);
void ip_vs_zero_estimator(struct ip_vs_stats *stats);
void ip_vs_read_estimator(struct ip_vs_kstats *dst, struct ip_vs_stats *stats);


int ip_vs_null_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		    struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_bypass_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		      struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_nat_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		   struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_tunnel_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		      struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_dr_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		  struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_icmp_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
		    struct ip_vs_protocol *pp, int offset,
		    unsigned int hooknum, struct ip_vs_iphdr *iph);
void ip_vs_dest_dst_rcu_free(struct rcu_head *head);

#ifdef CONFIG_IP_VS_IPV6
int ip_vs_bypass_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
			 struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_nat_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
		      struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_tunnel_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
			 struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_dr_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
		     struct ip_vs_protocol *pp, struct ip_vs_iphdr *iph);
int ip_vs_icmp_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
		       struct ip_vs_protocol *pp, int offset,
		       unsigned int hooknum, struct ip_vs_iphdr *iph);
#endif

#ifdef CONFIG_SYSCTL

static inline int ip_vs_todrop(struct netns_ipvs *ipvs)
{
	if (!ipvs->drop_rate)
		return 0;
	if (--ipvs->drop_counter > 0)
		return 0;
	ipvs->drop_counter = ipvs->drop_rate;
	return 1;
}
#else
static inline int ip_vs_todrop(struct netns_ipvs *ipvs) { return 0; }
#endif


#define IP_VS_FWD_METHOD(cp)  (cp->flags & IP_VS_CONN_F_FWD_MASK)

static inline char ip_vs_fwd_tag(struct ip_vs_conn *cp)
{
	char fwd;

	switch (IP_VS_FWD_METHOD(cp)) {
	case IP_VS_CONN_F_MASQ:
		fwd = 'M'; break;
	case IP_VS_CONN_F_LOCALNODE:
		fwd = 'L'; break;
	case IP_VS_CONN_F_TUNNEL:
		fwd = 'T'; break;
	case IP_VS_CONN_F_DROUTE:
		fwd = 'R'; break;
	case IP_VS_CONN_F_BYPASS:
		fwd = 'B'; break;
	default:
		fwd = '?'; break;
	}
	return fwd;
}

void ip_vs_nat_icmp(struct sk_buff *skb, struct ip_vs_protocol *pp,
		    struct ip_vs_conn *cp, int dir);

#ifdef CONFIG_IP_VS_IPV6
void ip_vs_nat_icmp_v6(struct sk_buff *skb, struct ip_vs_protocol *pp,
		       struct ip_vs_conn *cp, int dir);
#endif

__sum16 ip_vs_checksum_complete(struct sk_buff *skb, int offset);

static inline __wsum ip_vs_check_diff4(__be32 old, __be32 new, __wsum oldsum)
{
	__be32 diff[2] = { ~old, new };

	return csum_partial(diff, sizeof(diff), oldsum);
}

#ifdef CONFIG_IP_VS_IPV6
static inline __wsum ip_vs_check_diff16(const __be32 *old, const __be32 *new,
					__wsum oldsum)
{
	__be32 diff[8] = { ~old[3], ~old[2], ~old[1], ~old[0],
			    new[3],  new[2],  new[1],  new[0] };

	return csum_partial(diff, sizeof(diff), oldsum);
}
#endif

static inline __wsum ip_vs_check_diff2(__be16 old, __be16 new, __wsum oldsum)
{
	__be16 diff[2] = { ~old, new };

	return csum_partial(diff, sizeof(diff), oldsum);
}


static inline void ip_vs_notrack(struct sk_buff *skb)
{
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	enum ip_conntrack_info ctinfo;
	struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

	if (!ct || !nf_ct_is_untracked(ct)) {
		nf_conntrack_put(skb->nfct);
		skb->nfct = &nf_ct_untracked_get()->ct_general;
		skb->nfctinfo = IP_CT_NEW;
		nf_conntrack_get(skb->nfct);
	}
#endif
}

#ifdef CONFIG_IP_VS_NFCT

static inline int ip_vs_conntrack_enabled(struct netns_ipvs *ipvs)
{
#ifdef CONFIG_SYSCTL
	return ipvs->sysctl_conntrack;
#else
	return 0;
#endif
}

void ip_vs_update_conntrack(struct sk_buff *skb, struct ip_vs_conn *cp,
			    int outin);
int ip_vs_confirm_conntrack(struct sk_buff *skb);
void ip_vs_nfct_expect_related(struct sk_buff *skb, struct nf_conn *ct,
			       struct ip_vs_conn *cp, u_int8_t proto,
			       const __be16 port, int from_rs);
void ip_vs_conn_drop_conntrack(struct ip_vs_conn *cp);

#else

static inline int ip_vs_conntrack_enabled(struct netns_ipvs *ipvs)
{
	return 0;
}

static inline void ip_vs_update_conntrack(struct sk_buff *skb,
					  struct ip_vs_conn *cp, int outin)
{
}

static inline int ip_vs_confirm_conntrack(struct sk_buff *skb)
{
	return NF_ACCEPT;
}

static inline void ip_vs_conn_drop_conntrack(struct ip_vs_conn *cp)
{
}
#endif 


static inline bool ip_vs_conn_uses_conntrack(struct ip_vs_conn *cp,
					     struct sk_buff *skb)
{
#ifdef CONFIG_IP_VS_NFCT
	enum ip_conntrack_info ctinfo;
	struct nf_conn *ct;

	if (!(cp->flags & IP_VS_CONN_F_NFCT))
		return false;
	ct = nf_ct_get(skb, &ctinfo);
	if (ct && !nf_ct_is_untracked(ct))
		return true;
#endif
	return false;
}

static inline int
ip_vs_dest_conn_overhead(struct ip_vs_dest *dest)
{
	
	return (atomic_read(&dest->activeconns) << 8) +
		atomic_read(&dest->inactconns);
}

#endif	
