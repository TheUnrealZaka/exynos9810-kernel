#ifndef _NF_CONNTRACK_SCTP_H
#define _NF_CONNTRACK_SCTP_H


#include <uapi/linux/netfilter/nf_conntrack_sctp.h>

struct ip_ct_sctp {
	enum sctp_conntrack state;

	__be32 vtag[IP_CT_DIR_MAX];
};

#endif 
