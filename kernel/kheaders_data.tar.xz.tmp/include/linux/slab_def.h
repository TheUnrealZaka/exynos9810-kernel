#ifndef _LINUX_SLAB_DEF_H
#define	_LINUX_SLAB_DEF_H

#include <linux/reciprocal_div.h>



struct kmem_cache {
	struct array_cache __percpu *cpu_cache;


	unsigned int batchcount;
	unsigned int limit;
	unsigned int shared;

	unsigned int size;
	struct reciprocal_value reciprocal_buffer_size;


	unsigned int flags;		
	unsigned int num;		


	
	unsigned int gfporder;

	
	gfp_t allocflags;

	size_t colour;			
	unsigned int colour_off;	
	struct kmem_cache *freelist_cache;
	unsigned int freelist_size;

	
	void (*ctor)(void *obj);


	const char *name;
	struct list_head list;
	int refcount;
	int object_size;
	int align;


#ifdef CONFIG_DEBUG_SLAB
	unsigned long num_active;
	unsigned long num_allocations;
	unsigned long high_mark;
	unsigned long grown;
	unsigned long reaped;
	unsigned long errors;
	unsigned long max_freeable;
	unsigned long node_allocs;
	unsigned long node_frees;
	unsigned long node_overflow;
	atomic_t allochit;
	atomic_t allocmiss;
	atomic_t freehit;
	atomic_t freemiss;
#ifdef CONFIG_DEBUG_SLAB_LEAK
	atomic_t store_user_clean;
#endif

	
	int obj_offset;
#endif 

#ifdef CONFIG_MEMCG
	struct memcg_cache_params memcg_params;
#endif
#ifdef CONFIG_KASAN
	struct kasan_cache kasan_info;
#endif

#ifdef CONFIG_SLAB_FREELIST_RANDOM
	unsigned int *random_seq;
#endif

	struct kmem_cache_node *node[MAX_NUMNODES];
};

static inline void *nearest_obj(struct kmem_cache *cache, struct page *page,
				void *x)
{
	void *object = x - (x - page->s_mem) % cache->size;
	void *last_object = page->s_mem + (cache->num - 1) * cache->size;

	if (unlikely(object > last_object))
		return last_object;
	else
		return object;
}

#endif	
