
#ifndef _LINUX_IF_ARP_H
#define _LINUX_IF_ARP_H

#include <linux/skbuff.h>
#include <uapi/linux/if_arp.h>

static inline struct arphdr *arp_hdr(const struct sk_buff *skb)
{
	return (struct arphdr *)skb_network_header(skb);
}

static inline int arp_hdr_len(struct net_device *dev)
{
	switch (dev->type) {
#if IS_ENABLED(CONFIG_FIREWIRE_NET)
	case ARPHRD_IEEE1394:
		
		return sizeof(struct arphdr) + dev->addr_len + sizeof(u32) * 2;
#endif
	default:
		
		return sizeof(struct arphdr) + (dev->addr_len + sizeof(u32)) * 2;
	}
}
#endif	
