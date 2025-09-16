

#ifndef _KREF_H_
#define _KREF_H_

#include <linux/bug.h>
#include <linux/atomic.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

struct kref {
	atomic_t refcount;
};

#define KREF_INIT(n)	{ .refcount = ATOMIC_INIT(n), }


static inline void kref_init(struct kref *kref)
{
	atomic_set(&kref->refcount, 1);
}

static inline int kref_read(const struct kref *kref)
{
	return atomic_read(&kref->refcount);
}


static inline void kref_get(struct kref *kref)
{
	
	WARN_ON_ONCE(atomic_inc_return(&kref->refcount) < 2);
}


static inline int kref_sub(struct kref *kref, unsigned int count,
	     void (*release)(struct kref *kref))
{
	WARN_ON(release == NULL);

	if (atomic_sub_and_test((int) count, &kref->refcount)) {
		release(kref);
		return 1;
	}
	return 0;
}


static inline int kref_put(struct kref *kref, void (*release)(struct kref *kref))
{
	return kref_sub(kref, 1, release);
}

static inline int kref_put_mutex(struct kref *kref,
				 void (*release)(struct kref *kref),
				 struct mutex *lock)
{
	WARN_ON(release == NULL);
	if (unlikely(!atomic_add_unless(&kref->refcount, -1, 1))) {
		mutex_lock(lock);
		if (unlikely(!atomic_dec_and_test(&kref->refcount))) {
			mutex_unlock(lock);
			return 0;
		}
		release(kref);
		return 1;
	}
	return 0;
}


static inline int __must_check kref_get_unless_zero(struct kref *kref)
{
	return atomic_add_unless(&kref->refcount, 1, 0);
}
#endif 
