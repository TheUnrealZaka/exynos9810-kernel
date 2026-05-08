
#ifndef _NF_NAT_L4PROTO_H
#define _NF_NAT_L4PROTO_H
#include <net/netfilter/nf_nat.h>
#include <linux/netfilter/nfnetlink_conntrack.h>

struct nf_nat_range;
struct nf_nat_l3proto;

struct nf_nat_l4proto {
	
	u8 l4proto;

	
	bool (*manip_pkt)(struct sk_buff *skb,
			  const struct nf_nat_l3proto *l3proto,
			  unsigned int iphdroff, unsigned int hdroff,
			  const struct nf_conntrack_tuple *tuple,
			  enum nf_nat_manip_type maniptype);

	
	bool (*in_range)(const struct nf_conntrack_tuple *tuple,
			 enum nf_nat_manip_type maniptype,
			 const union nf_conntrack_man_proto *min,
			 const union nf_conntrack_man_proto *max);

	
	void (*unique_tuple)(const struct nf_nat_l3proto *l3proto,
			     struct nf_conntrack_tuple *tuple,
			     const struct nf_nat_range *range,
			     enum nf_nat_manip_type maniptype,
			     const struct nf_conn *ct);

	int (*nlattr_to_range)(struct nlattr *tb[],
			       struct nf_nat_range *range);
};


int nf_nat_l4proto_register(u8 l3proto, const struct nf_nat_l4proto *l4proto);
void nf_nat_l4proto_unregister(u8 l3proto,
			       const struct nf_nat_l4proto *l4proto);

const struct nf_nat_l4proto *__nf_nat_l4proto_find(u8 l3proto, u8 l4proto);


extern const struct nf_nat_l4proto nf_nat_l4proto_tcp;
extern const struct nf_nat_l4proto nf_nat_l4proto_udp;
extern const struct nf_nat_l4proto nf_nat_l4proto_icmp;
extern const struct nf_nat_l4proto nf_nat_l4proto_icmpv6;
extern const struct nf_nat_l4proto nf_nat_l4proto_unknown;

bool nf_nat_l4proto_in_range(const struct nf_conntrack_tuple *tuple,
			     enum nf_nat_manip_type maniptype,
			     const union nf_conntrack_man_proto *min,
			     const union nf_conntrack_man_proto *max);

void nf_nat_l4proto_unique_tuple(const struct nf_nat_l3proto *l3proto,
				 struct nf_conntrack_tuple *tuple,
				 const struct nf_nat_range *range,
				 enum nf_nat_manip_type maniptype,
				 const struct nf_conn *ct);

int nf_nat_l4proto_nlattr_to_range(struct nlattr *tb[],
				   struct nf_nat_range *range);

#endif 
