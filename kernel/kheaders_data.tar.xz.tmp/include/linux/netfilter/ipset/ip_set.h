
#ifndef _IP_SET_H
#define _IP_SET_H

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/stringify.h>
#include <linux/vmalloc.h>
#include <net/netlink.h>
#include <uapi/linux/netfilter/ipset/ip_set.h>

#define _IP_SET_MODULE_DESC(a, b, c)		\
	MODULE_DESCRIPTION(a " type of IP sets, revisions " b "-" c)
#define IP_SET_MODULE_DESC(a, b, c)		\
	_IP_SET_MODULE_DESC(a, __stringify(b), __stringify(c))


enum ip_set_feature {
	IPSET_TYPE_IP_FLAG = 0,
	IPSET_TYPE_IP = (1 << IPSET_TYPE_IP_FLAG),
	IPSET_TYPE_PORT_FLAG = 1,
	IPSET_TYPE_PORT = (1 << IPSET_TYPE_PORT_FLAG),
	IPSET_TYPE_MAC_FLAG = 2,
	IPSET_TYPE_MAC = (1 << IPSET_TYPE_MAC_FLAG),
	IPSET_TYPE_IP2_FLAG = 3,
	IPSET_TYPE_IP2 = (1 << IPSET_TYPE_IP2_FLAG),
	IPSET_TYPE_NAME_FLAG = 4,
	IPSET_TYPE_NAME = (1 << IPSET_TYPE_NAME_FLAG),
	IPSET_TYPE_IFACE_FLAG = 5,
	IPSET_TYPE_IFACE = (1 << IPSET_TYPE_IFACE_FLAG),
	IPSET_TYPE_MARK_FLAG = 6,
	IPSET_TYPE_MARK = (1 << IPSET_TYPE_MARK_FLAG),
	IPSET_TYPE_NOMATCH_FLAG = 7,
	IPSET_TYPE_NOMATCH = (1 << IPSET_TYPE_NOMATCH_FLAG),
	
	IPSET_DUMP_LAST_FLAG = 8,
	IPSET_DUMP_LAST = (1 << IPSET_DUMP_LAST_FLAG),
};


enum ip_set_extension {
	IPSET_EXT_BIT_TIMEOUT = 0,
	IPSET_EXT_TIMEOUT = (1 << IPSET_EXT_BIT_TIMEOUT),
	IPSET_EXT_BIT_COUNTER = 1,
	IPSET_EXT_COUNTER = (1 << IPSET_EXT_BIT_COUNTER),
	IPSET_EXT_BIT_COMMENT = 2,
	IPSET_EXT_COMMENT = (1 << IPSET_EXT_BIT_COMMENT),
	IPSET_EXT_BIT_SKBINFO = 3,
	IPSET_EXT_SKBINFO = (1 << IPSET_EXT_BIT_SKBINFO),
	
	IPSET_EXT_BIT_DESTROY = 7,
	IPSET_EXT_DESTROY = (1 << IPSET_EXT_BIT_DESTROY),
};

#define SET_WITH_TIMEOUT(s)	((s)->extensions & IPSET_EXT_TIMEOUT)
#define SET_WITH_COUNTER(s)	((s)->extensions & IPSET_EXT_COUNTER)
#define SET_WITH_COMMENT(s)	((s)->extensions & IPSET_EXT_COMMENT)
#define SET_WITH_SKBINFO(s)	((s)->extensions & IPSET_EXT_SKBINFO)
#define SET_WITH_FORCEADD(s)	((s)->flags & IPSET_CREATE_FLAG_FORCEADD)


enum ip_set_ext_id {
	IPSET_EXT_ID_COUNTER = 0,
	IPSET_EXT_ID_TIMEOUT,
	IPSET_EXT_ID_SKBINFO,
	IPSET_EXT_ID_COMMENT,
	IPSET_EXT_ID_MAX,
};


struct ip_set_ext_type {
	
	void (*destroy)(void *ext);
	enum ip_set_extension type;
	enum ipset_cadt_flags flag;
	
	u8 len;
	u8 align;
};

extern const struct ip_set_ext_type ip_set_extensions[];

struct ip_set_ext {
	u64 packets;
	u64 bytes;
	u32 timeout;
	u32 skbmark;
	u32 skbmarkmask;
	u32 skbprio;
	u16 skbqueue;
	char *comment;
};

struct ip_set_counter {
	atomic64_t bytes;
	atomic64_t packets;
};

struct ip_set_comment_rcu {
	struct rcu_head rcu;
	char str[0];
};

struct ip_set_comment {
	struct ip_set_comment_rcu __rcu *c;
};

