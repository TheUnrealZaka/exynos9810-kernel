#ifndef _NET_DST_CACHE_H
#define _NET_DST_CACHE_H

#include <linux/jiffies.h>
#include <net/dst.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/ip6_fib.h>
#endif

struct dst_cache {
	struct dst_cache_pcpu __percpu *cache;
	unsigned long reset_ts;
};


struct dst_entry *dst_cache_get(struct dst_cache *dst_cache);


struct rtable *dst_cache_get_ip4(struct dst_cache *dst_cache, __be32 *saddr);


void dst_cache_set_ip4(struct dst_cache *dst_cache, struct dst_entry *dst,
		       __be32 saddr);

#if IS_ENABLED(CONFIG_IPV6)


void dst_cache_set_ip6(struct dst_cache *dst_cache, struct dst_entry *dst,
		       const struct in6_addr *addr);


struct dst_entry *dst_cache_get_ip6(struct dst_cache *dst_cache,
				    struct in6_addr *saddr);
#endif


static inline void dst_cache_reset(struct dst_cache *dst_cache)
{
	dst_cache->reset_ts = jiffies;
}


int dst_cache_init(struct dst_cache *dst_cache, gfp_t gfp);


void dst_cache_destroy(struct dst_cache *dst_cache);

#endif
