

#ifndef _LINUX_FSCACHE_CACHE_H
#define _LINUX_FSCACHE_CACHE_H

#include <linux/fscache.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

#define NR_MAXCACHES BITS_PER_LONG

struct fscache_cache;
struct fscache_cache_ops;
struct fscache_object;
struct fscache_operation;


struct fscache_cache_tag {
	struct list_head	link;
	struct fscache_cache	*cache;		
	unsigned long		flags;
#define FSCACHE_TAG_RESERVED	0		
	atomic_t		usage;
	char			name[0];	
};


struct fscache_cache {
	const struct fscache_cache_ops *ops;
	struct fscache_cache_tag *tag;		
	struct kobject		*kobj;		
	struct list_head	link;		
	size_t			max_index_size;	
	char			identifier[36];	

	
	struct work_struct	op_gc;		
	struct list_head	object_list;	
	struct list_head	op_gc_list;	
	spinlock_t		object_list_lock;
	spinlock_t		op_gc_list_lock;
	atomic_t		object_count;	
	struct fscache_object	*fsdef;		
	unsigned long		flags;
#define FSCACHE_IOERROR		0	
#define FSCACHE_CACHE_WITHDRAWN	1	
};

extern wait_queue_head_t fscache_cache_cleared_wq;


typedef void (*fscache_operation_release_t)(struct fscache_operation *op);
typedef void (*fscache_operation_processor_t)(struct fscache_operation *op);
typedef void (*fscache_operation_cancel_t)(struct fscache_operation *op);

enum fscache_operation_state {
	FSCACHE_OP_ST_BLANK,		
	FSCACHE_OP_ST_INITIALISED,	
	FSCACHE_OP_ST_PENDING,		
	FSCACHE_OP_ST_IN_PROGRESS,	
	FSCACHE_OP_ST_COMPLETE,		
	FSCACHE_OP_ST_CANCELLED,	
	FSCACHE_OP_ST_DEAD		
};

struct fscache_operation {
	struct work_struct	work;		
	struct list_head	pend_link;	
	struct fscache_object	*object;	

	unsigned long		flags;
#define FSCACHE_OP_TYPE		0x000f	
#define FSCACHE_OP_ASYNC	0x0001	
#define FSCACHE_OP_MYTHREAD	0x0002	
#define FSCACHE_OP_WAITING	4	
#define FSCACHE_OP_EXCLUSIVE	5	
#define FSCACHE_OP_DEC_READ_CNT	6	
#define FSCACHE_OP_UNUSE_COOKIE	7	
#define FSCACHE_OP_KEEP_FLAGS	0x00f0	

	enum fscache_operation_state state;
	atomic_t		usage;
	unsigned		debug_id;	

	
	fscache_operation_processor_t processor;

	
	fscache_operation_cancel_t cancel;

	
	fscache_operation_release_t release;
};

extern atomic_t fscache_op_debug_id;
extern void fscache_op_work_func(struct work_struct *work);

extern void fscache_enqueue_operation(struct fscache_operation *);
extern void fscache_op_complete(struct fscache_operation *, bool);
extern void fscache_put_operation(struct fscache_operation *);
extern void fscache_operation_init(struct fscache_operation *,
				   fscache_operation_processor_t,
				   fscache_operation_cancel_t,
				   fscache_operation_release_t);


struct fscache_retrieval {
	struct fscache_operation op;
	struct fscache_cookie	*cookie;	
	struct address_space	*mapping;	
	fscache_rw_complete_t	end_io_func;	
	void			*context;	
	struct list_head	to_do;		
	unsigned long		start_time;	
	atomic_t		n_pages;	
};

typedef int (*fscache_page_retrieval_func_t)(struct fscache_retrieval *op,
					     struct page *page,
					     gfp_t gfp);

typedef int (*fscache_pages_retrieval_func_t)(struct fscache_retrieval *op,
					      struct list_head *pages,
					      unsigned *nr_pages,
					      gfp_t gfp);


static inline
struct fscache_retrieval *fscache_get_retrieval(struct fscache_retrieval *op)
{
	atomic_inc(&op->op.usage);
	return op;
}


static inline void fscache_enqueue_retrieval(struct fscache_retrieval *op)
{
	fscache_enqueue_operation(&op->op);
}


