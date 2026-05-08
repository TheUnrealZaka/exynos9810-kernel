#ifndef _NETFILTER_INGRESS_H_
#define _NETFILTER_INGRESS_H_

#include <linux/netfilter.h>
#include <linux/netdevice.h>

#ifdef CONFIG_NETFILTER_INGRESS
static inline bool nf_hook_ingress_active(const struct sk_buff *skb)
{
#ifdef HAVE_JUMP_LABEL
	if (!static_key_false(&nf_hooks_needed[NFPROTO_NETDEV][NF_NETDEV_INGRESS]))
		return false;
#endif
	return rcu_access_pointer(skb->dev->nf_hooks_ingress);
}


static inline int nf_hook_ingress(struct sk_buff *skb)
{
	struct nf_hook_entry *e = rcu_dereference(skb->dev->nf_hooks_ingress);
	struct nf_hook_state state;

	
	if (unlikely(!e))
		return 0;

	nf_hook_state_init(&state, e, NF_NETDEV_INGRESS, INT_MIN,
			   NFPROTO_NETDEV, skb->dev, NULL, NULL,
			   dev_net(skb->dev), NULL);
	return nf_hook_slow(skb, &state);
}

static inline void nf_hook_ingress_init(struct net_device *dev)
{
	RCU_INIT_POINTER(dev->nf_hooks_ingress, NULL);
}
#else 
static inline int nf_hook_ingress_active(struct sk_buff *skb)
{
	return 0;
}

static inline int nf_hook_ingress(struct sk_buff *skb)
{
	return 0;
}

static inline void nf_hook_ingress_init(struct net_device *dev) {}
#endif 
#endif 
