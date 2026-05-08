
#ifndef __LINUX_LOCKDEP_H
#define __LINUX_LOCKDEP_H

struct task_struct;
struct lockdep_map;


extern int prove_locking;
extern int lock_stat;

#define MAX_LOCKDEP_SUBCLASSES		8UL

#ifdef CONFIG_LOCKDEP

#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/debug_locks.h>
#include <linux/stacktrace.h>


#define XXX_LOCK_USAGE_STATES		(1+3*4)


#define NR_LOCKDEP_CACHING_CLASSES	2


struct lockdep_subclass_key {
	char __one_byte;
} __attribute__ ((__packed__));

struct lock_class_key {
	struct lockdep_subclass_key	subkeys[MAX_LOCKDEP_SUBCLASSES];
};

extern struct lock_class_key __lockdep_no_validate__;

#define LOCKSTAT_POINTS		4


struct lock_class {
	
	struct hlist_node		hash_entry;

	
	struct list_head		lock_entry;

	struct lockdep_subclass_key	*key;
	unsigned int			subclass;
	unsigned int			dep_gen_id;

	
	unsigned long			usage_mask;
	struct stack_trace		usage_traces[XXX_LOCK_USAGE_STATES];

	
	struct list_head		locks_after, locks_before;

	
	unsigned int			version;

	
	unsigned long			ops;

	const char			*name;
	int				name_version;

#ifdef CONFIG_LOCK_STAT
	unsigned long			contention_point[LOCKSTAT_POINTS];
	unsigned long			contending_point[LOCKSTAT_POINTS];
#endif
};

#ifdef CONFIG_LOCK_STAT
struct lock_time {
	s64				min;
	s64				max;
	s64				total;
	unsigned long			nr;
};

enum bounce_type {
	bounce_acquired_write,
	bounce_acquired_read,
	bounce_contended_write,
	bounce_contended_read,
	nr_bounce_types,

	bounce_acquired = bounce_acquired_write,
	bounce_contended = bounce_contended_write,
};

struct lock_class_stats {
	unsigned long			contention_point[LOCKSTAT_POINTS];
	unsigned long			contending_point[LOCKSTAT_POINTS];
	struct lock_time		read_waittime;
	struct lock_time		write_waittime;
	struct lock_time		read_holdtime;
	struct lock_time		write_holdtime;
	unsigned long			bounces[nr_bounce_types];
};

struct lock_class_stats lock_stats(struct lock_class *class);
void clear_lock_stats(struct lock_class *class);
#endif


struct lockdep_map {
	struct lock_class_key		*key;
	struct lock_class		*class_cache[NR_LOCKDEP_CACHING_CLASSES];
	const char			*name;
#ifdef CONFIG_LOCK_STAT
	int				cpu;
	unsigned long			ip;
#endif
};

static inline void lockdep_copy_map(struct lockdep_map *to,
				    struct lockdep_map *from)
{
	int i;

	*to = *from;
	
	for (i = 0; i < NR_LOCKDEP_CACHING_CLASSES; i++)
		to->class_cache[i] = NULL;
}


struct lock_list {
	struct list_head		entry;
	struct lock_class		*class;
	struct stack_trace		trace;
	int				distance;

	
	struct lock_list		*parent;
};


struct lock_chain {
	
	unsigned int			irq_context :  2,
					depth       :  6,
					base	    : 24;
	
	struct hlist_node		entry;
	u64				chain_key;
};

#define MAX_LOCKDEP_KEYS_BITS		13

#define MAX_LOCKDEP_KEYS		((1UL << MAX_LOCKDEP_KEYS_BITS) - 1)

struct held_lock {
	
	u64				prev_chain_key;
	unsigned long			acquire_ip;
	struct lockdep_map		*instance;
	struct lockdep_map		*nest_lock;
#ifdef CONFIG_LOCK_STAT
	u64 				waittime_stamp;
	u64				holdtime_stamp;
#endif
	unsigned int			class_idx:MAX_LOCKDEP_KEYS_BITS;
	
	unsigned int irq_context:2; 
	unsigned int trylock:1;						

	unsigned int read:2;        
	unsigned int check:1;       
	unsigned int hardirqs_off:1;
	unsigned int references:12;					
	unsigned int pin_count;
};


extern void lockdep_info(void);
extern void lockdep_reset(void);
extern void lockdep_reset_lock(struct lockdep_map *lock);
extern void lockdep_free_key_range(void *start, unsigned long size);
extern asmlinkage void lockdep_sys_exit(void);

extern void lockdep_off(void);
extern void lockdep_on(void);



extern void lockdep_init_map(struct lockdep_map *lock, const char *name,
			     struct lock_class_key *key, int subclass);


#define STATIC_LOCKDEP_MAP_INIT(_name, _key) \
	{ .name = (_name), .key = (void *)(_key), }


