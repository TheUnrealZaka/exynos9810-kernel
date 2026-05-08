#ifndef DEF_RDMAVT_INCCQ_H
#define DEF_RDMAVT_INCCQ_H



#include <linux/kthread.h>
#include <rdma/ib_user_verbs.h>


#define RVT_CQ_NONE      (IB_CQ_NEXT_COMP + 1)


struct rvt_cq_wc {
	u32 head;               
	u32 tail;               
	union {
		
		struct ib_uverbs_wc uqueue[0];
		struct ib_wc kqueue[0];
	};
};


struct rvt_cq {
	struct ib_cq ibcq;
	struct kthread_work comptask;
	spinlock_t lock; 
	u8 notify;
	u8 triggered;
	struct rvt_dev_info *rdi;
	struct rvt_cq_wc *queue;
	struct rvt_mmap_info *ip;
};

static inline struct rvt_cq *ibcq_to_rvtcq(struct ib_cq *ibcq)
{
	return container_of(ibcq, struct rvt_cq, ibcq);
}

void rvt_cq_enter(struct rvt_cq *cq, struct ib_wc *entry, bool solicited);

#endif          
