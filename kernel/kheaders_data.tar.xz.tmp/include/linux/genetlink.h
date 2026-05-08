#ifndef __LINUX_GENERIC_NETLINK_H
#define __LINUX_GENERIC_NETLINK_H

#include <uapi/linux/genetlink.h>



extern void genl_lock(void);
extern void genl_unlock(void);
#ifdef CONFIG_LOCKDEP
extern bool lockdep_genl_is_held(void);
#endif


extern atomic_t genl_sk_destructing_cnt;
extern wait_queue_head_t genl_sk_destructing_waitq;


#define rcu_dereference_genl(p)					\
	rcu_dereference_check(p, lockdep_genl_is_held())


#define genl_dereference(p)					\
	rcu_dereference_protected(p, lockdep_genl_is_held())

#define MODULE_ALIAS_GENL_FAMILY(family)\
 MODULE_ALIAS_NET_PF_PROTO_NAME(PF_NETLINK, NETLINK_GENERIC, "-family-" family)

#endif	
