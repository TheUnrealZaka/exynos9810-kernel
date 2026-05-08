

#ifndef _INET_HASHTABLES_H
#define _INET_HASHTABLES_H


#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/wait.h>

#include <net/inet_connection_sock.h>
#include <net/inet_sock.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/tcp_states.h>
#include <net/netns/hash.h>

#include <linux/atomic.h>
#include <asm/byteorder.h>


struct inet_ehash_bucket {
	struct hlist_nulls_head chain;
};


struct inet_bind_bucket {
	possible_net_t		ib_net;
	unsigned short		port;
	signed char		fastreuse;
	signed char		fastreuseport;
	kuid_t			fastuid;
	int			num_owners;
	struct hlist_node	node;
	struct hlist_head	owners;
};

static inline struct net *ib_net(struct inet_bind_bucket *ib)
{
	return read_pnet(&ib->ib_net);
}

#define inet_bind_bucket_for_each(tb, head) \
	hlist_for_each_entry(tb, head, node)

struct inet_bind_hashbucket {
	spinlock_t		lock;
	struct hlist_head	chain;
};


#define LISTENING_NULLS_BASE (1U << 29)
struct inet_listen_hashbucket {
	spinlock_t		lock;
	union {
		struct hlist_head	head;
		struct hlist_nulls_head	nulls_head;
	};
};


#define INET_LHTABLE_SIZE	32	

struct inet_hashinfo {
	
	struct inet_ehash_bucket	*ehash;
	spinlock_t			*ehash_locks;
	unsigned int			ehash_mask;
	unsigned int			ehash_locks_mask;

	
	struct inet_bind_hashbucket	*bhash;

	unsigned int			bhash_size;
	

	struct kmem_cache		*bind_bucket_cachep;

	
	
	struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE]
					____cacheline_aligned_in_smp;
};

static inline struct inet_ehash_bucket *inet_ehash_bucket(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash[hash & hashinfo->ehash_mask];
}

static inline spinlock_t *inet_ehash_lockp(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash_locks[hash & hashinfo->ehash_locks_mask];
}

int inet_ehash_locks_alloc(struct inet_hashinfo *hashinfo);

static inline void inet_ehash_locks_free(struct inet_hashinfo *hashinfo)
{
	kvfree(hashinfo->ehash_locks);
	hashinfo->ehash_locks = NULL;
}

struct inet_bind_bucket *
inet_bind_bucket_create(struct kmem_cache *cachep, struct net *net,
			struct inet_bind_hashbucket *head,
			const unsigned short snum);
void inet_bind_bucket_destroy(struct kmem_cache *cachep,
			      struct inet_bind_bucket *tb);

static inline u32 inet_bhashfn(const struct net *net, const __u16 lport,
			       const u32 bhash_size)
{
	return (lport + net_hash_mix(net)) & (bhash_size - 1);
}

void inet_bind_hash(struct sock *sk, struct inet_bind_bucket *tb,
		    const unsigned short snum);


static inline u32 inet_lhashfn(const struct net *net, const unsigned short num)
{
	return (num + net_hash_mix(net)) & (INET_LHTABLE_SIZE - 1);
}

static inline int inet_sk_listen_hashfn(const struct sock *sk)
{
	return inet_lhashfn(sock_net(sk), inet_sk(sk)->inet_num);
}


int __inet_inherit_port(const struct sock *sk, struct sock *child);

void inet_put_port(struct sock *sk);

void inet_hashinfo_init(struct inet_hashinfo *h);

bool inet_ehash_insert(struct sock *sk, struct sock *osk);
bool inet_ehash_nolisten(struct sock *sk, struct sock *osk);
int __inet_hash(struct sock *sk, struct sock *osk,
		int (*saddr_same)(const struct sock *sk1,
				  const struct sock *sk2,
				  bool match_wildcard));
int inet_hash(struct sock *sk);
void inet_unhash(struct sock *sk);

struct sock *__inet_lookup_listener(struct net *net,
				    struct inet_hashinfo *hashinfo,
				    struct sk_buff *skb, int doff,
				    const __be32 saddr, const __be16 sport,
				    const __be32 daddr,
				    const unsigned short hnum,
				    const int dif, const int sdif);

static inline struct sock *inet_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo,
		struct sk_buff *skb, int doff,
		__be32 saddr, __be16 sport,
		__be32 daddr, __be16 dport, int dif, int sdif)
{
	return __inet_lookup_listener(net, hashinfo, skb, doff, saddr, sport,
				      daddr, ntohs(dport), dif, sdif);
}



#ifdef __BIG_ENDIAN
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__force __u32)(__be16)(__sport) << 16) | (__u32)(__dport)))
#else 
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__u32)(__dport) << 16) | (__force __u32)(__be16)(__sport)))
#endif

#if (BITS_PER_LONG == 64)
#ifdef __BIG_ENDIAN
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__saddr)) << 32) | \
				   ((__force __u64)(__be32)(__daddr)))
