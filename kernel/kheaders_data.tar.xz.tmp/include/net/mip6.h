

#ifndef _NET_MIP6_H
#define _NET_MIP6_H

#include <linux/skbuff.h>
#include <net/sock.h>


struct ip6_mh {
	__u8	ip6mh_proto;
	__u8	ip6mh_hdrlen;
	__u8	ip6mh_type;
	__u8	ip6mh_reserved;
	__u16	ip6mh_cksum;
	
	__u8	data[0];
} __packed;

#define IP6_MH_TYPE_BRR		0   
#define IP6_MH_TYPE_HOTI	1   
#define IP6_MH_TYPE_COTI	2   
#define IP6_MH_TYPE_HOT		3   
#define IP6_MH_TYPE_COT		4   
#define IP6_MH_TYPE_BU		5   
#define IP6_MH_TYPE_BACK	6   
#define IP6_MH_TYPE_BERROR	7   
#define IP6_MH_TYPE_MAX		IP6_MH_TYPE_BERROR

#endif
