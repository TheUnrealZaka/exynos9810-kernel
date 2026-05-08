#ifndef _NF_NAT_HELPER_H
#define _NF_NAT_HELPER_H


#include <net/netfilter/nf_conntrack.h>

struct sk_buff;


int __nf_nat_mangle_tcp_packet(struct sk_buff *skb, struct nf_conn *ct,
			       enum ip_conntrack_info ctinfo,
			       unsigned int protoff, unsigned int match_offset,
			       unsigned int match_len, const char *rep_buffer,
			       unsigned int rep_len, bool adjust);

static inline int nf_nat_mangle_tcp_packet(struct sk_buff *skb,
					   struct nf_conn *ct,
					   enum ip_conntrack_info ctinfo,
					   unsigned int protoff,
					   unsigned int match_offset,
					   unsigned int match_len,
					   const char *rep_buffer,
					   unsigned int rep_len)
{
	return __nf_nat_mangle_tcp_packet(skb, ct, ctinfo, protoff,
					  match_offset, match_len,
					  rep_buffer, rep_len, true);
}

int nf_nat_mangle_udp_packet(struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned int protoff, unsigned int match_offset,
			     unsigned int match_len, const char *rep_buffer,
			     unsigned int rep_len);


void nf_nat_follow_master(struct nf_conn *ct, struct nf_conntrack_expect *this);

#endif
