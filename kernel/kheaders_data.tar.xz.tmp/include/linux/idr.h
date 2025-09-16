

#ifndef __IDR_H__
#define __IDR_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/init.h>
#include <linux/rcupdate.h>


#define IDR_BITS 8
#define IDR_SIZE (1 << IDR_BITS)
#define IDR_MASK ((1 << IDR_BITS)-1)

struct idr_layer {
	int			prefix;	
	int			layer;	
	struct idr_layer __rcu	*ary[1<<IDR_BITS];
	int			count;	
	union {
		
		DECLARE_BITMAP(bitmap, IDR_SIZE);
		struct rcu_head		rcu_head;
	};
};

struct idr {
	struct idr_layer __rcu	*hint;	
	struct idr_layer __rcu	*top;
	int			layers;	
	int			cur;	
	spinlock_t		lock;
	int			id_free_cnt;
	struct idr_layer	*id_free;
};

#define IDR_INIT(name)							\
{									\
	.lock			= __SPIN_LOCK_UNLOCKED(name.lock),	\
}
#define DEFINE_IDR(name)	struct idr name = IDR_INIT(name)





void *idr_find_slowpath(struct idr *idp, int id);
void idr_preload(gfp_t gfp_mask);
int idr_alloc(struct idr *idp, void *ptr, int start, int end, gfp_t gfp_mask);
int idr_alloc_cyclic(struct idr *idr, void *ptr, int start, int end, gfp_t gfp_mask);
int idr_for_each(struct idr *idp,
		 int (*fn)(int id, void *p, void *data), void *data);
void *idr_get_next(struct idr *idp, int *nextid);
void *idr_replace(struct idr *idp, void *ptr, int id);
void idr_remove(struct idr *idp, int id);
void idr_destroy(struct idr *idp);
void idr_init(struct idr *idp);
bool idr_is_empty(struct idr *idp);


static inline void idr_preload_end(void)
{
	preempt_enable();
}


static inline void *idr_find(struct idr *idr, int id)
{
	struct idr_layer *hint = rcu_dereference_raw(idr->hint);

	if (hint && (id & ~IDR_MASK) == hint->prefix)
		return rcu_dereference_raw(hint->ary[id & IDR_MASK]);

	return idr_find_slowpath(idr, id);
}


#define idr_for_each_entry(idp, entry, id)			\
	for (id = 0; ((entry) = idr_get_next(idp, &(id))) != NULL; ++id)


#define idr_for_each_entry_continue(idp, entry, id)			\
	for ((entry) = idr_get_next((idp), &(id));			\
	     entry;							\
	     ++id, (entry) = idr_get_next((idp), &(id)))


#define IDA_CHUNK_SIZE		128	
#define IDA_BITMAP_LONGS	(IDA_CHUNK_SIZE / sizeof(long) - 1)
#define IDA_BITMAP_BITS 	(IDA_BITMAP_LONGS * sizeof(long) * 8)

struct ida_bitmap {
	long			nr_busy;
	unsigned long		bitmap[IDA_BITMAP_LONGS];
};

struct ida {
	struct idr		idr;
	struct ida_bitmap	*free_bitmap;
};

#define IDA_INIT(name)		{ .idr = IDR_INIT((name).idr), .free_bitmap = NULL, }
#define DEFINE_IDA(name)	struct ida name = IDA_INIT(name)

int ida_pre_get(struct ida *ida, gfp_t gfp_mask);
int ida_get_new_above(struct ida *ida, int starting_id, int *p_id);
void ida_remove(struct ida *ida, int id);
void ida_destroy(struct ida *ida);
void ida_init(struct ida *ida);

int ida_simple_get(struct ida *ida, unsigned int start, unsigned int end,
		   gfp_t gfp_mask);
void ida_simple_remove(struct ida *ida, unsigned int id);


static inline int ida_get_new(struct ida *ida, int *p_id)
{
	return ida_get_new_above(ida, 0, p_id);
}

static inline bool ida_is_empty(struct ida *ida)
{
	return idr_is_empty(&ida->idr);
}

void __init idr_init_cache(void);

#endif 
