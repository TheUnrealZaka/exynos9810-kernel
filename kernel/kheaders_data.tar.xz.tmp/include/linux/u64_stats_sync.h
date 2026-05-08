#ifndef _LINUX_U64_STATS_SYNC_H
#define _LINUX_U64_STATS_SYNC_H


#include <linux/seqlock.h>

struct u64_stats_sync {
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	seqcount_t	seq;
#endif
};


#if BITS_PER_LONG == 32 && defined(CONFIG_SMP)
#define u64_stats_init(syncp)	seqcount_init(&(syncp)->seq)
#else
static inline void u64_stats_init(struct u64_stats_sync *syncp)
{
}
#endif

static inline void u64_stats_update_begin(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_begin(&syncp->seq);
#endif
}

static inline void u64_stats_update_end(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_end(&syncp->seq);
#endif
}

static inline void u64_stats_update_begin_raw(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	raw_write_seqcount_begin(&syncp->seq);
#endif
}

static inline void u64_stats_update_end_raw(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	raw_write_seqcount_end(&syncp->seq);
#endif
}

static inline unsigned int __u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_begin(&syncp->seq);
#else
	return 0;
#endif
}

static inline unsigned int u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
	preempt_disable();
#endif
	return __u64_stats_fetch_begin(syncp);
}

static inline bool __u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
					 unsigned int start)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_retry(&syncp->seq, start);
#else
	return false;
#endif
}

static inline bool u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
					 unsigned int start)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
	preempt_enable();
#endif
	return __u64_stats_fetch_retry(syncp, start);
}


static inline unsigned int u64_stats_fetch_begin_irq(const struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
	local_irq_disable();
#endif
	return __u64_stats_fetch_begin(syncp);
}

static inline bool u64_stats_fetch_retry_irq(const struct u64_stats_sync *syncp,
					     unsigned int start)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
	local_irq_enable();
#endif
	return __u64_stats_fetch_retry(syncp, start);
}

#endif 
