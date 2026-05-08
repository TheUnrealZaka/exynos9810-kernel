
#ifndef _LINUX_IP_H
#define _LINUX_IP_H

#include <linux/skbuff.h>
#include <uapi/linux/ip.h>

static inline struct iphdr *ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_network_header(skb);
}

static inline struct iphdr *inner_ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_inner_network_header(skb);
}

static inline struct iphdr *ipip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_transport_header(skb);
}
#endif	
