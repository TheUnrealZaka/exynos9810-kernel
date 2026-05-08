
#ifndef _LRU_LIST_H
#define _LRU_LIST_H

#include <linux/list.h>
#include <linux/nodemask.h>
#include <linux/shrinker.h>

struct mem_cgroup;


enum lru_status {
	LRU_REMOVED,		
	LRU_REMOVED_RETRY,	
	LRU_ROTATE,		
	LRU_SKIP,		
	LRU_RETRY,		
};

struct list_lru_one {
	struct list_head	list;
	
	long			nr_items;
};

struct list_lru_memcg {
	
	struct list_lru_one	*lru[0];
};

struct list_lru_node {
	
	spinlock_t		lock;
	
	struct list_lru_one	lru;
#if defined(CONFIG_MEMCG) && !defined(CONFIG_SLOB)
	
	struct list_lru_memcg	*memcg_lrus;
#endif
	long nr_items;
} ____cacheline_aligned_in_smp;

struct list_lru {
	struct list_lru_node	*node;
#if defined(CONFIG_MEMCG) && !defined(CONFIG_SLOB)
	struct list_head	list;
	bool			memcg_aware;
#endif
};

void list_lru_destroy(struct list_lru *lru);
int __list_lru_init(struct list_lru *lru, bool memcg_aware,
		    struct lock_class_key *key);

#define list_lru_init(lru)		__list_lru_init((lru), false, NULL)
#define list_lru_init_key(lru, key)	__list_lru_init((lru), false, (key))
#define list_lru_init_memcg(lru)	__list_lru_init((lru), true, NULL)

int memcg_update_all_list_lrus(int num_memcgs);
void memcg_drain_all_list_lrus(int src_idx, int dst_idx);


bool list_lru_add(struct list_lru *lru, struct list_head *item);


bool list_lru_del(struct list_lru *lru, struct list_head *item);


unsigned long list_lru_count_one(struct list_lru *lru,
				 int nid, struct mem_cgroup *memcg);
unsigned long list_lru_count_node(struct list_lru *lru, int nid);

static inline unsigned long list_lru_shrink_count(struct list_lru *lru,
						  struct shrink_control *sc)
{
	return list_lru_count_one(lru, sc->nid, sc->memcg);
}

static inline unsigned long list_lru_count(struct list_lru *lru)
{
	long count = 0;
	int nid;

	for_each_node_state(nid, N_NORMAL_MEMORY)
		count += list_lru_count_node(lru, nid);

	return count;
}

void list_lru_isolate(struct list_lru_one *list, struct list_head *item);
void list_lru_isolate_move(struct list_lru_one *list, struct list_head *item,
			   struct list_head *head);

typedef enum lru_status (*list_lru_walk_cb)(struct list_head *item,
		struct list_lru_one *list, spinlock_t *lock, void *cb_arg);


unsigned long list_lru_walk_one(struct list_lru *lru,
				int nid, struct mem_cgroup *memcg,
				list_lru_walk_cb isolate, void *cb_arg,
				unsigned long *nr_to_walk);
unsigned long list_lru_walk_node(struct list_lru *lru, int nid,
				 list_lru_walk_cb isolate, void *cb_arg,
				 unsigned long *nr_to_walk);

static inline unsigned long
list_lru_shrink_walk(struct list_lru *lru, struct shrink_control *sc,
		     list_lru_walk_cb isolate, void *cb_arg)
{
	return list_lru_walk_one(lru, sc->nid, sc->memcg, isolate, cb_arg,
				 &sc->nr_to_scan);
}

static inline unsigned long
list_lru_walk(struct list_lru *lru, list_lru_walk_cb isolate,
	      void *cb_arg, unsigned long nr_to_walk)
{
	long isolated = 0;
	int nid;

	for_each_node_state(nid, N_NORMAL_MEMORY) {
		isolated += list_lru_walk_node(lru, nid, isolate,
					       cb_arg, &nr_to_walk);
		if (nr_to_walk <= 0)
			break;
	}
	return isolated;
}
#endif 
