#ifndef __NET_SCHED_CODEL_QDISC_H
#define __NET_SCHED_CODEL_QDISC_H






struct codel_skb_cb {
	codel_time_t enqueue_time;
	unsigned int mem_usage;
};

static struct codel_skb_cb *get_codel_cb(const struct sk_buff *skb)
{
	qdisc_cb_private_validate(skb, sizeof(struct codel_skb_cb));
	return (struct codel_skb_cb *)qdisc_skb_cb(skb)->data;
}

static codel_time_t codel_get_enqueue_time(const struct sk_buff *skb)
{
	return get_codel_cb(skb)->enqueue_time;
}

static void codel_set_enqueue_time(struct sk_buff *skb)
{
	get_codel_cb(skb)->enqueue_time = codel_get_time();
}

#endif
