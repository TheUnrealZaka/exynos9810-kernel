#ifndef _LINUX_CONTEXT_TRACKING_STATE_H
#define _LINUX_CONTEXT_TRACKING_STATE_H

#include <linux/percpu.h>
#include <linux/static_key.h>

struct context_tracking {
	
	bool active;
	int recursion;
	enum ctx_state {
		CONTEXT_DISABLED = -1,	
		CONTEXT_KERNEL = 0,
		CONTEXT_USER,
		CONTEXT_GUEST,
	} state;
};

#ifdef CONFIG_CONTEXT_TRACKING
extern struct static_key_false context_tracking_enabled;
DECLARE_PER_CPU(struct context_tracking, context_tracking);

static inline bool context_tracking_is_enabled(void)
{
	return static_branch_unlikely(&context_tracking_enabled);
}

static inline bool context_tracking_cpu_is_enabled(void)
{
	return __this_cpu_read(context_tracking.active);
}

static inline bool context_tracking_in_user(void)
{
	return __this_cpu_read(context_tracking.state) == CONTEXT_USER;
}
#else
static inline bool context_tracking_in_user(void) { return false; }
static inline bool context_tracking_active(void) { return false; }
static inline bool context_tracking_is_enabled(void) { return false; }
static inline bool context_tracking_cpu_is_enabled(void) { return false; }
#endif 

#endif
