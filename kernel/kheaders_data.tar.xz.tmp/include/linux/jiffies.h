#ifndef _LINUX_JIFFIES_H
#define _LINUX_JIFFIES_H

#include <linux/cache.h>
#include <linux/math64.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <asm/param.h>			
#include <generated/timeconst.h>


#if HZ >= 12 && HZ < 24
# define SHIFT_HZ	4
#elif HZ >= 24 && HZ < 48
# define SHIFT_HZ	5
#elif HZ >= 48 && HZ < 96
# define SHIFT_HZ	6
#elif HZ >= 96 && HZ < 192
# define SHIFT_HZ	7
#elif HZ >= 192 && HZ < 384
# define SHIFT_HZ	8
#elif HZ >= 384 && HZ < 768
# define SHIFT_HZ	9
#elif HZ >= 768 && HZ < 1536
# define SHIFT_HZ	10
#elif HZ >= 1536 && HZ < 3072
# define SHIFT_HZ	11
#elif HZ >= 3072 && HZ < 6144
# define SHIFT_HZ	12
#elif HZ >= 6144 && HZ < 12288
# define SHIFT_HZ	13
#else
# error Invalid value of HZ.
#endif


#define SH_DIV(NOM,DEN,LSH) (   (((NOM) / (DEN)) << (LSH))              \
                             + ((((NOM) % (DEN)) << (LSH)) + (DEN) / 2) / (DEN))


#define LATCH ((CLOCK_TICK_RATE + HZ/2) / HZ)	

extern int register_refined_jiffies(long clock_tick_rate);


#define TICK_NSEC ((NSEC_PER_SEC+HZ/2)/HZ)


#define TICK_USEC ((1000000UL + USER_HZ/2) / USER_HZ)

#ifndef __jiffy_arch_data
#define __jiffy_arch_data
#endif


extern u64 __cacheline_aligned_in_smp jiffies_64;
extern unsigned long volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;

#if (BITS_PER_LONG < 64)
u64 get_jiffies_64(void);
#else
static inline u64 get_jiffies_64(void)
{
	return (u64)jiffies;
}
#endif


#define time_after(a,b)		\
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)((b) - (a)) < 0))
#define time_before(a,b)	time_after(b,a)

#define time_after_eq(a,b)	\
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)((a) - (b)) >= 0))
#define time_before_eq(a,b)	time_after_eq(b,a)


#define time_in_range(a,b,c) \
	(time_after_eq(a,b) && \
	 time_before_eq(a,c))


#define time_in_range_open(a,b,c) \
	(time_after_eq(a,b) && \
	 time_before(a,c))


#define time_after64(a,b)	\
	(typecheck(__u64, a) &&	\
	 typecheck(__u64, b) && \
	 ((__s64)((b) - (a)) < 0))
#define time_before64(a,b)	time_after64(b,a)

#define time_after_eq64(a,b)	\
	(typecheck(__u64, a) && \
	 typecheck(__u64, b) && \
	 ((__s64)((a) - (b)) >= 0))
#define time_before_eq64(a,b)	time_after_eq64(b,a)

#define time_in_range64(a, b, c) \
	(time_after_eq64(a, b) && \
	 time_before_eq64(a, c))




#define time_is_before_jiffies(a) time_after(jiffies, a)
#define time_is_before_jiffies64(a) time_after64(get_jiffies_64(), a)


#define time_is_after_jiffies(a) time_before(jiffies, a)
#define time_is_after_jiffies64(a) time_before64(get_jiffies_64(), a)


#define time_is_before_eq_jiffies(a) time_after_eq(jiffies, a)
#define time_is_before_eq_jiffies64(a) time_after_eq64(get_jiffies_64(), a)


#define time_is_after_eq_jiffies(a) time_before_eq(jiffies, a)
#define time_is_after_eq_jiffies64(a) time_before_eq64(get_jiffies_64(), a)


#define INITIAL_JIFFIES ((unsigned long)(unsigned int) (-300*HZ))


#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)

extern unsigned long preset_lpj;





