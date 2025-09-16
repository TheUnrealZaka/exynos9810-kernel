#ifndef __LINUX_SEQLOCK_H
#define __LINUX_SEQLOCK_H


#include <linux/spinlock.h>
#include <linux/preempt.h>
#include <linux/lockdep.h>
#include <linux/compiler.h>
#include <asm/processor.h>


typedef struct seqcount {
	unsigned sequence;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} seqcount_t;

static inline void __seqcount_init(seqcount_t *s, const char *name,
					  struct lock_class_key *key)
{
	
	lockdep_init_map(&s->dep_map, name, key, 0);
	s->sequence = 0;
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define SEQCOUNT_DEP_MAP_INIT(lockname) \
		.dep_map = { .name = #lockname } \

# define seqcount_init(s)				\
	do {						\
		static struct lock_class_key __key;	\
		__seqcount_init((s), #s, &__key);	\
	} while (0)

static inline void seqcount_lockdep_reader_access(const seqcount_t *s)
{
	seqcount_t *l = (seqcount_t *)s;
	unsigned long flags;

	local_irq_save(flags);
	seqcount_acquire_read(&l->dep_map, 0, 0, _RET_IP_);
	seqcount_release(&l->dep_map, 1, _RET_IP_);
	local_irq_restore(flags);
}

#else
# define SEQCOUNT_DEP_MAP_INIT(lockname)
# define seqcount_init(s) __seqcount_init(s, NULL, NULL)
# define seqcount_lockdep_reader_access(x)
#endif

#define SEQCNT_ZERO(lockname) { .sequence = 0, SEQCOUNT_DEP_MAP_INIT(lockname)}



static inline unsigned __read_seqcount_begin(const seqcount_t *s)
{
	unsigned ret;

repeat:
	ret = READ_ONCE(s->sequence);
	if (unlikely(ret & 1)) {
		cpu_relax();
		goto repeat;
	}
	return ret;
}


static inline unsigned raw_read_seqcount(const seqcount_t *s)
{
	unsigned ret = READ_ONCE(s->sequence);
	smp_rmb();
	return ret;
}


static inline unsigned raw_read_seqcount_begin(const seqcount_t *s)
{
	unsigned ret = __read_seqcount_begin(s);
	smp_rmb();
	return ret;
}


static inline unsigned read_seqcount_begin(const seqcount_t *s)
{
	seqcount_lockdep_reader_access(s);
	return raw_read_seqcount_begin(s);
}


static inline unsigned raw_seqcount_begin(const seqcount_t *s)
{
	unsigned ret = READ_ONCE(s->sequence);
	smp_rmb();
	return ret & ~1;
}


static inline int __read_seqcount_retry(const seqcount_t *s, unsigned start)
{
	return unlikely(s->sequence != start);
}


static inline int read_seqcount_retry(const seqcount_t *s, unsigned start)
{
	smp_rmb();
	return __read_seqcount_retry(s, start);
}



static inline void raw_write_seqcount_begin(seqcount_t *s)
{
	s->sequence++;
	smp_wmb();
}

static inline void raw_write_seqcount_end(seqcount_t *s)
{
	smp_wmb();
	s->sequence++;
}


static inline void raw_write_seqcount_barrier(seqcount_t *s)
{
	s->sequence++;
	smp_wmb();
	s->sequence++;
}

static inline int raw_read_seqcount_latch(seqcount_t *s)
{
	int seq = READ_ONCE(s->sequence);
	
	smp_read_barrier_depends();
	return seq;
}


static inline void raw_write_seqcount_latch(seqcount_t *s)
{
       smp_wmb();      
       s->sequence++;
       smp_wmb();      
}


static inline void write_seqcount_begin_nested(seqcount_t *s, int subclass)
{
	raw_write_seqcount_begin(s);
	seqcount_acquire(&s->dep_map, subclass, 0, _RET_IP_);
}

static inline void write_seqcount_begin(seqcount_t *s)
{
	write_seqcount_begin_nested(s, 0);
}

static inline void write_seqcount_end(seqcount_t *s)
{
	seqcount_release(&s->dep_map, 1, _RET_IP_);
	raw_write_seqcount_end(s);
}


static inline void write_seqcount_invalidate(seqcount_t *s)
{
	smp_wmb();
	s->sequence+=2;
}

typedef struct {
	struct seqcount seqcount;
	spinlock_t lock;
} seqlock_t;


#define __SEQLOCK_UNLOCKED(lockname)			\
	{						\
		.seqcount = SEQCNT_ZERO(lockname),	\
		.lock =	__SPIN_LOCK_UNLOCKED(lockname)	\
	}

#define seqlock_init(x)					\
	do {						\
		seqcount_init(&(x)->seqcount);		\
		spin_lock_init(&(x)->lock);		\
	} while (0)

#define DEFINE_SEQLOCK(x) \
		seqlock_t x = __SEQLOCK_UNLOCKED(x)


static inline unsigned read_seqbegin(const seqlock_t *sl)
{
	return read_seqcount_begin(&sl->seqcount);
}

static inline unsigned read_seqretry(const seqlock_t *sl, unsigned start)
{
	return read_seqcount_retry(&sl->seqcount, start);
}


static inline void write_seqlock(seqlock_t *sl)
{
	spin_lock(&sl->lock);
	write_seqcount_begin(&sl->seqcount);
}

static inline void write_sequnlock(seqlock_t *sl)
{
	write_seqcount_end(&sl->seqcount);
	spin_unlock(&sl->lock);
}

static inline void write_seqlock_bh(seqlock_t *sl)
{
	spin_lock_bh(&sl->lock);
	write_seqcount_begin(&sl->seqcount);
}

static inline void write_sequnlock_bh(seqlock_t *sl)
{
	write_seqcount_end(&sl->seqcount);
	spin_unlock_bh(&sl->lock);
}

static inline void write_seqlock_irq(seqlock_t *sl)
{
	spin_lock_irq(&sl->lock);
	write_seqcount_begin(&sl->seqcount);
}

static inline void write_sequnlock_irq(seqlock_t *sl)
{
	write_seqcount_end(&sl->seqcount);
	spin_unlock_irq(&sl->lock);
}

static inline unsigned long __write_seqlock_irqsave(seqlock_t *sl)
{
	unsigned long flags;

	spin_lock_irqsave(&sl->lock, flags);
	write_seqcount_begin(&sl->seqcount);
	return flags;
}

#define write_seqlock_irqsave(lock, flags)				\
	do { flags = __write_seqlock_irqsave(lock); } while (0)

static inline void
write_sequnlock_irqrestore(seqlock_t *sl, unsigned long flags)
{
	write_seqcount_end(&sl->seqcount);
	spin_unlock_irqrestore(&sl->lock, flags);
}


static inline void read_seqlock_excl(seqlock_t *sl)
{
	spin_lock(&sl->lock);
}

static inline void read_sequnlock_excl(seqlock_t *sl)
{
	spin_unlock(&sl->lock);
}


static inline void read_seqbegin_or_lock(seqlock_t *lock, int *seq)
{
	if (!(*seq & 1))	
		*seq = read_seqbegin(lock);
	else			
		read_seqlock_excl(lock);
}

static inline int need_seqretry(seqlock_t *lock, int seq)
{
	return !(seq & 1) && read_seqretry(lock, seq);
}

static inline void done_seqretry(seqlock_t *lock, int seq)
{
	if (seq & 1)
		read_sequnlock_excl(lock);
}

static inline void read_seqlock_excl_bh(seqlock_t *sl)
{
	spin_lock_bh(&sl->lock);
}

static inline void read_sequnlock_excl_bh(seqlock_t *sl)
{
	spin_unlock_bh(&sl->lock);
}

static inline void read_seqlock_excl_irq(seqlock_t *sl)
{
	spin_lock_irq(&sl->lock);
}

static inline void read_sequnlock_excl_irq(seqlock_t *sl)
{
	spin_unlock_irq(&sl->lock);
}

static inline unsigned long __read_seqlock_excl_irqsave(seqlock_t *sl)
{
	unsigned long flags;

	spin_lock_irqsave(&sl->lock, flags);
	return flags;
}

#define read_seqlock_excl_irqsave(lock, flags)				\
	do { flags = __read_seqlock_excl_irqsave(lock); } while (0)

static inline void
read_sequnlock_excl_irqrestore(seqlock_t *sl, unsigned long flags)
{
	spin_unlock_irqrestore(&sl->lock, flags);
}

static inline unsigned long
read_seqbegin_or_lock_irqsave(seqlock_t *lock, int *seq)
{
	unsigned long flags = 0;

	if (!(*seq & 1))	
		*seq = read_seqbegin(lock);
	else			
		read_seqlock_excl_irqsave(lock, flags);

	return flags;
}

static inline void
done_seqretry_irqrestore(seqlock_t *lock, int seq, unsigned long flags)
{
	if (seq & 1)
		read_sequnlock_excl_irqrestore(lock, flags);
}
#endif 
