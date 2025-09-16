#ifndef _LINUX_TIMEKEEPING_H
#define _LINUX_TIMEKEEPING_H

#include <linux/errno.h>



void timekeeping_init(void);
extern int timekeeping_suspended;


extern void do_gettimeofday(struct timeval *tv);
extern int do_settimeofday64(const struct timespec64 *ts);
extern int do_sys_settimeofday64(const struct timespec64 *tv,
				 const struct timezone *tz);
static inline int do_sys_settimeofday(const struct timespec *tv,
				      const struct timezone *tz)
{
	struct timespec64 ts64;

	if (!tv)
		return do_sys_settimeofday64(NULL, tz);

	if (!timespec_valid(tv))
		return -EINVAL;

	ts64 = timespec_to_timespec64(*tv);
	return do_sys_settimeofday64(&ts64, tz);
}


unsigned long get_seconds(void);
struct timespec64 current_kernel_time64(void);

struct timespec __current_kernel_time(void);

static inline struct timespec current_kernel_time(void)
{
	struct timespec64 now = current_kernel_time64();

	return timespec64_to_timespec(now);
}


struct timespec64 get_monotonic_coarse64(void);
extern void getrawmonotonic64(struct timespec64 *ts);
extern void ktime_get_ts64(struct timespec64 *ts);
extern time64_t ktime_get_seconds(void);
extern time64_t ktime_get_real_seconds(void);

extern int __getnstimeofday64(struct timespec64 *tv);
extern void getnstimeofday64(struct timespec64 *tv);
extern void getboottime64(struct timespec64 *ts);

#if BITS_PER_LONG == 64

static inline int do_settimeofday(const struct timespec *ts)
{
	return do_settimeofday64(ts);
}

static inline int __getnstimeofday(struct timespec *ts)
{
	return __getnstimeofday64(ts);
}

static inline void getnstimeofday(struct timespec *ts)
{
	getnstimeofday64(ts);
}

static inline void ktime_get_ts(struct timespec *ts)
{
	ktime_get_ts64(ts);
}

static inline void ktime_get_real_ts(struct timespec *ts)
{
	getnstimeofday64(ts);
}

static inline void getrawmonotonic(struct timespec *ts)
{
	getrawmonotonic64(ts);
}

static inline struct timespec get_monotonic_coarse(void)
{
	return get_monotonic_coarse64();
}

static inline void getboottime(struct timespec *ts)
{
	return getboottime64(ts);
}
#else

static inline int do_settimeofday(const struct timespec *ts)
{
	struct timespec64 ts64;

	ts64 = timespec_to_timespec64(*ts);
	return do_settimeofday64(&ts64);
}

static inline int __getnstimeofday(struct timespec *ts)
{
	struct timespec64 ts64;
	int ret = __getnstimeofday64(&ts64);

	*ts = timespec64_to_timespec(ts64);
	return ret;
}

static inline void getnstimeofday(struct timespec *ts)
{
	struct timespec64 ts64;

	getnstimeofday64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void ktime_get_ts(struct timespec *ts)
{
	struct timespec64 ts64;

	ktime_get_ts64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void ktime_get_real_ts(struct timespec *ts)
{
	struct timespec64 ts64;

	getnstimeofday64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void getrawmonotonic(struct timespec *ts)
{
	struct timespec64 ts64;

	getrawmonotonic64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline struct timespec get_monotonic_coarse(void)
{
	return timespec64_to_timespec(get_monotonic_coarse64());
}

static inline void getboottime(struct timespec *ts)
{
	struct timespec64 ts64;

	getboottime64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}
#endif

#define ktime_get_real_ts64(ts)	getnstimeofday64(ts)



enum tk_offsets {
	TK_OFFS_REAL,
	TK_OFFS_BOOT,
	TK_OFFS_TAI,
	TK_OFFS_MAX,
};

extern ktime_t ktime_get(void);
extern ktime_t ktime_get_with_offset(enum tk_offsets offs);
extern ktime_t ktime_mono_to_any(ktime_t tmono, enum tk_offsets offs);
extern ktime_t ktime_get_raw(void);
extern u32 ktime_get_resolution_ns(void);


static inline ktime_t ktime_get_real(void)
{
	return ktime_get_with_offset(TK_OFFS_REAL);
}


static inline ktime_t ktime_get_boottime(void)
{
	return ktime_get_with_offset(TK_OFFS_BOOT);
}


static inline ktime_t ktime_get_clocktai(void)
{
	return ktime_get_with_offset(TK_OFFS_TAI);
}


static inline ktime_t ktime_mono_to_real(ktime_t mono)
{
	return ktime_mono_to_any(mono, TK_OFFS_REAL);
}

static inline u64 ktime_get_ns(void)
{
	return ktime_to_ns(ktime_get());
}

static inline u64 ktime_get_real_ns(void)
{
	return ktime_to_ns(ktime_get_real());
}

static inline u64 ktime_get_boot_ns(void)
{
	return ktime_to_ns(ktime_get_boottime());
}

static inline u64 ktime_get_tai_ns(void)
{
	return ktime_to_ns(ktime_get_clocktai());
}

static inline u64 ktime_get_raw_ns(void)
{
	return ktime_to_ns(ktime_get_raw());
}

extern u64 ktime_get_mono_fast_ns(void);
extern u64 ktime_get_raw_fast_ns(void);
extern u64 ktime_get_boot_fast_ns(void);


static inline void get_monotonic_boottime(struct timespec *ts)
{
	*ts = ktime_to_timespec(ktime_get_boottime());
}

static inline void get_monotonic_boottime64(struct timespec64 *ts)
{
	*ts = ktime_to_timespec64(ktime_get_boottime());
}

static inline void timekeeping_clocktai(struct timespec *ts)
{
	*ts = ktime_to_timespec(ktime_get_clocktai());
}


extern bool timekeeping_rtc_skipsuspend(void);
extern bool timekeeping_rtc_skipresume(void);

extern void timekeeping_inject_sleeptime64(struct timespec64 *delta);


extern void ktime_get_raw_and_real_ts64(struct timespec64 *ts_raw,
				        struct timespec64 *ts_real);


struct system_time_snapshot {
	cycle_t		cycles;
	ktime_t		real;
	ktime_t		raw;
	unsigned int	clock_was_set_seq;
	u8		cs_was_changed_seq;
};


struct system_device_crosststamp {
	ktime_t device;
	ktime_t sys_realtime;
	ktime_t sys_monoraw;
};


struct system_counterval_t {
	cycle_t			cycles;
	struct clocksource	*cs;
};


extern int get_device_system_crosststamp(
			int (*get_time_fn)(ktime_t *device_time,
				struct system_counterval_t *system_counterval,
				void *ctx),
			void *ctx,
			struct system_time_snapshot *history,
			struct system_device_crosststamp *xtstamp);


extern void ktime_get_snapshot(struct system_time_snapshot *systime_snapshot);


extern int persistent_clock_is_local;

extern void read_persistent_clock(struct timespec *ts);
extern void read_persistent_clock64(struct timespec64 *ts);
extern void read_boot_clock64(struct timespec64 *ts);
extern int update_persistent_clock(struct timespec now);
extern int update_persistent_clock64(struct timespec64 now);


#endif
