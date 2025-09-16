

#ifndef _ZS_MALLOC_H_
#define _ZS_MALLOC_H_

#include <linux/types.h>


enum zs_mapmode {
	ZS_MM_RW, 
	ZS_MM_RO, 
	ZS_MM_WO, 
#ifdef CONFIG_ZSWAP_SAME_PAGE_SHARING
	ZS_MM_RO_NOWAIT 
#endif
	
};

struct zs_pool_stats {
	
	atomic_long_t pages_compacted;
};

struct zs_pool;

struct zs_pool *zs_create_pool(const char *name);
void zs_destroy_pool(struct zs_pool *pool);

unsigned long zs_malloc(struct zs_pool *pool, size_t size, gfp_t flags);
void zs_free(struct zs_pool *pool, unsigned long obj);

size_t zs_huge_class_size(struct zs_pool *pool);

void *zs_map_object(struct zs_pool *pool, unsigned long handle,
			enum zs_mapmode mm);
void zs_unmap_object(struct zs_pool *pool, unsigned long handle);

unsigned long zs_get_total_pages(struct zs_pool *pool);
unsigned long zs_compact(struct zs_pool *pool);

void zs_pool_stats(struct zs_pool *pool, struct zs_pool_stats *stats);
#endif