#define lockdep_set_class(lock, key) \
		lockdep_init_map(&(lock)->dep_map, #key, key, 0)
#define lockdep_set_class_and_name(lock, key, name) \
		lockdep_init_map(&(lock)->dep_map, name, key, 0)
#define lockdep_set_class_and_subclass(lock, key, sub) \
		lockdep_init_map(&(lock)->dep_map, #key, key, sub)
#define lockdep_set_subclass(lock, sub)	\
		lockdep_init_map(&(lock)->dep_map, #lock, \
				 (lock)->dep_map.key, sub)

#define lockdep_set_novalidate_class(lock) \
	lockdep_set_class_and_name(lock, &__lockdep_no_validate__, #lock)

#define lockdep_match_class(lock, key) lockdep_match_key(&(lock)->dep_map, key)

static inline int lockdep_match_key(struct lockdep_map *lock,
				    struct lock_class_key *key)
{
	return lock->key == key;
}


extern void lock_acquire(struct lockdep_map *lock, unsigned int subclass,
			 int trylock, int read, int check,
			 struct lockdep_map *nest_lock, unsigned long ip);

extern void lock_release(struct lockdep_map *lock, int nested,
			 unsigned long ip);

#define lockdep_is_held(lock)	lock_is_held(&(lock)->dep_map)

extern int lock_is_held(struct lockdep_map *lock);

extern void lock_set_class(struct lockdep_map *lock, const char *name,
			   struct lock_class_key *key, unsigned int subclass,
			   unsigned long ip);

static inline void lock_set_subclass(struct lockdep_map *lock,
		unsigned int subclass, unsigned long ip)
{
	lock_set_class(lock, lock->name, lock->key, subclass, ip);
}

extern void lockdep_set_current_reclaim_state(gfp_t gfp_mask);
extern void lockdep_clear_current_reclaim_state(void);
extern void lockdep_trace_alloc(gfp_t mask);

struct pin_cookie { unsigned int val; };

#define NIL_COOKIE (struct pin_cookie){ .val = 0U, }

extern struct pin_cookie lock_pin_lock(struct lockdep_map *lock);
extern void lock_repin_lock(struct lockdep_map *lock, struct pin_cookie);
extern void lock_unpin_lock(struct lockdep_map *lock, struct pin_cookie);

# define INIT_LOCKDEP				.lockdep_recursion = 0, .lockdep_reclaim_gfp = 0,

#define lockdep_depth(tsk)	(debug_locks ? (tsk)->lockdep_depth : 0)

#define lockdep_assert_held(l)	do {				\
		WARN_ON(debug_locks && !lockdep_is_held(l));	\
	} while (0)

#define lockdep_assert_held_once(l)	do {				\
		WARN_ON_ONCE(debug_locks && !lockdep_is_held(l));	\
	} while (0)

#define lockdep_recursing(tsk)	((tsk)->lockdep_recursion)

#define lockdep_pin_lock(l)	lock_pin_lock(&(l)->dep_map)
#define lockdep_repin_lock(l,c)	lock_repin_lock(&(l)->dep_map, (c))
#define lockdep_unpin_lock(l,c)	lock_unpin_lock(&(l)->dep_map, (c))

#else 

static inline void lockdep_off(void)
{
}

static inline void lockdep_on(void)
{
}

# define lock_acquire(l, s, t, r, c, n, i)	do { } while (0)
# define lock_release(l, n, i)			do { } while (0)
# define lock_set_class(l, n, k, s, i)		do { } while (0)
# define lock_set_subclass(l, s, i)		do { } while (0)
# define lockdep_set_current_reclaim_state(g)	do { } while (0)
# define lockdep_clear_current_reclaim_state()	do { } while (0)
# define lockdep_trace_alloc(g)			do { } while (0)
# define lockdep_info()				do { } while (0)
# define lockdep_init_map(lock, name, key, sub) \
		do { (void)(name); (void)(key); } while (0)
# define lockdep_set_class(lock, key)		do { (void)(key); } while (0)
# define lockdep_set_class_and_name(lock, key, name) \
		do { (void)(key); (void)(name); } while (0)
#define lockdep_set_class_and_subclass(lock, key, sub) \
		do { (void)(key); } while (0)
#define lockdep_set_subclass(lock, sub)		do { } while (0)

#define lockdep_set_novalidate_class(lock) do { } while (0)



# define INIT_LOCKDEP
# define lockdep_reset()		do { debug_locks = 1; } while (0)
# define lockdep_free_key_range(start, size)	do { } while (0)
# define lockdep_sys_exit() 			do { } while (0)

struct lock_class_key { };

#define lockdep_depth(tsk)	(0)

#define lockdep_assert_held(l)			do { (void)(l); } while (0)
#define lockdep_assert_held_once(l)		do { (void)(l); } while (0)

#define lockdep_recursing(tsk)			(0)

struct pin_cookie { };

#define NIL_COOKIE (struct pin_cookie){ }

#define lockdep_pin_lock(l)			({ struct pin_cookie cookie; cookie; })
#define lockdep_repin_lock(l, c)		do { (void)(l); (void)(c); } while (0)
#define lockdep_unpin_lock(l, c)		do { (void)(l); (void)(c); } while (0)

#endif 

#ifdef CONFIG_LOCK_STAT

extern void lock_contended(struct lockdep_map *lock, unsigned long ip);
extern void lock_acquired(struct lockdep_map *lock, unsigned long ip);

#define LOCK_CONTENDED(_lock, try, lock)			\
do {								\
	if (!try(_lock)) {					\
		lock_contended(&(_lock)->dep_map, _RET_IP_);	\
		lock(_lock);					\
	}							\
	lock_acquired(&(_lock)->dep_map, _RET_IP_);			\
} while (0)

#define LOCK_CONTENDED_RETURN(_lock, try, lock)			\
({								\
	int ____err = 0;					\
	if (!try(_lock)) {					\
		lock_contended(&(_lock)->dep_map, _RET_IP_);	\
		____err = lock(_lock);				\
	}							\
	if (!____err)						\
		lock_acquired(&(_lock)->dep_map, _RET_IP_);	\
	____err;						\
})

#else 

#define lock_contended(lockdep_map, ip) do {} while (0)
#define lock_acquired(lockdep_map, ip) do {} while (0)

#define LOCK_CONTENDED(_lock, try, lock) \
	lock(_lock)

#define LOCK_CONTENDED_RETURN(_lock, try, lock) \
	lock(_lock)

#endif 

#ifdef CONFIG_LOCKDEP


#define LOCK_CONTENDED_FLAGS(_lock, try, lock, lockfl, flags) \
	LOCK_CONTENDED((_lock), (try), (lock))

#else 

#define LOCK_CONTENDED_FLAGS(_lock, try, lock, lockfl, flags) \
	lockfl((_lock), (flags))

#endif 

#ifdef CONFIG_TRACE_IRQFLAGS
extern void print_irqtrace_events(struct task_struct *curr);
#else
static inline void print_irqtrace_events(struct task_struct *curr)
{
}
#endif


#define SINGLE_DEPTH_NESTING			1



#define lock_acquire_exclusive(l, s, t, n, i)		lock_acquire(l, s, t, 0, 1, n, i)
#define lock_acquire_shared(l, s, t, n, i)		lock_acquire(l, s, t, 1, 1, n, i)
#define lock_acquire_shared_recursive(l, s, t, n, i)	lock_acquire(l, s, t, 2, 1, n, i)

#define spin_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
#define spin_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
#define spin_release(l, n, i)			lock_release(l, n, i)

#define rwlock_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
#define rwlock_acquire_read(l, s, t, i)		lock_acquire_shared_recursive(l, s, t, NULL, i)
#define rwlock_release(l, n, i)			lock_release(l, n, i)

#define seqcount_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
#define seqcount_acquire_read(l, s, t, i)	lock_acquire_shared_recursive(l, s, t, NULL, i)
#define seqcount_release(l, n, i)		lock_release(l, n, i)

#define mutex_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
#define mutex_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
#define mutex_release(l, n, i)			lock_release(l, n, i)

#define rwsem_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
#define rwsem_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
#define rwsem_acquire_read(l, s, t, i)		lock_acquire_shared(l, s, t, NULL, i)
#define rwsem_release(l, n, i)			lock_release(l, n, i)

#define lock_map_acquire(l)			lock_acquire_exclusive(l, 0, 0, NULL, _THIS_IP_)
#define lock_map_acquire_read(l)		lock_acquire_shared_recursive(l, 0, 0, NULL, _THIS_IP_)
#define lock_map_acquire_tryread(l)		lock_acquire_shared_recursive(l, 0, 1, NULL, _THIS_IP_)
#define lock_map_release(l)			lock_release(l, 1, _THIS_IP_)

#ifdef CONFIG_PROVE_LOCKING
# define might_lock(lock) 						\
do {									\
	typecheck(struct lockdep_map *, &(lock)->dep_map);		\
	lock_acquire(&(lock)->dep_map, 0, 0, 0, 1, NULL, _THIS_IP_);	\
	lock_release(&(lock)->dep_map, 0, _THIS_IP_);			\
} while (0)
# define might_lock_read(lock) 						\
do {									\
	typecheck(struct lockdep_map *, &(lock)->dep_map);		\
	lock_acquire(&(lock)->dep_map, 0, 0, 1, 1, NULL, _THIS_IP_);	\
	lock_release(&(lock)->dep_map, 0, _THIS_IP_);			\
} while (0)
#else
# define might_lock(lock) do { } while (0)
# define might_lock_read(lock) do { } while (0)
#endif

#ifdef CONFIG_LOCKDEP
void lockdep_rcu_suspicious(const char *file, const int line, const char *s);
#else
static inline void
lockdep_rcu_suspicious(const char *file, const int line, const char *s)
{
}
#endif

#endif 
