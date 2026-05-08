

#ifndef _CAN_SKB_H
#define _CAN_SKB_H

#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/can.h>
#include <net/sock.h>




struct can_skb_priv {
	int ifindex;
	int skbcnt;
	struct can_frame cf[0];
};

static inline struct can_skb_priv *can_skb_prv(struct sk_buff *skb)
{
	return (struct can_skb_priv *)(skb->head);
}

static inline void can_skb_reserve(struct sk_buff *skb)
{
	skb_reserve(skb, sizeof(struct can_skb_priv));
}

static inline void can_skb_set_owner(struct sk_buff *skb, struct sock *sk)
{
	
	if (sk && atomic_inc_not_zero(&sk->sk_refcnt)) {
		skb->destructor = sock_efree;
		skb->sk = sk;
	}
}


static inline struct sk_buff *can_create_echo_skb(struct sk_buff *skb)
{
	struct sk_buff *nskb;

	nskb = skb_clone(skb, GFP_ATOMIC);
	if (unlikely(!nskb)) {
		kfree_skb(skb);
		return NULL;
	}

	can_skb_set_owner(nskb, skb->sk);
	consume_skb(skb);
	return nskb;
}

#endif 
