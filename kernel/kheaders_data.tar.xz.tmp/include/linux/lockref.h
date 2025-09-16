#ifndef __LINUX_LOCKREF_H
#define __LINUX_LOCKREF_H



#include <linux/spinlock.h>
#include <generated/bounds.h>

#define USE_CMPXCHG_LOCKREF \
	(IS_ENABLED(CONFIG_ARCH_USE_CMPXCHG_LOCKREF) && \
	 IS_ENABLED(CONFIG_SMP) && SPINLOCK_SIZE <= 4)

struct lockref {
	union {
#if USE_CMPXCHG_LOCKREF
		aligned_u64 lock_count;
#endif
		struct {
			spinlock_t lock;
			int count;
		};
	};
};

extern void lockref_get(struct lockref *);
extern int lockref_put_return(struct lockref *);
extern int lockref_get_not_zero(struct lockref *);
extern int lockref_get_or_lock(struct lockref *);
extern int lockref_put_or_lock(struct lockref *);

extern void lockref_mark_dead(struct lockref *);
extern int lockref_get_not_dead(struct lockref *);


static inline int __lockref_is_dead(const struct lockref *l)
{
	return ((int)l->count < 0);
}

#endif 
