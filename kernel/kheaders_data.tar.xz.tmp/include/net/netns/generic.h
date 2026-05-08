

#ifndef __NET_GENERIC_H__
#define __NET_GENERIC_H__

#include <linux/bug.h>
#include <linux/rcupdate.h>



struct net_generic {
	unsigned int len;
	struct rcu_head rcu;

	void *ptr[0];
};

static inline void *net_generic(const struct net *net, int id)
{
	struct net_generic *ng;
	void *ptr;

	rcu_read_lock();
	ng = rcu_dereference(net->gen);
	ptr = ng->ptr[id - 1];
	rcu_read_unlock();

	return ptr;
}
#endif
