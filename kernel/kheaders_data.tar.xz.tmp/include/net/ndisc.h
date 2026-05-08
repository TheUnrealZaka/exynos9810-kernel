#ifndef _NDISC_H
#define _NDISC_H



#define NDISC_ROUTER_SOLICITATION	133
#define NDISC_ROUTER_ADVERTISEMENT	134
#define NDISC_NEIGHBOUR_SOLICITATION	135
#define NDISC_NEIGHBOUR_ADVERTISEMENT	136
#define NDISC_REDIRECT			137


#define NDISC_NODETYPE_UNSPEC		0	
#define NDISC_NODETYPE_HOST		1	
#define NDISC_NODETYPE_NODEFAULT	2	
#define NDISC_NODETYPE_DEFAULT		3	



enum {
	__ND_OPT_PREFIX_INFO_END = 0,
	ND_OPT_SOURCE_LL_ADDR = 1,	
	ND_OPT_TARGET_LL_ADDR = 2,	
	ND_OPT_PREFIX_INFO = 3,		
	ND_OPT_REDIRECT_HDR = 4,	
	ND_OPT_MTU = 5,			
	__ND_OPT_ARRAY_MAX,
	ND_OPT_ROUTE_INFO = 24,		
	ND_OPT_RDNSS = 25,		
	ND_OPT_DNSSL = 31,		
	ND_OPT_6CO = 34,		
	ND_OPT_CAPTIVE_PORTAL = 37,	
	ND_OPT_PREF64 = 38,		
	__ND_OPT_MAX
};

#define MAX_RTR_SOLICITATION_DELAY	HZ

#define ND_REACHABLE_TIME		(30*HZ)
#define ND_RETRANS_TIMER		HZ

#include <linux/compiler.h>
#include <linux/icmpv6.h>
#include <linux/in6.h>
#include <linux/types.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/hash.h>

#include <net/neighbour.h>


#define ND_DEBUG 1

#define ND_PRINTK(val, level, fmt, ...)				\
do {								\
	if (val <= ND_DEBUG)					\
		net_##level##_ratelimited(fmt, ##__VA_ARGS__);	\
} while (0)

struct ctl_table;
struct inet6_dev;
struct net_device;
struct net_proto_family;
struct sk_buff;
struct prefix_info;

extern struct neigh_table nd_tbl;

struct nd_msg {
        struct icmp6hdr	icmph;
        struct in6_addr	target;
	__u8		opt[0];
};

struct rs_msg {
	struct icmp6hdr	icmph;
	__u8		opt[0];
};

struct ra_msg {
        struct icmp6hdr		icmph;
	__be32			reachable_time;
	__be32			retrans_timer;
};

struct rd_msg {
	struct icmp6hdr icmph;
	struct in6_addr	target;
	struct in6_addr	dest;
	__u8		opt[0];
};

struct nd_opt_hdr {
	__u8		nd_opt_type;
	__u8		nd_opt_len;
} __packed;


struct ndisc_options {
	struct nd_opt_hdr *nd_opt_array[__ND_OPT_ARRAY_MAX];
#ifdef CONFIG_IPV6_ROUTE_INFO
	struct nd_opt_hdr *nd_opts_ri;
	struct nd_opt_hdr *nd_opts_ri_end;
#endif
	struct nd_opt_hdr *nd_useropts;
	struct nd_opt_hdr *nd_useropts_end;
#if IS_ENABLED(CONFIG_IEEE802154_6LOWPAN)
	struct nd_opt_hdr *nd_802154_opt_array[ND_OPT_TARGET_LL_ADDR + 1];
#endif
};

#define nd_opts_src_lladdr		nd_opt_array[ND_OPT_SOURCE_LL_ADDR]
#define nd_opts_tgt_lladdr		nd_opt_array[ND_OPT_TARGET_LL_ADDR]
#define nd_opts_pi			nd_opt_array[ND_OPT_PREFIX_INFO]
#define nd_opts_pi_end			nd_opt_array[__ND_OPT_PREFIX_INFO_END]
#define nd_opts_rh			nd_opt_array[ND_OPT_REDIRECT_HDR]
#define nd_opts_mtu			nd_opt_array[ND_OPT_MTU]
#define nd_802154_opts_src_lladdr	nd_802154_opt_array[ND_OPT_SOURCE_LL_ADDR]
#define nd_802154_opts_tgt_lladdr	nd_802154_opt_array[ND_OPT_TARGET_LL_ADDR]

#define NDISC_OPT_SPACE(len) (((len)+2+7)&~7)

struct ndisc_options *ndisc_parse_options(const struct net_device *dev,
					  u8 *opt, int opt_len,
					  struct ndisc_options *ndopts);

