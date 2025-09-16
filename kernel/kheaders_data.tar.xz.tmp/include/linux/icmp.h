
#ifndef _LINUX_ICMP_H
#define	_LINUX_ICMP_H

#include <linux/skbuff.h>
#include <uapi/linux/icmp.h>

static inline struct icmphdr *icmp_hdr(const struct sk_buff *skb)
{
	return (struct icmphdr *)skb_transport_header(skb);
}
#endif	
