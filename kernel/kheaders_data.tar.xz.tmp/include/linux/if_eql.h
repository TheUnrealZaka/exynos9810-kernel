
#ifndef _LINUX_IF_EQL_H
#define _LINUX_IF_EQL_H


#include <linux/timer.h>
#include <linux/spinlock.h>
#include <uapi/linux/if_eql.h>

typedef struct slave {
	struct list_head	list;
	struct net_device	*dev;
	long			priority;
	long			priority_bps;
	long			priority_Bps;
	long			bytes_queued;
} slave_t;

typedef struct slave_queue {
	spinlock_t		lock;
	struct list_head	all_slaves;
	int			num_slaves;
	struct net_device	*master_dev;
} slave_queue_t;

typedef struct equalizer {
	slave_queue_t		queue;
	int			min_slaves;
	int			max_slaves;
	struct timer_list	timer;
} equalizer_t;  

#endif 
