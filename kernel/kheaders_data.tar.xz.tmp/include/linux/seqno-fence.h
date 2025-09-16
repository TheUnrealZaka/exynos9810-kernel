

#ifndef __LINUX_SEQNO_FENCE_H
#define __LINUX_SEQNO_FENCE_H

#include <linux/fence.h>
#include <linux/dma-buf.h>

enum seqno_fence_condition {
	SEQNO_FENCE_WAIT_GEQUAL,
	SEQNO_FENCE_WAIT_NONZERO
};

struct seqno_fence {
	struct fence base;

	const struct fence_ops *ops;
	struct dma_buf *sync_buf;
	uint32_t seqno_ofs;
	enum seqno_fence_condition condition;
};

extern const struct fence_ops seqno_fence_ops;


static inline struct seqno_fence *
to_seqno_fence(struct fence *fence)
{
	if (fence->ops != &seqno_fence_ops)
		return NULL;
	return container_of(fence, struct seqno_fence, base);
}


static inline void
seqno_fence_init(struct seqno_fence *fence, spinlock_t *lock,
		 struct dma_buf *sync_buf,  uint32_t context,
		 uint32_t seqno_ofs, uint32_t seqno,
		 enum seqno_fence_condition cond,
		 const struct fence_ops *ops)
{
	BUG_ON(!fence || !sync_buf || !ops);
	BUG_ON(!ops->wait || !ops->enable_signaling ||
	       !ops->get_driver_name || !ops->get_timeline_name);

	
	fence->ops = ops;
	fence_init(&fence->base, &seqno_fence_ops, lock, context, seqno);
	get_dma_buf(sync_buf);
	fence->sync_buf = sync_buf;
	fence->seqno_ofs = seqno_ofs;
	fence->condition = cond;
}

#endif 
