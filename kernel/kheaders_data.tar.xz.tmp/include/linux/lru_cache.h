

#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/string.h> 
#include <linux/seq_file.h>




struct lc_element {
	struct hlist_node colision;
	struct list_head list;		 
	unsigned refcnt;
	
	unsigned lc_index;
	
	unsigned lc_number;
	
#define LC_FREE (~0U)

	
	unsigned lc_new_number;
};

struct lru_cache {
	
	struct list_head lru;
	struct list_head free;
	struct list_head in_use;
	struct list_head to_be_changed;

	
	struct kmem_cache *lc_cache;

	
	size_t element_size;
	
	size_t element_off;

	
	unsigned int nr_elements;
	
#define LC_MAX_ACTIVE	(1<<24)

	
	unsigned int max_pending_changes;
	
	unsigned int pending_changes;

	
	unsigned used; 
	unsigned long hits, misses, starving, locked, changed;

	
	unsigned long flags;


	void  *lc_private;
	const char *name;

	
	struct hlist_head *lc_slot;
	struct lc_element **lc_element;
};



enum {
	
	__LC_PARANOIA,

	
	__LC_DIRTY,

	
	__LC_LOCKED,

	
	__LC_STARVING,
};
#define LC_PARANOIA (1<<__LC_PARANOIA)
#define LC_DIRTY    (1<<__LC_DIRTY)
#define LC_LOCKED   (1<<__LC_LOCKED)
#define LC_STARVING (1<<__LC_STARVING)

extern struct lru_cache *lc_create(const char *name, struct kmem_cache *cache,
		unsigned max_pending_changes,
		unsigned e_count, size_t e_size, size_t e_off);
extern void lc_reset(struct lru_cache *lc);
extern void lc_destroy(struct lru_cache *lc);
extern void lc_set(struct lru_cache *lc, unsigned int enr, int index);
extern void lc_del(struct lru_cache *lc, struct lc_element *element);

extern struct lc_element *lc_get_cumulative(struct lru_cache *lc, unsigned int enr);
extern struct lc_element *lc_try_get(struct lru_cache *lc, unsigned int enr);
extern struct lc_element *lc_find(struct lru_cache *lc, unsigned int enr);
extern struct lc_element *lc_get(struct lru_cache *lc, unsigned int enr);
extern unsigned int lc_put(struct lru_cache *lc, struct lc_element *e);
extern void lc_committed(struct lru_cache *lc);

struct seq_file;
extern void lc_seq_printf_stats(struct seq_file *seq, struct lru_cache *lc);

extern void lc_seq_dump_details(struct seq_file *seq, struct lru_cache *lc, char *utext,
				void (*detail) (struct seq_file *, struct lc_element *));


static inline int lc_try_lock_for_transaction(struct lru_cache *lc)
{
	return !test_and_set_bit(__LC_LOCKED, &lc->flags);
}


extern int lc_try_lock(struct lru_cache *lc);


static inline void lc_unlock(struct lru_cache *lc)
{
	clear_bit(__LC_DIRTY, &lc->flags);
	clear_bit_unlock(__LC_LOCKED, &lc->flags);
}

extern bool lc_is_used(struct lru_cache *lc, unsigned int enr);

#define lc_entry(ptr, type, member) \
	container_of(ptr, type, member)

extern struct lc_element *lc_element_by_index(struct lru_cache *lc, unsigned i);
extern unsigned int lc_index_of(struct lru_cache *lc, struct lc_element *e);

#endif
