

#ifndef _ZPOOL_H_
#define _ZPOOL_H_

struct zpool;

struct zpool_ops {
	int (*evict)(struct zpool *pool, unsigned long handle);
};


enum zpool_mapmode {
	ZPOOL_MM_RW, 
	ZPOOL_MM_RO, 
	ZPOOL_MM_WO, 
#ifdef CONFIG_ZSWAP_SAME_PAGE_SHARING
	ZPOOL_MM_RO_NOWAIT, 
#endif

	ZPOOL_MM_DEFAULT = ZPOOL_MM_RW
};

bool zpool_has_pool(char *type);

struct zpool *zpool_create_pool(const char *type, const char *name,
			gfp_t gfp, const struct zpool_ops *ops);

const char *zpool_get_type(struct zpool *pool);

void zpool_destroy_pool(struct zpool *pool);

int zpool_malloc(struct zpool *pool, size_t size, gfp_t gfp,
			unsigned long *handle);

void zpool_free(struct zpool *pool, unsigned long handle);

int zpool_shrink(struct zpool *pool, unsigned int pages,
			unsigned int *reclaimed);

void *zpool_map_handle(struct zpool *pool, unsigned long handle,
			enum zpool_mapmode mm);

void zpool_unmap_handle(struct zpool *pool, unsigned long handle);

u64 zpool_get_total_size(struct zpool *pool);



struct zpool_driver {
	char *type;
	struct module *owner;
	atomic_t refcount;
	struct list_head list;

	void *(*create)(const char *name,
			gfp_t gfp,
			const struct zpool_ops *ops,
			struct zpool *zpool);
	void (*destroy)(void *pool);

	int (*malloc)(void *pool, size_t size, gfp_t gfp,
				unsigned long *handle);
	void (*free)(void *pool, unsigned long handle);

	int (*shrink)(void *pool, unsigned int pages,
				unsigned int *reclaimed);

	void *(*map)(void *pool, unsigned long handle,
				enum zpool_mapmode mm);
	void (*unmap)(void *pool, unsigned long handle);

	u64 (*total_size)(void *pool);
};

void zpool_register_driver(struct zpool_driver *driver);

int zpool_unregister_driver(struct zpool_driver *driver);

#endif