struct ip_set_skbinfo {
	u32 skbmark;
	u32 skbmarkmask;
	u32 skbprio;
	u16 skbqueue;
};

struct ip_set;

#define ext_timeout(e, s)	\
((unsigned long *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_TIMEOUT]))
#define ext_counter(e, s)	\
((struct ip_set_counter *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_COUNTER]))
#define ext_comment(e, s)	\
((struct ip_set_comment *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_COMMENT]))
#define ext_skbinfo(e, s)	\
((struct ip_set_skbinfo *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_SKBINFO]))

typedef int (*ipset_adtfn)(struct ip_set *set, void *value,
			   const struct ip_set_ext *ext,
			   struct ip_set_ext *mext, u32 cmdflags);


struct ip_set_adt_opt {
	u8 family;		
	u8 dim;			
	u8 flags;		
	u32 cmdflags;		
	struct ip_set_ext ext;	
};


struct ip_set_type_variant {
	
	int (*kadt)(struct ip_set *set, const struct sk_buff *skb,
		    const struct xt_action_param *par,
		    enum ipset_adt adt, struct ip_set_adt_opt *opt);

	
	int (*uadt)(struct ip_set *set, struct nlattr *tb[],
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried);

	
	ipset_adtfn adt[IPSET_ADT_MAX];

	
	int (*resize)(struct ip_set *set, bool retried);
	
	void (*destroy)(struct ip_set *set);
	
	void (*flush)(struct ip_set *set);
	
	void (*expire)(struct ip_set *set);
	
	int (*head)(struct ip_set *set, struct sk_buff *skb);
	
	int (*list)(const struct ip_set *set, struct sk_buff *skb,
		    struct netlink_callback *cb);
	
	void (*uref)(struct ip_set *set, struct netlink_callback *cb,
		     bool start);

	
	bool (*same_set)(const struct ip_set *a, const struct ip_set *b);
};


struct ip_set_type {
	struct list_head list;

	
	char name[IPSET_MAXNAMELEN];
	
	u8 protocol;
	
	u8 dimension;
	
	u8 family;
	
	u8 revision_min, revision_max;
	
	u16 features;

	
	int (*create)(struct net *net, struct ip_set *set,
		      struct nlattr *tb[], u32 flags);

	
	const struct nla_policy create_policy[IPSET_ATTR_CREATE_MAX + 1];
	const struct nla_policy adt_policy[IPSET_ATTR_ADT_MAX + 1];

	
	struct module *me;
};


extern int ip_set_type_register(struct ip_set_type *set_type);
extern void ip_set_type_unregister(struct ip_set_type *set_type);


struct ip_set {
	
	char name[IPSET_MAXNAMELEN];
	
	spinlock_t lock;
	
	u32 ref;
	
	u32 ref_netlink;
	
	struct ip_set_type *type;
	
	const struct ip_set_type_variant *variant;
	
	u8 family;
	
	u8 revision;
	
	u8 extensions;
	
	u8 flags;
	
	u32 timeout;
	
	size_t dsize;
	
	size_t offset[IPSET_EXT_ID_MAX];
	
	void *data;
};

static inline void
ip_set_ext_destroy(struct ip_set *set, void *data)
{
	
	if (SET_WITH_COMMENT(set))
		ip_set_extensions[IPSET_EXT_ID_COMMENT].destroy(
			ext_comment(data, set));
}

static inline int
ip_set_put_flags(struct sk_buff *skb, struct ip_set *set)
{
	u32 cadt_flags = 0;

	if (SET_WITH_TIMEOUT(set))
		if (unlikely(nla_put_net32(skb, IPSET_ATTR_TIMEOUT,
					   htonl(set->timeout))))
			return -EMSGSIZE;
	if (SET_WITH_COUNTER(set))
		cadt_flags |= IPSET_FLAG_WITH_COUNTERS;
	if (SET_WITH_COMMENT(set))
		cadt_flags |= IPSET_FLAG_WITH_COMMENT;
	if (SET_WITH_SKBINFO(set))
		cadt_flags |= IPSET_FLAG_WITH_SKBINFO;
	if (SET_WITH_FORCEADD(set))
		cadt_flags |= IPSET_FLAG_WITH_FORCEADD;

	if (!cadt_flags)
		return 0;
	return nla_put_net32(skb, IPSET_ATTR_CADT_FLAGS, htonl(cadt_flags));
}

static inline void
ip_set_add_bytes(u64 bytes, struct ip_set_counter *counter)
{
	atomic64_add((long long)bytes, &(counter)->bytes);
}

