

#ifndef _NF_CONNTRACK_L3PROTO_H
#define _NF_CONNTRACK_L3PROTO_H
#include <linux/netlink.h>
#include <net/netlink.h>
#include <linux/seq_file.h>
#include <net/netfilter/nf_conntrack.h>

struct nf_conntrack_l3proto {
	
	u_int16_t l3proto;

	
	const char *name;

	
	bool (*pkt_to_tuple)(const struct sk_buff *skb, unsigned int nhoff,
			     struct nf_conntrack_tuple *tuple);

	
	bool (*invert_tuple)(struct nf_conntrack_tuple *inverse,
			     const struct nf_conntrack_tuple *orig);

	
	void (*print_tuple)(struct seq_file *s,
			    const struct nf_conntrack_tuple *);

	
	int (*get_l4proto)(const struct sk_buff *skb, unsigned int nhoff,
			   unsigned int *dataoff, u_int8_t *protonum);

	int (*tuple_to_nlattr)(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *t);

	
	int (*nlattr_tuple_size)(void);

	int (*nlattr_to_tuple)(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t);
	const struct nla_policy *nla_policy;

	size_t nla_size;

	
	int (*init_net)(struct net *net);

	
	struct module *me;
};

extern struct nf_conntrack_l3proto __rcu *nf_ct_l3protos[AF_MAX];


int nf_ct_l3proto_pernet_register(struct net *net,
				  struct nf_conntrack_l3proto *proto);
void nf_ct_l3proto_pernet_unregister(struct net *net,
				     struct nf_conntrack_l3proto *proto);


int nf_ct_l3proto_register(struct nf_conntrack_l3proto *proto);
void nf_ct_l3proto_unregister(struct nf_conntrack_l3proto *proto);

struct nf_conntrack_l3proto *nf_ct_l3proto_find_get(u_int16_t l3proto);


extern struct nf_conntrack_l3proto nf_conntrack_l3proto_generic;

static inline struct nf_conntrack_l3proto *
__nf_ct_l3proto_find(u_int16_t l3proto)
{
	if (unlikely(l3proto >= AF_MAX))
		return &nf_conntrack_l3proto_generic;
	return rcu_dereference(nf_ct_l3protos[l3proto]);
}

#endif 
