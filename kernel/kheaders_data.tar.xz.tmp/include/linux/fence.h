

#ifndef __LINUX_FENCE_H
#define __LINUX_FENCE_H

#include <linux/err.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <linux/kref.h>
#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/rcupdate.h>

struct fence;
struct fence_ops;
struct fence_cb;


struct fence {
	int magic_bit;
	struct kref refcount;
	const struct fence_ops *ops;
	struct rcu_head rcu;
	struct list_head cb_list;
	spinlock_t *lock;
	u64 context;
	unsigned seqno;
	unsigned long flags;
	ktime_t timestamp;
	int error;
};

enum fence_flag_bits {
	FENCE_FLAG_SIGNALED_BIT,
	FENCE_FLAG_ENABLE_SIGNAL_BIT,
	FENCE_FLAG_USER_BITS, 
};

typedef void (*fence_func_t)(struct fence *fence, struct fence_cb *cb);


struct fence_cb {
	struct list_head node;
	fence_func_t func;
};



struct fence_ops {
	const char * (*get_driver_name)(struct fence *fence);
	const char * (*get_timeline_name)(struct fence *fence);
	bool (*enable_signaling)(struct fence *fence);
	void (*disable_signaling)(struct fence *fence);
	bool (*signaled)(struct fence *fence);
	signed long (*wait)(struct fence *fence, bool intr, signed long timeout);
	void (*release)(struct fence *fence);

	int (*fill_driver_data)(struct fence *fence, void *data, int size);
	void (*fence_value_str)(struct fence *fence, char *str, int size);
	void (*timeline_value_str)(struct fence *fence, char *str, int size);
};

void fence_init(struct fence *fence, const struct fence_ops *ops,
		spinlock_t *lock, u64 context, unsigned seqno);

void fence_release(struct kref *kref);
void fence_free(struct fence *fence);


static inline void fence_put(struct fence *fence)
{
	if (fence)
		kref_put(&fence->refcount, fence_release);
}


static inline struct fence *fence_get(struct fence *fence)
{
	if (fence)
		kref_get(&fence->refcount);
	return fence;
}


static inline struct fence *fence_get_rcu(struct fence *fence)
{
	if (kref_get_unless_zero(&fence->refcount))
		return fence;
	else
		return NULL;
}


static inline struct fence *fence_get_rcu_safe(struct fence * __rcu *fencep)
{
	do {
		struct fence *fence;

		fence = rcu_dereference(*fencep);
		if (!fence || !fence_get_rcu(fence))
			return NULL;

		
		if (fence == rcu_access_pointer(*fencep))
			return rcu_pointer_handoff(fence);

		fence_put(fence);
	} while (1);
}

int fence_signal(struct fence *fence);
int fence_signal_locked(struct fence *fence);
signed long fence_default_wait(struct fence *fence, bool intr, signed long timeout);
int fence_add_callback(struct fence *fence, struct fence_cb *cb,
		       fence_func_t func);
bool fence_remove_callback(struct fence *fence, struct fence_cb *cb);
void fence_enable_sw_signaling(struct fence *fence);


static inline bool
fence_is_signaled_locked(struct fence *fence)
{
	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return true;

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		fence_signal_locked(fence);
		return true;
	}

	return false;
}


static inline bool
fence_is_signaled(struct fence *fence)
{
	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return true;

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		fence_signal(fence);
		return true;
	}

	return false;
}


static inline bool fence_is_later(struct fence *f1, struct fence *f2)
{
	if (WARN_ON(f1->context != f2->context))
		return false;

	return (int)(f1->seqno - f2->seqno) > 0;
}


static inline struct fence *fence_later(struct fence *f1, struct fence *f2)
{
	if (WARN_ON(f1->context != f2->context))
		return NULL;

	
	if (fence_is_later(f1, f2))
		return fence_is_signaled(f1) ? NULL : f1;
	else
		return fence_is_signaled(f2) ? NULL : f2;
}


static inline int fence_get_status_locked(struct fence *fence)
{
	if (fence_is_signaled_locked(fence))
		return fence->error ?: 1;
	else
		return 0;
}

int fence_get_status(struct fence *fence);


static inline void fence_set_error(struct fence *fence,
				       int error)
{
	BUG_ON(test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags));
	BUG_ON(error >= 0 || error < -MAX_ERRNO);

	fence->error = error;
}

signed long fence_wait_timeout(struct fence *, bool intr, signed long timeout);
signed long fence_wait_any_timeout(struct fence **fences, uint32_t count,
				   bool intr, signed long timeout);


static inline signed long fence_wait(struct fence *fence, bool intr)
{
	signed long ret;

	
	ret = fence_wait_timeout(fence, intr, MAX_SCHEDULE_TIMEOUT);

	return ret < 0 ? ret : 0;
}

u64 fence_context_alloc(unsigned num);

#define FENCE_TRACE(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		if (IS_ENABLED(CONFIG_FENCE_TRACE))			\
			pr_info("f %llu#%u: " fmt,			\
				__ff->context, __ff->seqno, ##args);	\
	} while (0)

#define FENCE_WARN(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		pr_warn("f %llu#%u: " fmt, __ff->context, __ff->seqno,	\
			 ##args);					\
	} while (0)

#define FENCE_ERR(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		pr_err("f %llu#%u: " fmt, __ff->context, __ff->seqno,	\
			##args);					\
	} while (0)

#endif 