static inline void
ip_set_add_packets(u64 packets, struct ip_set_counter *counter)
{
	atomic64_add((long long)packets, &(counter)->packets);
}

static inline u64
ip_set_get_bytes(const struct ip_set_counter *counter)
{
	return (u64)atomic64_read(&(counter)->bytes);
}

static inline u64
ip_set_get_packets(const struct ip_set_counter *counter)
{
	return (u64)atomic64_read(&(counter)->packets);
}

static inline void
ip_set_update_counter(struct ip_set_counter *counter,
		      const struct ip_set_ext *ext,
		      struct ip_set_ext *mext, u32 flags)
{
	if (ext->packets != ULLONG_MAX &&
	    !(flags & IPSET_FLAG_SKIP_COUNTER_UPDATE)) {
		ip_set_add_bytes(ext->bytes, counter);
		ip_set_add_packets(ext->packets, counter);
	}
	if (flags & IPSET_FLAG_MATCH_COUNTERS) {
		mext->packets = ip_set_get_packets(counter);
		mext->bytes = ip_set_get_bytes(counter);
	}
}

static inline void
ip_set_get_skbinfo(struct ip_set_skbinfo *skbinfo,
		      const struct ip_set_ext *ext,
		      struct ip_set_ext *mext, u32 flags)
{
		mext->skbmark = skbinfo->skbmark;
		mext->skbmarkmask = skbinfo->skbmarkmask;
		mext->skbprio = skbinfo->skbprio;
		mext->skbqueue = skbinfo->skbqueue;
}
static inline bool
ip_set_put_skbinfo(struct sk_buff *skb, struct ip_set_skbinfo *skbinfo)
{
	
	return ((skbinfo->skbmark || skbinfo->skbmarkmask) &&
		nla_put_net64(skb, IPSET_ATTR_SKBMARK,
			      cpu_to_be64((u64)skbinfo->skbmark << 32 |
					  skbinfo->skbmarkmask),
			      IPSET_ATTR_PAD)) ||
	       (skbinfo->skbprio &&
		nla_put_net32(skb, IPSET_ATTR_SKBPRIO,
			      cpu_to_be32(skbinfo->skbprio))) ||
	       (skbinfo->skbqueue &&
		nla_put_net16(skb, IPSET_ATTR_SKBQUEUE,
			     cpu_to_be16(skbinfo->skbqueue)));
}

static inline void
ip_set_init_skbinfo(struct ip_set_skbinfo *skbinfo,
		    const struct ip_set_ext *ext)
{
	skbinfo->skbmark = ext->skbmark;
	skbinfo->skbmarkmask = ext->skbmarkmask;
	skbinfo->skbprio = ext->skbprio;
	skbinfo->skbqueue = ext->skbqueue;
}

static inline bool
ip_set_put_counter(struct sk_buff *skb, struct ip_set_counter *counter)
{
	return nla_put_net64(skb, IPSET_ATTR_BYTES,
			     cpu_to_be64(ip_set_get_bytes(counter)),
			     IPSET_ATTR_PAD) ||
	       nla_put_net64(skb, IPSET_ATTR_PACKETS,
			     cpu_to_be64(ip_set_get_packets(counter)),
			     IPSET_ATTR_PAD);
}

static inline void
ip_set_init_counter(struct ip_set_counter *counter,
		    const struct ip_set_ext *ext)
{
	if (ext->bytes != ULLONG_MAX)
		atomic64_set(&(counter)->bytes, (long long)(ext->bytes));
	if (ext->packets != ULLONG_MAX)
		atomic64_set(&(counter)->packets, (long long)(ext->packets));
}


enum {
	IPSET_CB_NET = 0,	
	IPSET_CB_DUMP,		
	IPSET_CB_INDEX,		
	IPSET_CB_PRIVATE,	
	IPSET_CB_ARG0,		
	IPSET_CB_ARG1,
};


extern ip_set_id_t ip_set_get_byname(struct net *net,
				     const char *name, struct ip_set **set);
extern void ip_set_put_byindex(struct net *net, ip_set_id_t index);
extern const char *ip_set_name_byindex(struct net *net, ip_set_id_t index);
extern ip_set_id_t ip_set_nfnl_get_byindex(struct net *net, ip_set_id_t index);
extern void ip_set_nfnl_put(struct net *net, ip_set_id_t index);



extern int ip_set_add(ip_set_id_t id, const struct sk_buff *skb,
		      const struct xt_action_param *par,
		      struct ip_set_adt_opt *opt);
extern int ip_set_del(ip_set_id_t id, const struct sk_buff *skb,
		      const struct xt_action_param *par,
		      struct ip_set_adt_opt *opt);