static inline void fscache_retrieval_complete(struct fscache_retrieval *op,
					      int n_pages)
{
	atomic_sub(n_pages, &op->n_pages);
	if (atomic_read(&op->n_pages) <= 0)
		fscache_op_complete(&op->op, true);
}


static inline void fscache_put_retrieval(struct fscache_retrieval *op)
{
	fscache_put_operation(&op->op);
}


struct fscache_storage {
	struct fscache_operation op;
	pgoff_t			store_limit;	
};


struct fscache_cache_ops {
	
	const char *name;

	
	struct fscache_object *(*alloc_object)(struct fscache_cache *cache,
					       struct fscache_cookie *cookie);

	
	int (*lookup_object)(struct fscache_object *object);

	
	void (*lookup_complete)(struct fscache_object *object);

	
	struct fscache_object *(*grab_object)(struct fscache_object *object);

	
	int (*pin_object)(struct fscache_object *object);

	
	void (*unpin_object)(struct fscache_object *object);

	
	int (*check_consistency)(struct fscache_operation *op);

	
	void (*update_object)(struct fscache_object *object);

	
	void (*invalidate_object)(struct fscache_operation *op);

	
	void (*drop_object)(struct fscache_object *object);

	
	void (*put_object)(struct fscache_object *object);

	
	void (*sync_cache)(struct fscache_cache *cache);

	
	int (*attr_changed)(struct fscache_object *object);

	
	int (*reserve_space)(struct fscache_object *object, loff_t i_size);

	
	fscache_page_retrieval_func_t read_or_alloc_page;

	
	fscache_pages_retrieval_func_t read_or_alloc_pages;

	
	fscache_page_retrieval_func_t allocate_page;

	
	fscache_pages_retrieval_func_t allocate_pages;

	
	int (*write_page)(struct fscache_storage *op, struct page *page);

	
	void (*uncache_page)(struct fscache_object *object,
			     struct page *page);

	
	void (*dissociate_pages)(struct fscache_cache *cache);
};

extern struct fscache_cookie fscache_fsdef_index;


enum {
	FSCACHE_OBJECT_EV_NEW_CHILD,	
	FSCACHE_OBJECT_EV_PARENT_READY,	
	FSCACHE_OBJECT_EV_UPDATE,	
	FSCACHE_OBJECT_EV_INVALIDATE,	
	FSCACHE_OBJECT_EV_CLEARED,	
	FSCACHE_OBJECT_EV_ERROR,	
	FSCACHE_OBJECT_EV_KILL,		
	NR_FSCACHE_OBJECT_EVENTS
};

#define FSCACHE_OBJECT_EVENTS_MASK ((1UL << NR_FSCACHE_OBJECT_EVENTS) - 1)


struct fscache_transition {
	unsigned long events;
	const struct fscache_state *transit_to;
};

struct fscache_state {
	char name[24];
	char short_name[8];
	const struct fscache_state *(*work)(struct fscache_object *object,
					    int event);
	const struct fscache_transition transitions[];
};


struct fscache_object {
	const struct fscache_state *state;	
	const struct fscache_transition *oob_table; 
	int			debug_id;	
	int			n_children;	
	int			n_ops;		
	int			n_obj_ops;	
	int			n_in_progress;	
	int			n_exclusive;	
	atomic_t		n_reads;	
	spinlock_t		lock;		

	unsigned long		lookup_jif;	
	unsigned long		oob_event_mask;	
	unsigned long		event_mask;	
	unsigned long		events;		

	unsigned long		flags;
#define FSCACHE_OBJECT_LOCK		0	
#define FSCACHE_OBJECT_PENDING_WRITE	1	
#define FSCACHE_OBJECT_WAITING		2	
#define FSCACHE_OBJECT_IS_LIVE		3	
#define FSCACHE_OBJECT_IS_LOOKED_UP	4	
#define FSCACHE_OBJECT_IS_AVAILABLE	5	
#define FSCACHE_OBJECT_RETIRED		6	
#define FSCACHE_OBJECT_KILLED_BY_CACHE	7	
#define FSCACHE_OBJECT_RUN_AFTER_DEAD	8	

	struct list_head	cache_link;	
	struct hlist_node	cookie_link;	
	struct fscache_cache	*cache;		
	struct fscache_cookie	*cookie;	
	struct fscache_object	*parent;	
	struct work_struct	work;		
	struct list_head	dependents;	
	struct list_head	dep_link;	
	struct list_head	pending_ops;	
#ifdef CONFIG_FSCACHE_OBJECT_LIST
	struct rb_node		objlist_link;	
#endif
	pgoff_t			store_limit;	
	loff_t			store_limit_l;	
};

