#ifndef _RDMA_NETLINK_H
#define _RDMA_NETLINK_H


#include <linux/netlink.h>
#include <uapi/rdma/rdma_netlink.h>

struct ibnl_client_cbs {
	int (*dump)(struct sk_buff *skb, struct netlink_callback *nlcb);
	struct module *module;
};

int ibnl_init(void);
void ibnl_cleanup(void);


int ibnl_add_client(int index, int nops,
		    const struct ibnl_client_cbs cb_table[]);


int ibnl_remove_client(int index);


void *ibnl_put_msg(struct sk_buff *skb, struct nlmsghdr **nlh, int seq,
		   int len, int client, int op, int flags);

int ibnl_put_attr(struct sk_buff *skb, struct nlmsghdr *nlh,
		  int len, void *data, int type);


int ibnl_unicast(struct sk_buff *skb, struct nlmsghdr *nlh,
			__u32 pid);


int ibnl_multicast(struct sk_buff *skb, struct nlmsghdr *nlh,
			unsigned int group, gfp_t flags);


int ibnl_chk_listeners(unsigned int group);

#endif 
