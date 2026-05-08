
#ifndef __NET_SCHED_FQ_H
#define __NET_SCHED_FQ_H

struct fq_tin;


struct fq_flow {
	struct fq_tin *tin;
	struct list_head flowchain;
	struct list_head backlogchain;
	struct sk_buff_head queue;
	u32 backlog;
	int deficit;
};


struct fq_tin {
	struct list_head new_flows;
	struct list_head old_flows;
	u32 backlog_bytes;
	u32 backlog_packets;
	u32 overlimit;
	u32 collisions;
	u32 flows;
	u32 tx_bytes;
	u32 tx_packets;
};


struct fq {
	struct fq_flow *flows;
	struct list_head backlogs;
	spinlock_t lock;
	u32 flows_cnt;
	siphash_key_t	perturbation;
	u32 limit;
	u32 memory_limit;
	u32 memory_usage;
	u32 quantum;
	u32 backlog;
	u32 overlimit;
	u32 overmemory;
	u32 collisions;
};

typedef struct sk_buff *fq_tin_dequeue_t(struct fq *,
					 struct fq_tin *,
					 struct fq_flow *flow);

typedef void fq_skb_free_t(struct fq *,
			   struct fq_tin *,
			   struct fq_flow *,
			   struct sk_buff *);

typedef struct fq_flow *fq_flow_get_default_t(struct fq *,
					      struct fq_tin *,
					      int idx,
					      struct sk_buff *);

#endif
