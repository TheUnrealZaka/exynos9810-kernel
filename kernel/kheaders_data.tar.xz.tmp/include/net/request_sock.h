
#ifndef _REQUEST_SOCK_H
#define _REQUEST_SOCK_H

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/bug.h>

#include <net/sock.h>

struct request_sock;
struct sk_buff;
struct dst_entry;
struct proto;

struct request_sock_ops {
	int		family;
	int		obj_size;
	struct kmem_cache	*slab;
	char		*slab_name;
	int		(*rtx_syn_ack)(const struct sock *sk,
				       struct request_sock *req);
	void		(*send_ack)(const struct sock *sk, struct sk_buff *skb,
				    struct request_sock *req);
	void		(*send_reset)(const struct sock *sk,
				      struct sk_buff *skb);
	void		(*destructor)(struct request_sock *req);
	void		(*syn_ack_timeout)(const struct request_sock *req);
};

int inet_rtx_syn_ack(const struct sock *parent, struct request_sock *req);


struct request_sock {
	struct sock_common		__req_common;
#define rsk_refcnt			__req_common.skc_refcnt
#define rsk_hash			__req_common.skc_hash
#define rsk_listener			__req_common.skc_listener
#define rsk_window_clamp		__req_common.skc_window_clamp
#define rsk_rcv_wnd			__req_common.skc_rcv_wnd

	struct request_sock		*dl_next;
	u16				mss;
	u8				num_retrans; 
	u8				cookie_ts:1; 
	u8				num_timeout:7; 
	u32				ts_recent;
	struct timer_list		rsk_timer;
	const struct request_sock_ops	*rsk_ops;
	struct sock			*sk;
	u32				*saved_syn;
	u32				secid;
	u32				peer_secid;
};

static inline struct request_sock *inet_reqsk(const struct sock *sk)
{
	return (struct request_sock *)sk;
}

static inline struct sock *req_to_sk(struct request_sock *req)
{
	return (struct sock *)req;
}

static inline struct request_sock *
reqsk_alloc(const struct request_sock_ops *ops, struct sock *sk_listener,
	    bool attach_listener)
{
	struct request_sock *req;

	req = kmem_cache_alloc(ops->slab, GFP_ATOMIC | __GFP_NOWARN);
	if (!req)
		return NULL;
	req->rsk_listener = NULL;
	if (attach_listener) {
		if (unlikely(!atomic_inc_not_zero(&sk_listener->sk_refcnt))) {
			kmem_cache_free(ops->slab, req);
			return NULL;
		}
		req->rsk_listener = sk_listener;
	}
	req->rsk_ops = ops;
	req_to_sk(req)->sk_prot = sk_listener->sk_prot;
	sk_node_init(&req_to_sk(req)->sk_node);
	sk_tx_queue_clear(req_to_sk(req));
	req->saved_syn = NULL;
	atomic_set(&req->rsk_refcnt, 0);

	return req;
}

static inline void reqsk_free(struct request_sock *req)
{
	
	WARN_ON_ONCE(atomic_read(&req->rsk_refcnt) != 0);

	req->rsk_ops->destructor(req);
	if (req->rsk_listener)
		sock_put(req->rsk_listener);
	kfree(req->saved_syn);
	kmem_cache_free(req->rsk_ops->slab, req);
}

static inline void reqsk_put(struct request_sock *req)
{
	if (atomic_dec_and_test(&req->rsk_refcnt))
		reqsk_free(req);
}

extern int sysctl_max_syn_backlog;


struct fastopen_queue {
	struct request_sock	*rskq_rst_head; 
	struct request_sock	*rskq_rst_tail; 
	spinlock_t	lock;
	int		qlen;		
	int		max_qlen;	
};


struct request_sock_queue {
	spinlock_t		rskq_lock;
	u8			rskq_defer_accept;

	u32			synflood_warned;
	atomic_t		qlen;
	atomic_t		young;

	struct request_sock	*rskq_accept_head;
	struct request_sock	*rskq_accept_tail;
	struct fastopen_queue	fastopenq;  
};

void reqsk_queue_alloc(struct request_sock_queue *queue);

void reqsk_fastopen_remove(struct sock *sk, struct request_sock *req,
			   bool reset);

static inline bool reqsk_queue_empty(const struct request_sock_queue *queue)
{
	return queue->rskq_accept_head == NULL;
}

static inline struct request_sock *reqsk_queue_remove(struct request_sock_queue *queue,
						      struct sock *parent)
{
	struct request_sock *req;

	spin_lock_bh(&queue->rskq_lock);
	req = queue->rskq_accept_head;
	if (req) {
		sk_acceptq_removed(parent);
		queue->rskq_accept_head = req->dl_next;
		if (queue->rskq_accept_head == NULL)
			queue->rskq_accept_tail = NULL;
	}
	spin_unlock_bh(&queue->rskq_lock);
	return req;
}

static inline void reqsk_queue_removed(struct request_sock_queue *queue,
				       const struct request_sock *req)
{
	if (req->num_timeout == 0)
		atomic_dec(&queue->young);
	atomic_dec(&queue->qlen);
}

static inline void reqsk_queue_added(struct request_sock_queue *queue)
{
	atomic_inc(&queue->young);
	atomic_inc(&queue->qlen);
}

static inline int reqsk_queue_len(const struct request_sock_queue *queue)
{
	return atomic_read(&queue->qlen);
}

static inline int reqsk_queue_len_young(const struct request_sock_queue *queue)
{
	return atomic_read(&queue->young);
}

#endif 
