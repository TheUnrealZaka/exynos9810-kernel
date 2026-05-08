#ifndef LLIST_H
#define LLIST_H


#include <linux/atomic.h>
#include <linux/kernel.h>

struct llist_head {
	struct llist_node *first;
};

struct llist_node {
	struct llist_node *next;
};

#define LLIST_HEAD_INIT(name)	{ NULL }
#define LLIST_HEAD(name)	struct llist_head name = LLIST_HEAD_INIT(name)


static inline void init_llist_head(struct llist_head *list)
{
	list->first = NULL;
}


#define llist_entry(ptr, type, member)		\
	container_of(ptr, type, member)


#define member_address_is_nonnull(ptr, member)	\
	((uintptr_t)(ptr) + offsetof(typeof(*(ptr)), member) != 0)


#define llist_for_each(pos, node)			\
	for ((pos) = (node); pos; (pos) = (pos)->next)


#define llist_for_each_safe(pos, n, node)			\
	for ((pos) = (node); (pos) && ((n) = (pos)->next, true); (pos) = (n))


#define llist_for_each_entry(pos, node, member)				\
	for ((pos) = llist_entry((node), typeof(*(pos)), member);	\
	     member_address_is_nonnull(pos, member);			\
	     (pos) = llist_entry((pos)->member.next, typeof(*(pos)), member))


#define llist_for_each_entry_safe(pos, n, node, member)			       \
	for (pos = llist_entry((node), typeof(*pos), member);		       \
	     member_address_is_nonnull(pos, member) &&			       \
	        (n = llist_entry(pos->member.next, typeof(*n), member), true); \
	     pos = n)


static inline bool llist_empty(const struct llist_head *head)
{
	return ACCESS_ONCE(head->first) == NULL;
}

static inline struct llist_node *llist_next(struct llist_node *node)
{
	return node->next;
}

extern bool llist_add_batch(struct llist_node *new_first,
			    struct llist_node *new_last,
			    struct llist_head *head);

static inline bool llist_add(struct llist_node *new, struct llist_head *head)
{
	return llist_add_batch(new, new, head);
}


static inline struct llist_node *llist_del_all(struct llist_head *head)
{
	return xchg(&head->first, NULL);
}

extern struct llist_node *llist_del_first(struct llist_head *head);

struct llist_node *llist_reverse_order(struct llist_node *head);

#endif 
