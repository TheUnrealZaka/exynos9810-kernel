#ifndef IOCONTEXT_H
#define IOCONTEXT_H

#include <linux/radix-tree.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>

enum {
	ICQ_EXITED		= 1 << 2,
};


struct io_cq {
	struct request_queue	*q;
	struct io_context	*ioc;

	
	union {
		struct list_head	q_node;
		struct kmem_cache	*__rcu_icq_cache;
	};
	union {
		struct hlist_node	ioc_node;
		struct rcu_head		__rcu_head;
	};

	unsigned int		flags;
};


struct io_context {
	atomic_long_t refcount;
	atomic_t active_ref;
	atomic_t nr_tasks;

	
	spinlock_t lock;

	unsigned short ioprio;

	
	int nr_batch_requests;     
	unsigned long last_waited; 

	struct radix_tree_root	icq_tree;
	struct io_cq __rcu	*icq_hint;
	struct hlist_head	icq_list;

	struct work_struct release_work;
};


static inline void get_io_context_active(struct io_context *ioc)
{
	WARN_ON_ONCE(atomic_long_read(&ioc->refcount) <= 0);
	WARN_ON_ONCE(atomic_read(&ioc->active_ref) <= 0);
	atomic_long_inc(&ioc->refcount);
	atomic_inc(&ioc->active_ref);
}

static inline void ioc_task_link(struct io_context *ioc)
{
	get_io_context_active(ioc);

	WARN_ON_ONCE(atomic_read(&ioc->nr_tasks) <= 0);
	atomic_inc(&ioc->nr_tasks);
}

struct task_struct;
#ifdef CONFIG_BLOCK
void put_io_context(struct io_context *ioc);
void put_io_context_active(struct io_context *ioc);
void exit_io_context(struct task_struct *task);
struct io_context *get_task_io_context(struct task_struct *task,
				       gfp_t gfp_flags, int node);
#else
struct io_context;
static inline void put_io_context(struct io_context *ioc) { }
static inline void exit_io_context(struct task_struct *task) { }
#endif

#endif
