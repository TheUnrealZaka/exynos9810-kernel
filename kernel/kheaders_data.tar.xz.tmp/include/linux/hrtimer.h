
#ifndef _LINUX_HRTIMER_H
#define _LINUX_HRTIMER_H

#include <linux/rbtree.h>
#include <linux/ktime.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/percpu.h>
#include <linux/timer.h>
#include <linux/timerqueue.h>

struct hrtimer_clock_base;
struct hrtimer_cpu_base;


enum hrtimer_mode {
	HRTIMER_MODE_ABS = 0x0,		
	HRTIMER_MODE_REL = 0x1,		
	HRTIMER_MODE_PINNED = 0x02,	
	HRTIMER_MODE_ABS_PINNED = 0x02,
	HRTIMER_MODE_REL_PINNED = 0x03,
};


enum hrtimer_restart {
	HRTIMER_NORESTART,	
	HRTIMER_RESTART,	
};


#define HRTIMER_STATE_INACTIVE	0x00
#define HRTIMER_STATE_ENQUEUED	0x01


struct hrtimer {
	struct timerqueue_node		node;
	ktime_t				_softexpires;
	enum hrtimer_restart		(*function)(struct hrtimer *);
	struct hrtimer_clock_base	*base;
	u8				state;
	u8				is_rel;
#ifdef CONFIG_TIMER_STATS
	int				start_pid;
	void				*start_site;
	char				start_comm[16];
#endif
	bool                            bounded_to_boot_cluster;
};


struct hrtimer_sleeper {
	struct hrtimer timer;
	struct task_struct *task;
};

#ifdef CONFIG_64BIT
# define HRTIMER_CLOCK_BASE_ALIGN	64
#else
# define HRTIMER_CLOCK_BASE_ALIGN	32
#endif


struct hrtimer_clock_base {
	struct hrtimer_cpu_base	*cpu_base;
	int			index;
	clockid_t		clockid;
	struct timerqueue_head	active;
	ktime_t			(*get_time)(void);
	ktime_t			offset;
} __attribute__((__aligned__(HRTIMER_CLOCK_BASE_ALIGN)));

enum  hrtimer_base_type {
	HRTIMER_BASE_MONOTONIC,
	HRTIMER_BASE_REALTIME,
	HRTIMER_BASE_BOOTTIME,
	HRTIMER_BASE_TAI,
	HRTIMER_MAX_CLOCK_BASES,
};


struct hrtimer_cpu_base {
	raw_spinlock_t			lock;
	seqcount_t			seq;
	struct hrtimer			*running;
	unsigned int			cpu;
	unsigned int			active_bases;
	unsigned int			clock_was_set_seq;
	bool				migration_enabled;
	bool				nohz_active;
#ifdef CONFIG_HIGH_RES_TIMERS
	unsigned int			in_hrtirq	: 1,
					hres_active	: 1,
					hang_detected	: 1;
	ktime_t				expires_next;
	struct hrtimer			*next_timer;
	unsigned int			nr_events;
	unsigned int			nr_retries;
	unsigned int			nr_hangs;
	unsigned int			max_hang_time;
#endif
	struct hrtimer_clock_base	clock_base[HRTIMER_MAX_CLOCK_BASES];
} ____cacheline_aligned;

static inline void hrtimer_set_expires(struct hrtimer *timer, ktime_t time)
{
	BUILD_BUG_ON(sizeof(struct hrtimer_clock_base) > HRTIMER_CLOCK_BASE_ALIGN);

	timer->node.expires = time;
	timer->_softexpires = time;
}

static inline void hrtimer_set_expires_range(struct hrtimer *timer, ktime_t time, ktime_t delta)
{
	timer->_softexpires = time;
	timer->node.expires = ktime_add_safe(time, delta);
}

static inline void hrtimer_set_expires_range_ns(struct hrtimer *timer, ktime_t time, u64 delta)
{
	timer->_softexpires = time;
	timer->node.expires = ktime_add_safe(time, ns_to_ktime(delta));
}

