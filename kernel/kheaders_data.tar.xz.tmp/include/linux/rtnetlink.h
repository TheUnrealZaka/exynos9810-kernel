#ifndef __LINUX_RTNETLINK_H
#define __LINUX_RTNETLINK_H


#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/wait.h>
#include <uapi/linux/rtnetlink.h>

extern int rtnetlink_send(struct sk_buff *skb, struct net *net, u32 pid, u32 group, int echo);
extern int rtnl_unicast(struct sk_buff *skb, struct net *net, u32 pid);
extern void rtnl_notify(struct sk_buff *skb, struct net *net, u32 pid,
			u32 group, struct nlmsghdr *nlh, gfp_t flags);
extern void rtnl_set_sk_err(struct net *net, u32 group, int error);
extern int rtnetlink_put_metrics(struct sk_buff *skb, u32 *metrics);
extern int rtnl_put_cacheinfo(struct sk_buff *skb, struct dst_entry *dst,
			      u32 id, long expires, u32 error);

void rtmsg_ifinfo(int type, struct net_device *dev, unsigned change, gfp_t flags);
struct sk_buff *rtmsg_ifinfo_build_skb(int type, struct net_device *dev,
				       unsigned change, gfp_t flags);
void rtmsg_ifinfo_send(struct sk_buff *skb, struct net_device *dev,
		       gfp_t flags);



extern void rtnl_lock(void);
extern void rtnl_unlock(void);
extern int rtnl_trylock(void);
extern int rtnl_is_locked(void);

extern wait_queue_head_t netdev_unregistering_wq;
extern struct mutex net_mutex;

#ifdef CONFIG_PROVE_LOCKING
extern bool lockdep_rtnl_is_held(void);
#else
static inline bool lockdep_rtnl_is_held(void)
{
	return true;
}
#endif 


#define rcu_dereference_rtnl(p)					\
	rcu_dereference_check(p, lockdep_rtnl_is_held())


#define rcu_dereference_bh_rtnl(p)				\
	rcu_dereference_bh_check(p, lockdep_rtnl_is_held())


#define rtnl_dereference(p)					\
	rcu_dereference_protected(p, lockdep_rtnl_is_held())

static inline struct netdev_queue *dev_ingress_queue(struct net_device *dev)
{
	return rtnl_dereference(dev->ingress_queue);
}

struct netdev_queue *dev_ingress_queue_create(struct net_device *dev);

#ifdef CONFIG_NET_INGRESS
void net_inc_ingress_queue(void);
void net_dec_ingress_queue(void);
#endif

#ifdef CONFIG_NET_EGRESS
void net_inc_egress_queue(void);
void net_dec_egress_queue(void);
#endif

void rtnetlink_init(void);
void __rtnl_unlock(void);
void rtnl_kfree_skbs(struct sk_buff *head, struct sk_buff *tail);

#define ASSERT_RTNL() do { \
	if (unlikely(!rtnl_is_locked())) { \
		printk(KERN_ERR "RTNL: assertion failed at %s (%d)\n", \
		       __FILE__,  __LINE__); \
		dump_stack(); \
	} \
} while(0)

extern int ndo_dflt_fdb_dump(struct sk_buff *skb,
			     struct netlink_callback *cb,
			     struct net_device *dev,
			     struct net_device *filter_dev,
			     int *idx);
extern int ndo_dflt_fdb_add(struct ndmsg *ndm,
			    struct nlattr *tb[],
			    struct net_device *dev,
			    const unsigned char *addr,
			    u16 vid,
			    u16 flags);
extern int ndo_dflt_fdb_del(struct ndmsg *ndm,
			    struct nlattr *tb[],
			    struct net_device *dev,
			    const unsigned char *addr,
			    u16 vid);

extern int ndo_dflt_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
				   struct net_device *dev, u16 mode,
				   u32 flags, u32 mask, int nlflags,
				   u32 filter_mask,
				   int (*vlan_fill)(struct sk_buff *skb,
						    struct net_device *dev,
						    u32 filter_mask));
#endif	
