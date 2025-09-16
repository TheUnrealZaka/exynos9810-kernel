
#ifndef _LINUX_RADIX_TREE_H
#define _LINUX_RADIX_TREE_H

#include <linux/bitops.h>
#include <linux/preempt.h>
#include <linux/types.h>
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/rcupdate.h>


#define RADIX_TREE_ENTRY_MASK		3UL
#define RADIX_TREE_INTERNAL_NODE	1UL


#define RADIX_TREE_EXCEPTIONAL_ENTRY	2
#define RADIX_TREE_EXCEPTIONAL_SHIFT	2

static inline bool radix_tree_is_internal_node(void *ptr)
{
	return ((unsigned long)ptr & RADIX_TREE_ENTRY_MASK) ==
				RADIX_TREE_INTERNAL_NODE;
}



#define RADIX_TREE_MAX_TAGS 3

#ifndef RADIX_TREE_MAP_SHIFT
#define RADIX_TREE_MAP_SHIFT	(CONFIG_BASE_SMALL ? 4 : 6)
#endif

#define RADIX_TREE_MAP_SIZE	(1UL << RADIX_TREE_MAP_SHIFT)
#define RADIX_TREE_MAP_MASK	(RADIX_TREE_MAP_SIZE-1)

#define RADIX_TREE_TAG_LONGS	\
	((RADIX_TREE_MAP_SIZE + BITS_PER_LONG - 1) / BITS_PER_LONG)

#define RADIX_TREE_INDEX_BITS  (8  * sizeof(unsigned long))
#define RADIX_TREE_MAX_PATH (DIV_ROUND_UP(RADIX_TREE_INDEX_BITS, \
					  RADIX_TREE_MAP_SHIFT))


#define RADIX_TREE_COUNT_SHIFT	(RADIX_TREE_MAP_SHIFT + 1)
#define RADIX_TREE_COUNT_MASK	((1UL << RADIX_TREE_COUNT_SHIFT) - 1)

struct radix_tree_node {
	unsigned char	shift;	
	unsigned char	offset;	
	unsigned int	count;
	union {
		struct {
			
			struct radix_tree_node *parent;
			
			void *private_data;
		};
		
		struct rcu_head	rcu_head;
	};
	
	struct list_head private_list;
	void __rcu	*slots[RADIX_TREE_MAP_SIZE];
	unsigned long	tags[RADIX_TREE_MAX_TAGS][RADIX_TREE_TAG_LONGS];
};


struct radix_tree_root {
	gfp_t			gfp_mask;
	struct radix_tree_node	__rcu *rnode;
};

#define RADIX_TREE_INIT(mask)	{					\
	.gfp_mask = (mask),						\
	.rnode = NULL,							\
}

#define RADIX_TREE(name, mask) \
	struct radix_tree_root name = RADIX_TREE_INIT(mask)

#define INIT_RADIX_TREE(root, mask)					\
do {									\
	(root)->gfp_mask = (mask);					\
	(root)->rnode = NULL;						\
} while (0)

static inline bool radix_tree_empty(struct radix_tree_root *root)
{
	return root->rnode == NULL;
}




static inline void *radix_tree_deref_slot(void **pslot)
{
	return rcu_dereference(*pslot);
}


static inline void *radix_tree_deref_slot_protected(void **pslot,
							spinlock_t *treelock)
{
	return rcu_dereference_protected(*pslot, lockdep_is_held(treelock));
}


static inline int radix_tree_deref_retry(void *arg)
{
	return unlikely(radix_tree_is_internal_node(arg));
}


static inline int radix_tree_exceptional_entry(void *arg)
{
	
	return (unsigned long)arg & RADIX_TREE_EXCEPTIONAL_ENTRY;
}


static inline int radix_tree_exception(void *arg)
{
	return unlikely((unsigned long)arg & RADIX_TREE_ENTRY_MASK);
}


static inline void radix_tree_replace_slot(void **pslot, void *item)
{
	BUG_ON(radix_tree_is_internal_node(item));
	rcu_assign_pointer(*pslot, item);
}

int __radix_tree_create(struct radix_tree_root *root, unsigned long index,
			unsigned order, struct radix_tree_node **nodep,
			void ***slotp);
int __radix_tree_insert(struct radix_tree_root *, unsigned long index,
			unsigned order, void *);
static inline int radix_tree_insert(struct radix_tree_root *root,
			unsigned long index, void *entry)
{
	return __radix_tree_insert(root, index, 0, entry);
}
void *__radix_tree_lookup(struct radix_tree_root *root, unsigned long index,
			  struct radix_tree_node **nodep, void ***slotp);
void *radix_tree_lookup(struct radix_tree_root *, unsigned long);
void **radix_tree_lookup_slot(struct radix_tree_root *, unsigned long);
bool __radix_tree_delete_node(struct radix_tree_root *root,
			      struct radix_tree_node *node);
void *radix_tree_delete_item(struct radix_tree_root *, unsigned long, void *);
void *radix_tree_delete(struct radix_tree_root *, unsigned long);
void radix_tree_clear_tags(struct radix_tree_root *root,
			   struct radix_tree_node *node,
			   void **slot);
unsigned int radix_tree_gang_lookup(struct radix_tree_root *root,
			void **results, unsigned long first_index,
			unsigned int max_items);
unsigned int radix_tree_gang_lookup_slot(struct radix_tree_root *root,
			void ***results, unsigned long *indices,
			unsigned long first_index, unsigned int max_items);
