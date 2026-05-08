

#ifndef __LINUX_FENCE_ARRAY_H
#define __LINUX_FENCE_ARRAY_H

#include <linux/fence.h>


struct fence_array_cb {
	struct fence_cb cb;
	struct fence_array *array;
};


struct fence_array {
	struct fence base;

	spinlock_t lock;
	unsigned num_fences;
	atomic_t num_pending;
	struct fence **fences;
};

extern const struct fence_ops fence_array_ops;


static inline bool fence_is_array(struct fence *fence)
{
	return fence->ops == &fence_array_ops;
}


static inline struct fence_array *to_fence_array(struct fence *fence)
{
	if (fence->ops != &fence_array_ops)
		return NULL;

	return container_of(fence, struct fence_array, base);
}

struct fence_array *fence_array_create(int num_fences, struct fence **fences,
				       u64 context, unsigned seqno,
				       bool signal_on_any);

#endif 