extern void fscache_object_init(struct fscache_object *, struct fscache_cookie *,
				struct fscache_cache *);
extern void fscache_object_destroy(struct fscache_object *);

extern void fscache_object_lookup_negative(struct fscache_object *object);
extern void fscache_obtained_object(struct fscache_object *object);

static inline bool fscache_object_is_live(struct fscache_object *object)
{
	return test_bit(FSCACHE_OBJECT_IS_LIVE, &object->flags);
}

static inline bool fscache_object_is_dying(struct fscache_object *object)
{
	return !fscache_object_is_live(object);
}

static inline bool fscache_object_is_available(struct fscache_object *object)
{
	return test_bit(FSCACHE_OBJECT_IS_AVAILABLE, &object->flags);
}

static inline bool fscache_cache_is_broken(struct fscache_object *object)
{
	return test_bit(FSCACHE_IOERROR, &object->cache->flags);
}

static inline bool fscache_object_is_active(struct fscache_object *object)
{
	return fscache_object_is_available(object) &&
		fscache_object_is_live(object) &&
		!fscache_cache_is_broken(object);
}


static inline void fscache_object_destroyed(struct fscache_cache *cache)
{
	if (atomic_dec_and_test(&cache->object_count))
		wake_up_all(&fscache_cache_cleared_wq);
}


static inline void fscache_object_lookup_error(struct fscache_object *object)
{
	set_bit(FSCACHE_OBJECT_EV_ERROR, &object->events);
}


static inline
void fscache_set_store_limit(struct fscache_object *object, loff_t i_size)
{
	object->store_limit_l = i_size;
	object->store_limit = i_size >> PAGE_SHIFT;
	if (i_size & ~PAGE_MASK)
		object->store_limit++;
}


static inline void fscache_end_io(struct fscache_retrieval *op,
				  struct page *page, int error)
{
	op->end_io_func(page, op->context, error);
}

static inline void __fscache_use_cookie(struct fscache_cookie *cookie)
{
	atomic_inc(&cookie->n_active);
}


static inline bool fscache_use_cookie(struct fscache_object *object)
{
	struct fscache_cookie *cookie = object->cookie;
	return atomic_inc_not_zero(&cookie->n_active) != 0;
}

static inline bool __fscache_unuse_cookie(struct fscache_cookie *cookie)
{
	return atomic_dec_and_test(&cookie->n_active);
}

static inline void __fscache_wake_unused_cookie(struct fscache_cookie *cookie)
{
	wake_up_atomic_t(&cookie->n_active);
}


static inline void fscache_unuse_cookie(struct fscache_object *object)
{
	struct fscache_cookie *cookie = object->cookie;
	if (__fscache_unuse_cookie(cookie))
		__fscache_wake_unused_cookie(cookie);
}


extern __printf(3, 4)
void fscache_init_cache(struct fscache_cache *cache,
			const struct fscache_cache_ops *ops,
			const char *idfmt, ...);

extern int fscache_add_cache(struct fscache_cache *cache,
			     struct fscache_object *fsdef,
			     const char *tagname);
extern void fscache_withdraw_cache(struct fscache_cache *cache);

extern void fscache_io_error(struct fscache_cache *cache);

extern void fscache_mark_page_cached(struct fscache_retrieval *op,
				     struct page *page);

extern void fscache_mark_pages_cached(struct fscache_retrieval *op,
				      struct pagevec *pagevec);

extern bool fscache_object_sleep_till_congested(signed long *timeoutp);

extern enum fscache_checkaux fscache_check_aux(struct fscache_object *object,
					       const void *data,
					       uint16_t datalen);

extern void fscache_object_retrying_stale(struct fscache_object *object);

enum fscache_why_object_killed {
	FSCACHE_OBJECT_IS_STALE,
	FSCACHE_OBJECT_NO_SPACE,
	FSCACHE_OBJECT_WAS_RETIRED,
	FSCACHE_OBJECT_WAS_CULLED,
};
extern void fscache_object_mark_killed(struct fscache_object *object,
				       enum fscache_why_object_killed why);

#endif 
