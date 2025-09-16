#ifndef _LINUX_RCULIST_NULLS_H
#define _LINUX_RCULIST_NULLS_H

#ifdef __KERNEL__


#include <linux/list_nulls.h>
#include <linux/rcupdate.h>


static inline void hlist_nulls_del_init_rcu(struct hlist_nulls_node *n)
{
	if (!hlist_nulls_unhashed(n)) {
		__hlist_nulls_del(n);
		WRITE_ONCE(n->pprev, NULL);
	}
}

#define hlist_nulls_first_rcu(head) \
	(*((struct hlist_nulls_node __rcu __force **)&(head)->first))

#define hlist_nulls_next_rcu(node) \
	(*((struct hlist_nulls_node __rcu __force **)&(node)->next))


static inline void hlist_nulls_del_rcu(struct hlist_nulls_node *n)
{
	__hlist_nulls_del(n);
	WRITE_ONCE(n->pprev, LIST_POISON2);
}


static inline void hlist_nulls_add_head_rcu(struct hlist_nulls_node *n,
					struct hlist_nulls_head *h)
{
	struct hlist_nulls_node *first = h->first;

	n->next = first;
	WRITE_ONCE(n->pprev, &h->first);
	rcu_assign_pointer(hlist_nulls_first_rcu(h), n);
	if (!is_a_nulls(first))
		WRITE_ONCE(first->pprev, &n->next);
}


static inline void hlist_nulls_add_tail_rcu(struct hlist_nulls_node *n,
					    struct hlist_nulls_head *h)
{
	struct hlist_nulls_node *i, *last = NULL;

	
	for (i = h->first; !is_a_nulls(i); i = i->next)
		last = i;

	if (last) {
		n->next = last->next;
		n->pprev = &last->next;
		rcu_assign_pointer(hlist_next_rcu(last), n);
	} else {
		hlist_nulls_add_head_rcu(n, h);
	}
}


#define hlist_nulls_for_each_entry_rcu(tpos, pos, head, member)			\
	for (({barrier();}),							\
	     pos = rcu_dereference_raw(hlist_nulls_first_rcu(head));		\
		(!is_a_nulls(pos)) &&						\
		({ tpos = hlist_nulls_entry(pos, typeof(*tpos), member); 1; }); \
		pos = rcu_dereference_raw(hlist_nulls_next_rcu(pos)))


#define hlist_nulls_for_each_entry_safe(tpos, pos, head, member)		\
	for (({barrier();}),							\
	     pos = rcu_dereference_raw(hlist_nulls_first_rcu(head));		\
		(!is_a_nulls(pos)) &&						\
		({ tpos = hlist_nulls_entry(pos, typeof(*tpos), member);	\
		   pos = rcu_dereference_raw(hlist_nulls_next_rcu(pos)); 1; });)
#endif
#endif
