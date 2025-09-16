
#ifndef _LINUX_HIPPIDEVICE_H
#define _LINUX_HIPPIDEVICE_H

#include <linux/if_hippi.h>

#ifdef __KERNEL__

struct hippi_cb {
	__u32	ifield;
};

__be16 hippi_type_trans(struct sk_buff *skb, struct net_device *dev);
int hippi_change_mtu(struct net_device *dev, int new_mtu);
int hippi_mac_addr(struct net_device *dev, void *p);
int hippi_neigh_setup_dev(struct net_device *dev, struct neigh_parms *p);
struct net_device *alloc_hippi_dev(int sizeof_priv);
#endif

#endif	