void __ndisc_fill_addr_option(struct sk_buff *skb, int type, void *data,
			      int data_len, int pad);

#define NDISC_OPS_REDIRECT_DATA_SPACE	2


struct ndisc_ops {
	int	(*is_useropt)(u8 nd_opt_type);
	int	(*parse_options)(const struct net_device *dev,
				 struct nd_opt_hdr *nd_opt,
				 struct ndisc_options *ndopts);
	void	(*update)(const struct net_device *dev, struct neighbour *n,
			  u32 flags, u8 icmp6_type,
			  const struct ndisc_options *ndopts);
	int	(*opt_addr_space)(const struct net_device *dev, u8 icmp6_type,
				  struct neighbour *neigh, u8 *ha_buf,
				  u8 **ha);
	void	(*fill_addr_option)(const struct net_device *dev,
				    struct sk_buff *skb, u8 icmp6_type,
				    const u8 *ha);
	void	(*prefix_rcv_add_addr)(struct net *net, struct net_device *dev,
				       const struct prefix_info *pinfo,
				       struct inet6_dev *in6_dev,
				       struct in6_addr *addr,
				       int addr_type, u32 addr_flags,
				       bool sllao, bool tokenized,
				       __u32 valid_lft, u32 prefered_lft,
				       bool dev_addr_generated);
};

#if IS_ENABLED(CONFIG_IPV6)
static inline int ndisc_ops_is_useropt(const struct net_device *dev,
				       u8 nd_opt_type)
{
	if (dev->ndisc_ops && dev->ndisc_ops->is_useropt)
		return dev->ndisc_ops->is_useropt(nd_opt_type);
	else
		return 0;
}

static inline int ndisc_ops_parse_options(const struct net_device *dev,
					  struct nd_opt_hdr *nd_opt,
					  struct ndisc_options *ndopts)
{
	if (dev->ndisc_ops && dev->ndisc_ops->parse_options)
		return dev->ndisc_ops->parse_options(dev, nd_opt, ndopts);
	else
		return 0;
}

static inline void ndisc_ops_update(const struct net_device *dev,
					  struct neighbour *n, u32 flags,
					  u8 icmp6_type,
					  const struct ndisc_options *ndopts)
{
	if (dev->ndisc_ops && dev->ndisc_ops->update)
		dev->ndisc_ops->update(dev, n, flags, icmp6_type, ndopts);
}

static inline int ndisc_ops_opt_addr_space(const struct net_device *dev,
					   u8 icmp6_type)
{
	if (dev->ndisc_ops && dev->ndisc_ops->opt_addr_space &&
	    icmp6_type != NDISC_REDIRECT)
		return dev->ndisc_ops->opt_addr_space(dev, icmp6_type, NULL,
						      NULL, NULL);
	else
		return 0;
}

static inline int ndisc_ops_redirect_opt_addr_space(const struct net_device *dev,
						    struct neighbour *neigh,
						    u8 *ha_buf, u8 **ha)
{
	if (dev->ndisc_ops && dev->ndisc_ops->opt_addr_space)
		return dev->ndisc_ops->opt_addr_space(dev, NDISC_REDIRECT,
						      neigh, ha_buf, ha);
	else
		return 0;
}

static inline void ndisc_ops_fill_addr_option(const struct net_device *dev,
					      struct sk_buff *skb,
					      u8 icmp6_type)
{
	if (dev->ndisc_ops && dev->ndisc_ops->fill_addr_option &&
	    icmp6_type != NDISC_REDIRECT)
		dev->ndisc_ops->fill_addr_option(dev, skb, icmp6_type, NULL);
}

static inline void ndisc_ops_fill_redirect_addr_option(const struct net_device *dev,
						       struct sk_buff *skb,
						       const u8 *ha)
{
	if (dev->ndisc_ops && dev->ndisc_ops->fill_addr_option)
		dev->ndisc_ops->fill_addr_option(dev, skb, NDISC_REDIRECT, ha);
}

static inline void ndisc_ops_prefix_rcv_add_addr(struct net *net,
						 struct net_device *dev,
						 const struct prefix_info *pinfo,
						 struct inet6_dev *in6_dev,
						 struct in6_addr *addr,
						 int addr_type, u32 addr_flags,
						 bool sllao, bool tokenized,
						 __u32 valid_lft,
						 u32 prefered_lft,
						 bool dev_addr_generated)
{
	if (dev->ndisc_ops && dev->ndisc_ops->prefix_rcv_add_addr)
		dev->ndisc_ops->prefix_rcv_add_addr(net, dev, pinfo, in6_dev,
						    addr, addr_type,
						    addr_flags, sllao,
						    tokenized, valid_lft,
						    prefered_lft,
						    dev_addr_generated);
}
#endif


