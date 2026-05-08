
#ifndef _LINUX_FDDIDEVICE_H
#define _LINUX_FDDIDEVICE_H

#include <linux/if_fddi.h>

#ifdef __KERNEL__
__be16 fddi_type_trans(struct sk_buff *skb, struct net_device *dev);
int fddi_change_mtu(struct net_device *dev, int new_mtu);
struct net_device *alloc_fddidev(int sizeof_priv);
#endif

#endif	
