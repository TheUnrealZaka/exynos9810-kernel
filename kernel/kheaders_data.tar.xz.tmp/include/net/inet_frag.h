#ifndef __NET_FRAG_H__
#define __NET_FRAG_H__

#include <linux/rhashtable.h>

struct netns_frags {
	
	long			high_thresh;
	long			low_thresh;
	int			timeout;
	int			max_dist;
	struct inet_frags	*f;

	struct rhashtable       rhashtable ____cacheline_aligned_in_smp;

	
	atomic_long_t		mem ____cacheline_aligned_in_smp;
};


enum {
	INET_FRAG_FIRST_IN	= BIT(0),
	INET_FRAG_LAST_IN	= BIT(1),
	INET_FRAG_COMPLETE	= BIT(2),
};

struct frag_v4_compare_key {
	__be32		saddr;
	__be32		daddr;
	u32		user;
	u32		vif;
	__be16		id;
	u16		protocol;
};

struct frag_v6_compare_key {
	struct in6_addr	saddr;
	struct in6_addr	daddr;
	u32		user;
	__be32		id;
	u32		iif;
};


struct inet_frag_queue {
	struct rhash_head	node;
	union {
		struct frag_v4_compare_key v4;
		struct frag_v6_compare_key v6;
	} key;
	struct timer_list	timer;
	spinlock_t		lock;
	atomic_t		refcnt;
	struct sk_buff		*fragments;  
	struct rb_root		rb_fragments; 
	struct sk_buff		*fragments_tail;
	struct sk_buff		*last_run_head;
	ktime_t			stamp;
	int			len;
	int			meat;
	__u8			flags;
	u16			max_size;
	struct netns_frags      *net;
	struct rcu_head		rcu;
};

struct inet_frags {
	int			qsize;

	void			(*constructor)(struct inet_frag_queue *q,
					       const void *arg);
	void			(*destructor)(struct inet_frag_queue *);
	void			(*frag_expire)(unsigned long data);
	struct kmem_cache	*frags_cachep;
	const char		*frags_cache_name;
	struct rhashtable_params rhash_params;
};

int inet_frags_init(struct inet_frags *);
void inet_frags_fini(struct inet_frags *);

static inline int inet_frags_init_net(struct netns_frags *nf)
{
	atomic_long_set(&nf->mem, 0);
	return rhashtable_init(&nf->rhashtable, &nf->f->rhash_params);
}
void inet_frags_exit_net(struct netns_frags *nf);

void inet_frag_kill(struct inet_frag_queue *q);
void inet_frag_destroy(struct inet_frag_queue *q);
struct inet_frag_queue *inet_frag_find(struct netns_frags *nf, void *key);


unsigned int inet_frag_rbtree_purge(struct rb_root *root);

static inline void inet_frag_put(struct inet_frag_queue *q)
{
	if (atomic_dec_and_test(&q->refcnt))
		inet_frag_destroy(q);
}



static inline long frag_mem_limit(const struct netns_frags *nf)
{
	return atomic_long_read(&nf->mem);
}

static inline void sub_frag_mem_limit(struct netns_frags *nf, long val)
{
	atomic_long_sub(val, &nf->mem);
}

static inline void add_frag_mem_limit(struct netns_frags *nf, long val)
{
	atomic_long_add(val, &nf->mem);
}


#define	IPFRAG_ECN_NOT_ECT	0x01 
#define	IPFRAG_ECN_ECT_1	0x02 
#define	IPFRAG_ECN_ECT_0	0x04 
#define	IPFRAG_ECN_CE		0x08 

extern const u8 ip_frag_ecn_table[16];


#define IPFRAG_OK	0
#define IPFRAG_DUP	1
#define IPFRAG_OVERLAP	2
int inet_frag_queue_insert(struct inet_frag_queue *q, struct sk_buff *skb,
			   int offset, int end);
void *inet_frag_reasm_prepare(struct inet_frag_queue *q, struct sk_buff *skb,
			      struct sk_buff *parent);
void inet_frag_reasm_finish(struct inet_frag_queue *q, struct sk_buff *head,
			    void *reasm_data);
struct sk_buff *inet_frag_pull_head(struct inet_frag_queue *q);

#endif