static inline void hrtimer_set_expires_tv64(struct hrtimer *timer, s64 tv64)
{
	timer->node.expires.tv64 = tv64;
	timer->_softexpires.tv64 = tv64;
}

static inline void hrtimer_add_expires(struct hrtimer *timer, ktime_t time)
{
	timer->node.expires = ktime_add_safe(timer->node.expires, time);
	timer->_softexpires = ktime_add_safe(timer->_softexpires, time);
}

static inline void hrtimer_add_expires_ns(struct hrtimer *timer, u64 ns)
{
	timer->node.expires = ktime_add_ns(timer->node.expires, ns);
	timer->_softexpires = ktime_add_ns(timer->_softexpires, ns);
}

static inline ktime_t hrtimer_get_expires(const struct hrtimer *timer)
{
	return timer->node.expires;
}

static inline ktime_t hrtimer_get_softexpires(const struct hrtimer *timer)
{
	return timer->_softexpires;
}

static inline s64 hrtimer_get_expires_tv64(const struct hrtimer *timer)
{
	return timer->node.expires.tv64;
}
static inline s64 hrtimer_get_softexpires_tv64(const struct hrtimer *timer)
{
	return timer->_softexpires.tv64;
}

static inline s64 hrtimer_get_expires_ns(const struct hrtimer *timer)
{
	return ktime_to_ns(timer->node.expires);
}

static inline ktime_t hrtimer_expires_remaining(const struct hrtimer *timer)
{
	return ktime_sub(timer->node.expires, timer->base->get_time());
}

static inline ktime_t hrtimer_cb_get_time(struct hrtimer *timer)
{
	return timer->base->get_time();
}

#ifdef CONFIG_HIGH_RES_TIMERS
struct clock_event_device;

extern void hrtimer_interrupt(struct clock_event_device *dev);

static inline int hrtimer_is_hres_active(struct hrtimer *timer)
{
	return timer->base->cpu_base->hres_active;
}

extern void hrtimer_peek_ahead_timers(void);


# define HIGH_RES_NSEC		1
# define KTIME_HIGH_RES		(ktime_t) { .tv64 = HIGH_RES_NSEC }
# define MONOTONIC_RES_NSEC	HIGH_RES_NSEC
# define KTIME_MONOTONIC_RES	KTIME_HIGH_RES

extern void clock_was_set_delayed(void);

extern unsigned int hrtimer_resolution;

#else

# define MONOTONIC_RES_NSEC	LOW_RES_NSEC
# define KTIME_MONOTONIC_RES	KTIME_LOW_RES

#define hrtimer_resolution	(unsigned int)LOW_RES_NSEC

static inline void hrtimer_peek_ahead_timers(void) { }

static inline int hrtimer_is_hres_active(struct hrtimer *timer)
{
	return 0;
}

static inline void clock_was_set_delayed(void) { }

#endif

static inline ktime_t
__hrtimer_expires_remaining_adjusted(const struct hrtimer *timer, ktime_t now)
{
	ktime_t rem = ktime_sub(timer->node.expires, now);

	
	if (IS_ENABLED(CONFIG_TIME_LOW_RES) && timer->is_rel)
		rem.tv64 -= hrtimer_resolution;
	return rem;
}

static inline ktime_t
hrtimer_expires_remaining_adjusted(const struct hrtimer *timer)
{
	return __hrtimer_expires_remaining_adjusted(timer,
						    timer->base->get_time());
}

extern void clock_was_set(void);
#ifdef CONFIG_TIMERFD
extern void timerfd_clock_was_set(void);
#else
static inline void timerfd_clock_was_set(void) { }
#endif
extern void hrtimers_resume(void);

DECLARE_PER_CPU(struct tick_device, tick_cpu_device);