static inline int ndisc_addr_option_pad(unsigned short type)
{
	switch (type) {
	case ARPHRD_INFINIBAND: return 2;
	default:                return 0;
	}
}

static inline int __ndisc_opt_addr_space(unsigned char addr_len, int pad)
{
	return NDISC_OPT_SPACE(addr_len + pad);
}

#if IS_ENABLED(CONFIG_IPV6)
static inline int ndisc_opt_addr_space(struct net_device *dev, u8 icmp6_type)
{
	return __ndisc_opt_addr_space(dev->addr_len,
				      ndisc_addr_option_pad(dev->type)) +
		ndisc_ops_opt_addr_space(dev, icmp6_type);
}

static inline int ndisc_redirect_opt_addr_space(struct net_device *dev,
						struct neighbour *neigh,
						u8 *ops_data_buf,
						u8 **ops_data)
{
	return __ndisc_opt_addr_space(dev->addr_len,
				      ndisc_addr_option_pad(dev->type)) +
		ndisc_ops_redirect_opt_addr_space(dev, neigh, ops_data_buf,
						  ops_data);
}
#endif

static inline u8 *__ndisc_opt_addr_data(struct nd_opt_hdr *p,
					unsigned char addr_len, int prepad)
{
	u8 *lladdr = (u8 *)(p + 1);
	int lladdrlen = p->nd_opt_len << 3;
	if (lladdrlen != __ndisc_opt_addr_space(addr_len, prepad))
		return NULL;
	return lladdr + prepad;
}

static inline u8 *ndisc_opt_addr_data(struct nd_opt_hdr *p,
				      struct net_device *dev)
{
	return __ndisc_opt_addr_data(p, dev->addr_len,
				     ndisc_addr_option_pad(dev->type));
}

static inline u32 ndisc_hashfn(const void *pkey, const struct net_device *dev, __u32 *hash_rnd)
{
	const u32 *p32 = pkey;

	return (((p32[0] ^ hash32_ptr(dev)) * hash_rnd[0]) +
		(p32[1] * hash_rnd[1]) +
		(p32[2] * hash_rnd[2]) +
		(p32[3] * hash_rnd[3]));
}

static inline struct neighbour *__ipv6_neigh_lookup_noref(struct net_device *dev, const void *pkey)
{
	return ___neigh_lookup_noref(&nd_tbl, neigh_key_eq128, ndisc_hashfn, pkey, dev);
}

static inline struct neighbour *__ipv6_neigh_lookup(struct net_device *dev, const void *pkey)
{
	struct neighbour *n;

	rcu_read_lock_bh();
	n = __ipv6_neigh_lookup_noref(dev, pkey);
	if (n && !atomic_inc_not_zero(&n->refcnt))
		n = NULL;
	rcu_read_unlock_bh();

	return n;
}

int ndisc_init(void);
int ndisc_late_init(void);

void ndisc_late_cleanup(void);
void ndisc_cleanup(void);

int ndisc_rcv(struct sk_buff *skb);

void ndisc_send_ns(struct net_device *dev, const struct in6_addr *solicit,
		   const struct in6_addr *daddr, const struct in6_addr *saddr);

void ndisc_send_rs(struct net_device *dev,
		   const struct in6_addr *saddr, const struct in6_addr *daddr);
void ndisc_send_na(struct net_device *dev, const struct in6_addr *daddr,
		   const struct in6_addr *solicited_addr,
		   bool router, bool solicited, bool override, bool inc_opt);

void ndisc_send_redirect(struct sk_buff *skb, const struct in6_addr *target);

int ndisc_mc_map(const struct in6_addr *addr, char *buf, struct net_device *dev,
		 int dir);

void ndisc_update(const struct net_device *dev, struct neighbour *neigh,
		  const u8 *lladdr, u8 new, u32 flags, u8 icmp6_type,
		  struct ndisc_options *ndopts);


int igmp6_init(void);

void igmp6_cleanup(void);

int igmp6_event_query(struct sk_buff *skb);

int igmp6_event_report(struct sk_buff *skb);


#ifdef CONFIG_SYSCTL
int ndisc_ifinfo_sysctl_change(struct ctl_table *ctl, int write,
			       void __user *buffer, size_t *lenp, loff_t *ppos);
int ndisc_ifinfo_sysctl_strategy(struct ctl_table *ctl,
				 void __user *oldval, size_t __user *oldlenp,
				 void __user *newval, size_t newlen);
#endif

void inet6_ifinfo_notify(int event, struct inet6_dev *idev);

#endif
