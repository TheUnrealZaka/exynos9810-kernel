

#ifndef _LINUX_NET_BUSY_POLL_H
#define _LINUX_NET_BUSY_POLL_H

#include <linux/netdevice.h>
#include <net/ip.h>

#ifdef CONFIG_NET_RX_BUSY_POLL

struct napi_struct;
extern unsigned int sysctl_net_busy_read __read_mostly;
extern unsigned int sysctl_net_busy_poll __read_mostly;


#define MIN_NAPI_ID ((unsigned int)(NR_CPUS + 1))


#define LL_FLUSH_FAILED		-1
#define LL_FLUSH_BUSY		-2

static inline bool net_busy_loop_on(void)
{
	return READ_ONCE(sysctl_net_busy_poll);
}

static inline u64 busy_loop_us_clock(void)
{
	return local_clock() >> 10;
}

static inline unsigned long sk_busy_loop_end_time(struct sock *sk)
{
	return busy_loop_us_clock() + ACCESS_ONCE(sk->sk_ll_usec);
}


static inline unsigned long busy_loop_end_time(void)
{
	return busy_loop_us_clock() + ACCESS_ONCE(sysctl_net_busy_poll);
}

static inline bool sk_can_busy_loop(struct sock *sk)
{
	return sk->sk_ll_usec && sk->sk_napi_id &&
	       !need_resched() && !signal_pending(current);
}


static inline bool busy_loop_timeout(unsigned long end_time)
{
	unsigned long now = busy_loop_us_clock();

	return time_after(now, end_time);
}

bool sk_busy_loop(struct sock *sk, int nonblock);


static inline void skb_mark_napi_id(struct sk_buff *skb,
				    struct napi_struct *napi)
{
	skb->napi_id = napi->napi_id;
}


static inline void sk_mark_napi_id(struct sock *sk, struct sk_buff *skb)
{
	sk->sk_napi_id = skb->napi_id;
}

#else 
static inline unsigned long net_busy_loop_on(void)
{
	return 0;
}

static inline unsigned long busy_loop_end_time(void)
{
	return 0;
}

static inline bool sk_can_busy_loop(struct sock *sk)
{
	return false;
}

static inline void skb_mark_napi_id(struct sk_buff *skb,
				    struct napi_struct *napi)
{
}

static inline void sk_mark_napi_id(struct sock *sk, struct sk_buff *skb)
{
}

static inline bool busy_loop_timeout(unsigned long end_time)
{
	return true;
}

static inline bool sk_busy_loop(struct sock *sk, int nonblock)
{
	return false;
}

#endif 
#endif 
