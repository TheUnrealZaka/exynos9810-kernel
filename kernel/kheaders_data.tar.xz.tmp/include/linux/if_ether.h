
#ifndef _LINUX_IF_ETHER_H
#define _LINUX_IF_ETHER_H

#include <linux/skbuff.h>
#include <uapi/linux/if_ether.h>

static inline struct ethhdr *eth_hdr(const struct sk_buff *skb)
{
	return (struct ethhdr *)skb_mac_header(skb);
}


static inline struct ethhdr *skb_eth_hdr(const struct sk_buff *skb)
{
	return (struct ethhdr *)skb->data;
}

static inline struct ethhdr *inner_eth_hdr(const struct sk_buff *skb)
{
	return (struct ethhdr *)skb_inner_mac_header(skb);
}

int eth_header_parse(const struct sk_buff *skb, unsigned char *haddr);

extern ssize_t sysfs_format_mac(char *buf, const unsigned char *addr, int len);

#endif	
