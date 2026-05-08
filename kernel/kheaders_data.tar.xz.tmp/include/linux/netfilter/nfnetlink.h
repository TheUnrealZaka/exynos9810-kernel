#ifndef _NFNETLINK_H
#define _NFNETLINK_H


#include <linux/netlink.h>
#include <linux/capability.h>
#include <net/netlink.h>
#include <uapi/linux/netfilter/nfnetlink.h>

struct nfnl_callback {
	int (*call)(struct net *net, struct sock *nl, struct sk_buff *skb,
		    const struct nlmsghdr *nlh,
		    const struct nlattr * const cda[]);
	int (*call_rcu)(struct net *net, struct sock *nl, struct sk_buff *skb,
			const struct nlmsghdr *nlh,
			const struct nlattr * const cda[]);
	int (*call_batch)(struct net *net, struct sock *nl, struct sk_buff *skb,
			  const struct nlmsghdr *nlh,
			  const struct nlattr * const cda[]);
	const struct nla_policy *policy;	
	const u_int16_t attr_count;		
};

struct nfnetlink_subsystem {
	const char *name;
	__u8 subsys_id;			
	__u8 cb_count;			
	const struct nfnl_callback *cb;	
	int (*commit)(struct net *net, struct sk_buff *skb);
	int (*abort)(struct net *net, struct sk_buff *skb);
};

int nfnetlink_subsys_register(const struct nfnetlink_subsystem *n);
int nfnetlink_subsys_unregister(const struct nfnetlink_subsystem *n);

int nfnetlink_has_listeners(struct net *net, unsigned int group);
int nfnetlink_send(struct sk_buff *skb, struct net *net, u32 portid,
		   unsigned int group, int echo, gfp_t flags);
int nfnetlink_set_err(struct net *net, u32 portid, u32 group, int error);
int nfnetlink_unicast(struct sk_buff *skb, struct net *net, u32 portid,
		      int flags);

void nfnl_lock(__u8 subsys_id);
void nfnl_unlock(__u8 subsys_id);
#ifdef CONFIG_PROVE_LOCKING
bool lockdep_nfnl_is_held(__u8 subsys_id);
#else
static inline bool lockdep_nfnl_is_held(__u8 subsys_id)
{
	return true;
}
#endif 


#define nfnl_dereference(p, ss)					\
	rcu_dereference_protected(p, lockdep_nfnl_is_held(ss))

#define MODULE_ALIAS_NFNL_SUBSYS(subsys) \
	MODULE_ALIAS("nfnetlink-subsys-" __stringify(subsys))

#endif	
