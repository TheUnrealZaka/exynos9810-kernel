#ifndef _LINUX_SHRINKER_H
#define _LINUX_SHRINKER_H


struct shrink_control {
	gfp_t gfp_mask;

	
	unsigned long nr_to_scan;

	
	int nid;

	
	struct mem_cgroup *memcg;
};

#define SHRINK_STOP (~0UL)

struct shrinker {
	unsigned long (*count_objects)(struct shrinker *,
				       struct shrink_control *sc);
	unsigned long (*scan_objects)(struct shrinker *,
				      struct shrink_control *sc);

	int seeks;	
	long batch;	
	unsigned long flags;

	
	struct list_head list;
	
	atomic_long_t *nr_deferred;
};
#define DEFAULT_SEEKS 2 


#define SHRINKER_NUMA_AWARE	(1 << 0)
#define SHRINKER_MEMCG_AWARE	(1 << 1)

extern int register_shrinker(struct shrinker *);
extern void unregister_shrinker(struct shrinker *);
#endif
