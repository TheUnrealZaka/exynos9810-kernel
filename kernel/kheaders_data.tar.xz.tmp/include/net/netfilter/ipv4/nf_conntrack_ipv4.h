

#ifndef _NF_CONNTRACK_IPV4_H
#define _NF_CONNTRACK_IPV4_H


extern struct nf_conntrack_l3proto nf_conntrack_l3proto_ipv4;

extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_icmp;

int nf_conntrack_ipv4_compat_init(void);
void nf_conntrack_ipv4_compat_fini(void);

#endif 