int radix_tree_preload(gfp_t gfp_mask);
int radix_tree_maybe_preload(gfp_t gfp_mask);
int radix_tree_maybe_preload_order(gfp_t gfp_mask, int order);
void radix_tree_init(void);
void *radix_tree_tag_set(struct radix_tree_root *root,
			unsigned long index, unsigned int tag);
void *radix_tree_tag_clear(struct radix_tree_root *root,
			unsigned long index, unsigned int tag);
int radix_tree_tag_get(struct radix_tree_root *root,
			unsigned long index, unsigned int tag);
unsigned int
radix_tree_gang_lookup_tag(struct radix_tree_root *root, void **results,
		unsigned long first_index, unsigned int max_items,
		unsigned int tag);
unsigned int
radix_tree_gang_lookup_tag_slot(struct radix_tree_root *root, void ***results,
		unsigned long first_index, unsigned int max_items,
		unsigned int tag);
unsigned long radix_tree_range_tag_if_tagged(struct radix_tree_root *root,
		unsigned long *first_indexp, unsigned long last_index,
		unsigned long nr_to_tag,
		unsigned int fromtag, unsigned int totag);
int radix_tree_tagged(struct radix_tree_root *root, unsigned int tag);
unsigned long radix_tree_locate_item(struct radix_tree_root *root, void *item);

static inline void radix_tree_preload_end(void)
{
	preempt_enable();
}


struct radix_tree_iter {
	unsigned long	index;
	unsigned long	next_index;
	unsigned long	tags;
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	unsigned int	shift;
#endif
};

static inline unsigned int iter_shift(struct radix_tree_iter *iter)
{
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	return iter->shift;
#else
	return 0;
#endif
}

#define RADIX_TREE_ITER_TAG_MASK	0x00FF	
#define RADIX_TREE_ITER_TAGGED		0x0100	
#define RADIX_TREE_ITER_CONTIG		0x0200	


static __always_inline void **
radix_tree_iter_init(struct radix_tree_iter *iter, unsigned long start)
{
	
	iter->index = 0;
	iter->next_index = start;
	return NULL;
}


void **radix_tree_next_chunk(struct radix_tree_root *root,
			     struct radix_tree_iter *iter, unsigned flags);


static inline __must_check
void **radix_tree_iter_retry(struct radix_tree_iter *iter)
{
	iter->next_index = iter->index;
	iter->tags = 0;
	return NULL;
}

static inline unsigned long
__radix_tree_iter_add(struct radix_tree_iter *iter, unsigned long slots)
{
	return iter->index + (slots << iter_shift(iter));
}


static inline __must_check
void **radix_tree_iter_next(struct radix_tree_iter *iter)
{
	iter->next_index = __radix_tree_iter_add(iter, 1);
	iter->tags = 0;
	return NULL;
}


static __always_inline long
radix_tree_chunk_size(struct radix_tree_iter *iter)
{
	return (iter->next_index - iter->index) >> iter_shift(iter);
}

static inline struct radix_tree_node *entry_to_node(void *ptr)
{
	return (void *)((unsigned long)ptr & ~RADIX_TREE_INTERNAL_NODE);
}


static __always_inline void **
radix_tree_next_slot(void **slot, struct radix_tree_iter *iter, unsigned flags)
{
	if (flags & RADIX_TREE_ITER_TAGGED) {
		void *canon = slot;

		iter->tags >>= 1;
		if (unlikely(!iter->tags))
			return NULL;
		while (IS_ENABLED(CONFIG_RADIX_TREE_MULTIORDER) &&
					radix_tree_is_internal_node(slot[1])) {
			if (entry_to_node(slot[1]) == canon) {
				iter->tags >>= 1;
				iter->index = __radix_tree_iter_add(iter, 1);
				slot++;
				continue;
			}
			iter->next_index = __radix_tree_iter_add(iter, 1);
			return NULL;
		}
		if (likely(iter->tags & 1ul)) {
			iter->index = __radix_tree_iter_add(iter, 1);
			return slot + 1;
		}
		if (!(flags & RADIX_TREE_ITER_CONTIG)) {
			unsigned offset = __ffs(iter->tags);

			iter->tags >>= offset;
			iter->index = __radix_tree_iter_add(iter, offset + 1);
			return slot + offset + 1;
		}
	} else {
		long count = radix_tree_chunk_size(iter);
		void *canon = slot;

		while (--count > 0) {
			slot++;
			iter->index = __radix_tree_iter_add(iter, 1);

			if (IS_ENABLED(CONFIG_RADIX_TREE_MULTIORDER) &&
			    radix_tree_is_internal_node(*slot)) {
				if (entry_to_node(*slot) == canon)
					continue;
				iter->next_index = iter->index;
				break;
			}

			if (likely(*slot))
				return slot;
			if (flags & RADIX_TREE_ITER_CONTIG) {
				
				iter->next_index = 0;
				break;
			}
		}
	}
	return NULL;
}


#define radix_tree_for_each_slot(slot, root, iter, start)		\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter, 0)) ;	\
	     slot = radix_tree_next_slot(slot, iter, 0))


#define radix_tree_for_each_contig(slot, root, iter, start)		\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter,		\
				RADIX_TREE_ITER_CONTIG)) ;		\
	     slot = radix_tree_next_slot(slot, iter,			\
				RADIX_TREE_ITER_CONTIG))


#define radix_tree_for_each_tagged(slot, root, iter, start, tag)	\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter,		\
			      RADIX_TREE_ITER_TAGGED | tag)) ;		\
	     slot = radix_tree_next_slot(slot, iter,			\
				RADIX_TREE_ITER_TAGGED))

#endif 