extern int ip_set_test(ip_set_id_t id, const struct sk_buff *skb,
		       const struct xt_action_param *par,
		       struct ip_set_adt_opt *opt);


extern void *ip_set_alloc(size_t size);
extern void ip_set_free(void *members);
extern int ip_set_get_ipaddr4(struct nlattr *nla,  __be32 *ipaddr);
extern int ip_set_get_ipaddr6(struct nlattr *nla, union nf_inet_addr *ipaddr);
extern size_t ip_set_elem_len(struct ip_set *set, struct nlattr *tb[],
			      size_t len, size_t align);
extern int ip_set_get_extensions(struct ip_set *set, struct nlattr *tb[],
				 struct ip_set_ext *ext);

static inline int
ip_set_get_hostipaddr4(struct nlattr *nla, u32 *ipaddr)
{
	__be32 ip;
	int ret = ip_set_get_ipaddr4(nla, &ip);

	if (ret)
		return ret;
	*ipaddr = ntohl(ip);
	return 0;
}


static inline bool
ip_set_eexist(int ret, u32 flags)
{
	return ret == -IPSET_ERR_EXIST && (flags & IPSET_FLAG_EXIST);
}


static inline bool
ip_set_enomatch(int ret, u32 flags, enum ipset_adt adt, struct ip_set *set)
{
	return adt == IPSET_TEST &&
	       (set->type->features & IPSET_TYPE_NOMATCH) &&
	       ((flags >> 16) & IPSET_FLAG_NOMATCH) &&
	       (ret > 0 || ret == -ENOTEMPTY);
}


static inline bool
ip_set_attr_netorder(struct nlattr *tb[], int type)
{
	return tb[type] && (tb[type]->nla_type & NLA_F_NET_BYTEORDER);
}

static inline bool
ip_set_optattr_netorder(struct nlattr *tb[], int type)
{
	return !tb[type] || (tb[type]->nla_type & NLA_F_NET_BYTEORDER);
}


static inline u32
ip_set_get_h32(const struct nlattr *attr)
{
	return ntohl(nla_get_be32(attr));
}

static inline u16
ip_set_get_h16(const struct nlattr *attr)
{
	return ntohs(nla_get_be16(attr));
}

#define ipset_nest_start(skb, attr) nla_nest_start(skb, attr | NLA_F_NESTED)
#define ipset_nest_end(skb, start)  nla_nest_end(skb, start)

static inline int nla_put_ipaddr4(struct sk_buff *skb, int type, __be32 ipaddr)
{
	struct nlattr *__nested = ipset_nest_start(skb, type);
	int ret;

	if (!__nested)
		return -EMSGSIZE;
	ret = nla_put_in_addr(skb, IPSET_ATTR_IPADDR_IPV4, ipaddr);
	if (!ret)
		ipset_nest_end(skb, __nested);
	return ret;
}

static inline int nla_put_ipaddr6(struct sk_buff *skb, int type,
				  const struct in6_addr *ipaddrptr)
{
	struct nlattr *__nested = ipset_nest_start(skb, type);
	int ret;

	if (!__nested)
		return -EMSGSIZE;
	ret = nla_put_in6_addr(skb, IPSET_ATTR_IPADDR_IPV6, ipaddrptr);
	if (!ret)
		ipset_nest_end(skb, __nested);
	return ret;
}


static inline __be32
ip4addr(const struct sk_buff *skb, bool src)
{
	return src ? ip_hdr(skb)->saddr : ip_hdr(skb)->daddr;
}

static inline void
ip4addrptr(const struct sk_buff *skb, bool src, __be32 *addr)
{
	*addr = src ? ip_hdr(skb)->saddr : ip_hdr(skb)->daddr;
}

static inline void
ip6addrptr(const struct sk_buff *skb, bool src, struct in6_addr *addr)
{
	memcpy(addr, src ? &ipv6_hdr(skb)->saddr : &ipv6_hdr(skb)->daddr,
	       sizeof(*addr));
}

#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_comment.h>

int
ip_set_put_extensions(struct sk_buff *skb, const struct ip_set *set,
		      const void *e, bool active);

#define IP_SET_INIT_KEXT(skb, opt, set)			\
	{ .bytes = (skb)->len, .packets = 1,		\
	  .timeout = ip_set_adt_opt_timeout(opt, set) }

#define IP_SET_INIT_UEXT(set)				\
	{ .bytes = ULLONG_MAX, .packets = ULLONG_MAX,	\
	  .timeout = (set)->timeout }

#define IPSET_CONCAT(a, b)		a##b
#define IPSET_TOKEN(a, b)		IPSET_CONCAT(a, b)

#endif 
