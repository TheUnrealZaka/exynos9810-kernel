

#ifndef _CAN_CORE_H
#define _CAN_CORE_H

#include <linux/can.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#define CAN_VERSION "20120528"


#define CAN_ABI_VERSION "9"

#define CAN_VERSION_STRING "rev " CAN_VERSION " abi " CAN_ABI_VERSION

#define DNAME(dev) ((dev) ? (dev)->name : "any")


struct can_proto {
	int type;
	int protocol;
	const struct proto_ops *ops;
	struct proto *prot;
};



extern int  can_proto_register(const struct can_proto *cp);
extern void can_proto_unregister(const struct can_proto *cp);

int can_rx_register(struct net_device *dev, canid_t can_id, canid_t mask,
		    void (*func)(struct sk_buff *, void *),
		    void *data, char *ident, struct sock *sk);

extern void can_rx_unregister(struct net_device *dev, canid_t can_id,
			      canid_t mask,
			      void (*func)(struct sk_buff *, void *),
			      void *data);

extern int can_send(struct sk_buff *skb, int loop);
extern int can_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);

#endif 