#else 
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__daddr)) << 32) | \
				   ((__force __u64)(__be32)(__saddr)))
#endif 
#define INET_MATCH(__sk, __net, __cookie, __saddr, __daddr, __ports, __dif, __sdif) \
	(((__sk)->sk_portpair == (__ports))			&&	\
	 ((__sk)->sk_addrpair == (__cookie))			&&	\
	 (!(__sk)->sk_bound_dev_if	||				\
	   ((__sk)->sk_bound_dev_if == (__dif))			||	\
	   ((__sk)->sk_bound_dev_if == (__sdif)))		&&	\
	 net_eq(sock_net(__sk), (__net)))
#else 
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const int __name __deprecated __attribute__((unused))

#define INET_MATCH(__sk, __net, __cookie, __saddr, __daddr, __ports, __dif, __sdif) \
	(((__sk)->sk_portpair == (__ports))		&&		\
	 ((__sk)->sk_daddr	== (__saddr))		&&		\
	 ((__sk)->sk_rcv_saddr	== (__daddr))		&&		\
	 (!(__sk)->sk_bound_dev_if	||				\
	   ((__sk)->sk_bound_dev_if == (__dif))		||		\
	   ((__sk)->sk_bound_dev_if == (__sdif)))	&&		\
	 net_eq(sock_net(__sk), (__net)))
#endif 


struct sock *__inet_lookup_established(struct net *net,
				       struct inet_hashinfo *hashinfo,
				       const __be32 saddr, const __be16 sport,
				       const __be32 daddr, const u16 hnum,
				       const int dif, const int sdif);

static inline struct sock *
	inet_lookup_established(struct net *net, struct inet_hashinfo *hashinfo,
				const __be32 saddr, const __be16 sport,
				const __be32 daddr, const __be16 dport,
				const int dif)
{
	return __inet_lookup_established(net, hashinfo, saddr, sport, daddr,
					 ntohs(dport), dif, 0);
}

static inline struct sock *__inet_lookup(struct net *net,
					 struct inet_hashinfo *hashinfo,
					 struct sk_buff *skb, int doff,
					 const __be32 saddr, const __be16 sport,
					 const __be32 daddr, const __be16 dport,
					 const int dif, const int sdif,
					 bool *refcounted)
{
	u16 hnum = ntohs(dport);
	struct sock *sk;

	sk = __inet_lookup_established(net, hashinfo, saddr, sport,
				       daddr, hnum, dif, sdif);
	*refcounted = true;
	if (sk)
		return sk;
	*refcounted = false;
	return __inet_lookup_listener(net, hashinfo, skb, doff, saddr,
				      sport, daddr, hnum, dif, sdif);
}

static inline struct sock *inet_lookup(struct net *net,
				       struct inet_hashinfo *hashinfo,
				       struct sk_buff *skb, int doff,
				       const __be32 saddr, const __be16 sport,
				       const __be32 daddr, const __be16 dport,
				       const int dif)
{
	struct sock *sk;
	bool refcounted;

	sk = __inet_lookup(net, hashinfo, skb, doff, saddr, sport, daddr,
			   dport, dif, 0, &refcounted);

	if (sk && !refcounted && !atomic_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;
	return sk;
}

static inline struct sock *__inet_lookup_skb(struct inet_hashinfo *hashinfo,
					     struct sk_buff *skb,
					     int doff,
					     const __be16 sport,
					     const __be16 dport,
					     const int sdif,
					     bool *refcounted)
{
	struct sock *sk = skb_steal_sock(skb);
	const struct iphdr *iph = ip_hdr(skb);

	*refcounted = true;
	if (sk)
		return sk;

	return __inet_lookup(dev_net(skb_dst(skb)->dev), hashinfo, skb,
			     doff, iph->saddr, sport,
			     iph->daddr, dport, inet_iif(skb), sdif,
			     refcounted);
}

u32 sk_ehashfn(const struct sock *sk);
u32 inet6_ehashfn(const struct net *net,
		  const struct in6_addr *laddr, const u16 lport,
		  const struct in6_addr *faddr, const __be16 fport);

static inline void sk_daddr_set(struct sock *sk, __be32 addr)
{
	sk->sk_daddr = addr; 
#if IS_ENABLED(CONFIG_IPV6)
	ipv6_addr_set_v4mapped(addr, &sk->sk_v6_daddr);
#endif
}

static inline void sk_rcv_saddr_set(struct sock *sk, __be32 addr)
{
	sk->sk_rcv_saddr = addr; 
#if IS_ENABLED(CONFIG_IPV6)
	ipv6_addr_set_v4mapped(addr, &sk->sk_v6_rcv_saddr);
#endif
}

int __inet_hash_connect(struct inet_timewait_death_row *death_row,
			struct sock *sk, u64 port_offset,
			int (*check_established)(struct inet_timewait_death_row *,
						 struct sock *, __u16,
						 struct inet_timewait_sock **));

int inet_hash_connect(struct inet_timewait_death_row *death_row,
		      struct sock *sk);
#endif 
