#ifndef _LINUX_LIST_NULLS_H
#define _LINUX_LIST_NULLS_H

#include <linux/poison.h>
#include <linux/const.h>



struct hlist_nulls_head {
	struct hlist_nulls_node *first;
};

struct hlist_nulls_node {
	struct hlist_nulls_node *next, **pprev;
};
#define NULLS_MARKER(value) (1UL | (((long)value) << 1))
#define INIT_HLIST_NULLS_HEAD(ptr, nulls) \
	((ptr)->first = (struct hlist_nulls_node *) NULLS_MARKER(nulls))

#define hlist_nulls_entry(ptr, type, member) container_of(ptr,type,member)

#define hlist_nulls_entry_safe(ptr, type, member) \
	({ typeof(ptr) ____ptr = (ptr); \
	   !is_a_nulls(____ptr) ? hlist_nulls_entry(____ptr, type, member) : NULL; \
	})

static inline int is_a_nulls(const struct hlist_nulls_node *ptr)
{
	return ((unsigned long)ptr & 1);
}


static inline unsigned long get_nulls_value(const struct hlist_nulls_node *ptr)
{
	return ((unsigned long)ptr) >> 1;
}

static inline int hlist_nulls_unhashed(const struct hlist_nulls_node *h)
{
	return !h->pprev;
}

static inline int hlist_nulls_empty(const struct hlist_nulls_head *h)
{
	return is_a_nulls(READ_ONCE(h->first));
}

static inline void hlist_nulls_add_head(struct hlist_nulls_node *n,
					struct hlist_nulls_head *h)
{
	struct hlist_nulls_node *first = h->first;

	n->next = first;
	WRITE_ONCE(n->pprev, &h->first);
	h->first = n;
	if (!is_a_nulls(first))
		WRITE_ONCE(first->pprev, &n->next);
}

static inline void __hlist_nulls_del(struct hlist_nulls_node *n)
{
	struct hlist_nulls_node *next = n->next;
	struct hlist_nulls_node **pprev = n->pprev;

	WRITE_ONCE(*pprev, next);
	if (!is_a_nulls(next))
		WRITE_ONCE(next->pprev, pprev);
}

static inline void hlist_nulls_del(struct hlist_nulls_node *n)
{
	__hlist_nulls_del(n);
	WRITE_ONCE(n->pprev, LIST_POISON2);
}


#define hlist_nulls_for_each_entry(tpos, pos, head, member)		       \
	for (pos = (head)->first;					       \
	     (!is_a_nulls(pos)) &&					       \
		({ tpos = hlist_nulls_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)


#define hlist_nulls_for_each_entry_from(tpos, pos, member)	\
	for (; (!is_a_nulls(pos)) && 				\
		({ tpos = hlist_nulls_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

#endif
