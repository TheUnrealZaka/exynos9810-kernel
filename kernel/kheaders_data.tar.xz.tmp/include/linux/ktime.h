
#ifndef _LINUX_KTIME_H
#define _LINUX_KTIME_H

#include <linux/time.h>
#include <linux/jiffies.h>


union ktime {
	s64	tv64;
};

typedef union ktime ktime_t;		


static inline ktime_t ktime_set(const s64 secs, const unsigned long nsecs)
{
	if (unlikely(secs >= KTIME_SEC_MAX))
		return (ktime_t){ .tv64 = KTIME_MAX };

	return (ktime_t) { .tv64 = secs * NSEC_PER_SEC + (s64)nsecs };
}


#define ktime_sub(lhs, rhs) \
		({ (ktime_t){ .tv64 = (lhs).tv64 - (rhs).tv64 }; })


#define ktime_add(lhs, rhs) \
		({ (ktime_t){ .tv64 = (lhs).tv64 + (rhs).tv64 }; })


#define ktime_add_unsafe(lhs, rhs) \
		({ (ktime_t){ .tv64 = (u64) (lhs).tv64 + (rhs).tv64 }; })


#define ktime_add_ns(kt, nsval) \
		({ (ktime_t){ .tv64 = (kt).tv64 + (nsval) }; })


#define ktime_sub_ns(kt, nsval) \
		({ (ktime_t){ .tv64 = (kt).tv64 - (nsval) }; })


static inline ktime_t timespec_to_ktime(struct timespec ts)
{
	return ktime_set(ts.tv_sec, ts.tv_nsec);
}


static inline ktime_t timespec64_to_ktime(struct timespec64 ts)
{
	return ktime_set(ts.tv_sec, ts.tv_nsec);
}


static inline ktime_t timeval_to_ktime(struct timeval tv)
{
	return ktime_set(tv.tv_sec, tv.tv_usec * NSEC_PER_USEC);
}


#define ktime_to_timespec(kt)		ns_to_timespec((kt).tv64)


#define ktime_to_timespec64(kt)		ns_to_timespec64((kt).tv64)


#define ktime_to_timeval(kt)		ns_to_timeval((kt).tv64)


#define ktime_to_ns(kt)			((kt).tv64)



static inline int ktime_equal(const ktime_t cmp1, const ktime_t cmp2)
{
	return cmp1.tv64 == cmp2.tv64;
}


static inline int ktime_compare(const ktime_t cmp1, const ktime_t cmp2)
{
	if (cmp1.tv64 < cmp2.tv64)
		return -1;
	if (cmp1.tv64 > cmp2.tv64)
		return 1;
	return 0;
}


static inline bool ktime_after(const ktime_t cmp1, const ktime_t cmp2)
{
	return ktime_compare(cmp1, cmp2) > 0;
}


static inline bool ktime_before(const ktime_t cmp1, const ktime_t cmp2)
{
	return ktime_compare(cmp1, cmp2) < 0;
}

#if BITS_PER_LONG < 64
extern s64 __ktime_divns(const ktime_t kt, s64 div);
static inline s64 ktime_divns(const ktime_t kt, s64 div)
{
	
	BUG_ON(div < 0);
	if (__builtin_constant_p(div) && !(div >> 32)) {
		s64 ns = kt.tv64;
		u64 tmp = ns < 0 ? -ns : ns;

		do_div(tmp, div);
		return ns < 0 ? -tmp : tmp;
	} else {
		return __ktime_divns(kt, div);
	}
}
#else 
static inline s64 ktime_divns(const ktime_t kt, s64 div)
{
	
	WARN_ON(div < 0);
	return kt.tv64 / div;
}
#endif

static inline s64 ktime_to_us(const ktime_t kt)
{
	return ktime_divns(kt, NSEC_PER_USEC);
}

static inline s64 ktime_to_ms(const ktime_t kt)
{
	return ktime_divns(kt, NSEC_PER_MSEC);
}

static inline s64 ktime_us_delta(const ktime_t later, const ktime_t earlier)
{
       return ktime_to_us(ktime_sub(later, earlier));
}

static inline s64 ktime_ms_delta(const ktime_t later, const ktime_t earlier)
{
	return ktime_to_ms(ktime_sub(later, earlier));
}

static inline ktime_t ktime_add_us(const ktime_t kt, const u64 usec)
{
	return ktime_add_ns(kt, usec * NSEC_PER_USEC);
}

static inline ktime_t ktime_add_ms(const ktime_t kt, const u64 msec)
{
	return ktime_add_ns(kt, msec * NSEC_PER_MSEC);
}

static inline ktime_t ktime_sub_us(const ktime_t kt, const u64 usec)
{
	return ktime_sub_ns(kt, usec * NSEC_PER_USEC);
}

static inline ktime_t ktime_sub_ms(const ktime_t kt, const u64 msec)
{
	return ktime_sub_ns(kt, msec * NSEC_PER_MSEC);
}

extern ktime_t ktime_add_safe(const ktime_t lhs, const ktime_t rhs);


static inline __must_check bool ktime_to_timespec_cond(const ktime_t kt,
						       struct timespec *ts)
{
	if (kt.tv64) {
		*ts = ktime_to_timespec(kt);
		return true;
	} else {
		return false;
	}
}


static inline __must_check bool ktime_to_timespec64_cond(const ktime_t kt,
						       struct timespec64 *ts)
{
	if (kt.tv64) {
		*ts = ktime_to_timespec64(kt);
		return true;
	} else {
		return false;
	}
}


#define LOW_RES_NSEC		TICK_NSEC
#define KTIME_LOW_RES		(ktime_t){ .tv64 = LOW_RES_NSEC }

static inline ktime_t ns_to_ktime(u64 ns)
{
	static const ktime_t ktime_zero = { .tv64 = 0 };

	return ktime_add_ns(ktime_zero, ns);
}

static inline ktime_t ms_to_ktime(u64 ms)
{
	static const ktime_t ktime_zero = { .tv64 = 0 };

	return ktime_add_ms(ktime_zero, ms);
}

# include <linux/timekeeping.h>

#endif