#define SEC_JIFFIE_SC (31 - SHIFT_HZ)
#if !((((NSEC_PER_SEC << 2) / TICK_NSEC) << (SEC_JIFFIE_SC - 2)) & 0x80000000)
#undef SEC_JIFFIE_SC
#define SEC_JIFFIE_SC (32 - SHIFT_HZ)
#endif
#define NSEC_JIFFIE_SC (SEC_JIFFIE_SC + 29)
#define SEC_CONVERSION ((unsigned long)((((u64)NSEC_PER_SEC << SEC_JIFFIE_SC) +\
                                TICK_NSEC -1) / (u64)TICK_NSEC))

#define NSEC_CONVERSION ((unsigned long)((((u64)1 << NSEC_JIFFIE_SC) +\
                                        TICK_NSEC -1) / (u64)TICK_NSEC))

#if BITS_PER_LONG < 64
# define MAX_SEC_IN_JIFFIES \
	(long)((u64)((u64)MAX_JIFFY_OFFSET * TICK_NSEC) / NSEC_PER_SEC)
#else	
# define MAX_SEC_IN_JIFFIES \
	(SH_DIV((MAX_JIFFY_OFFSET >> SEC_JIFFIE_SC) * TICK_NSEC, NSEC_PER_SEC, 1) - 1)

#endif


extern unsigned int jiffies_to_msecs(const unsigned long j);
extern unsigned int jiffies_to_usecs(const unsigned long j);

static inline u64 jiffies_to_nsecs(const unsigned long j)
{
	return (u64)jiffies_to_usecs(j) * NSEC_PER_USEC;
}

extern u64 jiffies64_to_nsecs(u64 j);

extern unsigned long __msecs_to_jiffies(const unsigned int m);
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)

static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
}
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)

static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	return m * (HZ / MSEC_PER_SEC);
}
#else

static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	if (HZ > MSEC_PER_SEC && m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;

	return (MSEC_TO_HZ_MUL32 * m + MSEC_TO_HZ_ADJ32) >> MSEC_TO_HZ_SHR32;
}
#endif

static __always_inline unsigned long msecs_to_jiffies(const unsigned int m)
{
	if (__builtin_constant_p(m)) {
		if ((int)m < 0)
			return MAX_JIFFY_OFFSET;
		return _msecs_to_jiffies(m);
	} else {
		return __msecs_to_jiffies(m);
	}
}

extern unsigned long __usecs_to_jiffies(const unsigned int u);
#if !(USEC_PER_SEC % HZ)
static inline unsigned long _usecs_to_jiffies(const unsigned int u)
{
	return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
}
#else
static inline unsigned long _usecs_to_jiffies(const unsigned int u)
{
	return (USEC_TO_HZ_MUL32 * u + USEC_TO_HZ_ADJ32)
		>> USEC_TO_HZ_SHR32;
}
#endif


static __always_inline unsigned long usecs_to_jiffies(const unsigned int u)
{
	if (__builtin_constant_p(u)) {
		if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
			return MAX_JIFFY_OFFSET;
		return _usecs_to_jiffies(u);
	} else {
		return __usecs_to_jiffies(u);
	}
}

extern unsigned long timespec64_to_jiffies(const struct timespec64 *value);
extern void jiffies_to_timespec64(const unsigned long jiffies,
				  struct timespec64 *value);
static inline unsigned long timespec_to_jiffies(const struct timespec *value)
{
	struct timespec64 ts = timespec_to_timespec64(*value);

	return timespec64_to_jiffies(&ts);
}

static inline void jiffies_to_timespec(const unsigned long jiffies,
				       struct timespec *value)
{
	struct timespec64 ts;

	jiffies_to_timespec64(jiffies, &ts);
	*value = timespec64_to_timespec(ts);
}

extern unsigned long timeval_to_jiffies(const struct timeval *value);
extern void jiffies_to_timeval(const unsigned long jiffies,
			       struct timeval *value);

extern clock_t jiffies_to_clock_t(unsigned long x);
static inline clock_t jiffies_delta_to_clock_t(long delta)
{
	return jiffies_to_clock_t(max(0L, delta));
}

extern unsigned long clock_t_to_jiffies(unsigned long x);
extern u64 jiffies_64_to_clock_t(u64 x);
extern u64 nsec_to_clock_t(u64 x);
extern u64 nsecs_to_jiffies64(u64 n);
extern unsigned long nsecs_to_jiffies(u64 n);

#define TIMESTAMP_SIZE	30

#endif