extern void hrtimer_init(struct hrtimer *timer, clockid_t which_clock,
			 enum hrtimer_mode mode);

#ifdef CONFIG_DEBUG_OBJECTS_TIMERS
extern void hrtimer_init_on_stack(struct hrtimer *timer, clockid_t which_clock,
				  enum hrtimer_mode mode);

extern void destroy_hrtimer_on_stack(struct hrtimer *timer);
#else
static inline void hrtimer_init_on_stack(struct hrtimer *timer,
					 clockid_t which_clock,
					 enum hrtimer_mode mode)
{
	hrtimer_init(timer, which_clock, mode);
}
static inline void destroy_hrtimer_on_stack(struct hrtimer *timer) { }
#endif


extern void hrtimer_start_range_ns(struct hrtimer *timer, ktime_t tim,
				   u64 range_ns, const enum hrtimer_mode mode);


static inline void hrtimer_start(struct hrtimer *timer, ktime_t tim,
				 const enum hrtimer_mode mode)
{
	hrtimer_start_range_ns(timer, tim, 0, mode);
}

extern int hrtimer_cancel(struct hrtimer *timer);
extern int hrtimer_try_to_cancel(struct hrtimer *timer);

static inline void hrtimer_start_expires(struct hrtimer *timer,
					 enum hrtimer_mode mode)
{
	u64 delta;
	ktime_t soft, hard;
	soft = hrtimer_get_softexpires(timer);
	hard = hrtimer_get_expires(timer);
	delta = ktime_to_ns(ktime_sub(hard, soft));
	hrtimer_start_range_ns(timer, soft, delta, mode);
}

static inline void hrtimer_restart(struct hrtimer *timer)
{
	hrtimer_start_expires(timer, HRTIMER_MODE_ABS);
}


extern ktime_t __hrtimer_get_remaining(const struct hrtimer *timer, bool adjust);

static inline ktime_t hrtimer_get_remaining(const struct hrtimer *timer)
{
	return __hrtimer_get_remaining(timer, false);
}

extern u64 hrtimer_get_next_event(void);

extern bool hrtimer_active(const struct hrtimer *timer);


static inline bool hrtimer_is_queued(struct hrtimer *timer)
{
	
	return !!(READ_ONCE(timer->state) & HRTIMER_STATE_ENQUEUED);
}


static inline int hrtimer_callback_running(struct hrtimer *timer)
{
	return timer->base->cpu_base->running == timer;
}


extern u64
hrtimer_forward(struct hrtimer *timer, ktime_t now, ktime_t interval);


static inline u64 hrtimer_forward_now(struct hrtimer *timer,
				      ktime_t interval)
{
	return hrtimer_forward(timer, timer->base->get_time(), interval);
}


extern long hrtimer_nanosleep(struct timespec *rqtp,
			      struct timespec __user *rmtp,
			      const enum hrtimer_mode mode,
			      const clockid_t clockid);
extern long hrtimer_nanosleep_restart(struct restart_block *restart_block);

extern void hrtimer_init_sleeper(struct hrtimer_sleeper *sl,
				 struct task_struct *tsk);

extern int schedule_hrtimeout_range(ktime_t *expires, u64 delta,
						const enum hrtimer_mode mode);
extern int schedule_hrtimeout_range_clock(ktime_t *expires,
					  u64 delta,
					  const enum hrtimer_mode mode,
					  int clock);
extern int schedule_hrtimeout(ktime_t *expires, const enum hrtimer_mode mode);


extern void hrtimer_run_queues(void);


extern void __init hrtimers_init(void);


extern void sysrq_timer_list_show(void);

int hrtimers_prepare_cpu(unsigned int cpu);
#ifdef CONFIG_HOTPLUG_CPU
int hrtimers_dead_cpu(unsigned int cpu);
void save_pcpu_tick(int cpu);
void restore_pcpu_tick(int cpu);
#else
#define hrtimers_dead_cpu	NULL
#endif

#endif
