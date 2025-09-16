

#ifndef RB_TREE_LATCH_H
#define RB_TREE_LATCH_H

#include <linux/rbtree.h>
#include <linux/seqlock.h>

struct latch_tree_node {
	struct rb_node node[2];
};

struct latch_tree_root {
	seqcount_t	seq;
	struct rb_root	tree[2];
};


struct latch_tree_ops {
	bool (*less)(struct latch_tree_node *a, struct latch_tree_node *b);
	int  (*comp)(void *key,                 struct latch_tree_node *b);
};

static __always_inline struct latch_tree_node *
__lt_from_rb(struct rb_node *node, int idx)
{
	return container_of(node, struct latch_tree_node, node[idx]);
}

static __always_inline void
__lt_insert(struct latch_tree_node *ltn, struct latch_tree_root *ltr, int idx,
	    bool (*less)(struct latch_tree_node *a, struct latch_tree_node *b))
{
	struct rb_root *root = &ltr->tree[idx];
	struct rb_node **link = &root->rb_node;
	struct rb_node *node = &ltn->node[idx];
	struct rb_node *parent = NULL;
	struct latch_tree_node *ltp;

	while (*link) {
		parent = *link;
		ltp = __lt_from_rb(parent, idx);

		if (less(ltn, ltp))
			link = &parent->rb_left;
		else
			link = &parent->rb_right;
	}

	rb_link_node_rcu(node, parent, link);
	rb_insert_color(node, root);
}

static __always_inline void
__lt_erase(struct latch_tree_node *ltn, struct latch_tree_root *ltr, int idx)
{
	rb_erase(&ltn->node[idx], &ltr->tree[idx]);
}

static __always_inline struct latch_tree_node *
__lt_find(void *key, struct latch_tree_root *ltr, int idx,
	  int (*comp)(void *key, struct latch_tree_node *node))
{
	struct rb_node *node = rcu_dereference_raw(ltr->tree[idx].rb_node);
	struct latch_tree_node *ltn;
	int c;

	while (node) {
		ltn = __lt_from_rb(node, idx);
		c = comp(key, ltn);

		if (c < 0)
			node = rcu_dereference_raw(node->rb_left);
		else if (c > 0)
			node = rcu_dereference_raw(node->rb_right);
		else
			return ltn;
	}

	return NULL;
}


static __always_inline void
latch_tree_insert(struct latch_tree_node *node,
		  struct latch_tree_root *root,
		  const struct latch_tree_ops *ops)
{
	raw_write_seqcount_latch(&root->seq);
	__lt_insert(node, root, 0, ops->less);
	raw_write_seqcount_latch(&root->seq);
	__lt_insert(node, root, 1, ops->less);
}


static __always_inline void
latch_tree_erase(struct latch_tree_node *node,
		 struct latch_tree_root *root,
		 const struct latch_tree_ops *ops)
{
	raw_write_seqcount_latch(&root->seq);
	__lt_erase(node, root, 0);
	raw_write_seqcount_latch(&root->seq);
	__lt_erase(node, root, 1);
}


static __always_inline struct latch_tree_node *
latch_tree_find(void *key, struct latch_tree_root *root,
		const struct latch_tree_ops *ops)
{
	struct latch_tree_node *node;
	unsigned int seq;

	do {
		seq = raw_read_seqcount_latch(&root->seq);
		node = __lt_find(key, root, seq & 1, ops->comp);
	} while (read_seqcount_retry(&root->seq, seq));

	return node;
}

#endif 
