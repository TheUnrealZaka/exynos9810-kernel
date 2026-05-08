


#ifndef __GENALLOC_H__
#define __GENALLOC_H__

#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/atomic.h>

struct device;
struct device_node;
struct gen_pool;


typedef unsigned long (*genpool_algo_t)(unsigned long *map,
			unsigned long size,
			unsigned long start,
			unsigned int nr,
			void *data, struct gen_pool *pool,
			unsigned long start_addr);


struct gen_pool {
	spinlock_t lock;
	struct list_head chunks;	
	int min_alloc_order;		

	genpool_algo_t algo;		
	void *data;

	const char *name;
};


struct gen_pool_chunk {
	struct list_head next_chunk;	
	atomic_long_t avail;
	phys_addr_t phys_addr;		
	unsigned long start_addr;	
	unsigned long end_addr;		
	unsigned long bits[0];		
};


struct genpool_data_align {
	int align;		
};


struct genpool_data_fixed {
	unsigned long offset;		
};

extern struct gen_pool *gen_pool_create(int, int);
extern phys_addr_t gen_pool_virt_to_phys(struct gen_pool *pool, unsigned long);
extern int gen_pool_add_virt(struct gen_pool *, unsigned long, phys_addr_t,
			     size_t, int);

static inline int gen_pool_add(struct gen_pool *pool, unsigned long addr,
			       size_t size, int nid)
{
	return gen_pool_add_virt(pool, addr, -1, size, nid);
}
extern void gen_pool_destroy(struct gen_pool *);
extern unsigned long gen_pool_alloc(struct gen_pool *, size_t);
extern unsigned long gen_pool_alloc_algo(struct gen_pool *, size_t,
		genpool_algo_t algo, void *data);
extern void *gen_pool_dma_alloc(struct gen_pool *pool, size_t size,
		dma_addr_t *dma);
extern void gen_pool_free(struct gen_pool *, unsigned long, size_t);
extern void gen_pool_for_each_chunk(struct gen_pool *,
	void (*)(struct gen_pool *, struct gen_pool_chunk *, void *), void *);
extern size_t gen_pool_avail(struct gen_pool *);
extern size_t gen_pool_size(struct gen_pool *);

extern void gen_pool_set_algo(struct gen_pool *pool, genpool_algo_t algo,
		void *data);

extern unsigned long gen_pool_first_fit(unsigned long *map, unsigned long size,
		unsigned long start, unsigned int nr, void *data,
		struct gen_pool *pool, unsigned long start_addr);

extern unsigned long gen_pool_fixed_alloc(unsigned long *map,
		unsigned long size, unsigned long start, unsigned int nr,
		void *data, struct gen_pool *pool, unsigned long start_addr);

extern unsigned long gen_pool_first_fit_align(unsigned long *map,
		unsigned long size, unsigned long start, unsigned int nr,
		void *data, struct gen_pool *pool, unsigned long start_addr);


extern unsigned long gen_pool_first_fit_order_align(unsigned long *map,
		unsigned long size, unsigned long start, unsigned int nr,
		void *data, struct gen_pool *pool, unsigned long start_addr);

extern unsigned long gen_pool_best_fit(unsigned long *map, unsigned long size,
		unsigned long start, unsigned int nr, void *data,
		struct gen_pool *pool, unsigned long start_addr);


extern struct gen_pool *devm_gen_pool_create(struct device *dev,
		int min_alloc_order, int nid, const char *name);
extern struct gen_pool *gen_pool_get(struct device *dev, const char *name);

bool addr_in_gen_pool(struct gen_pool *pool, unsigned long start,
			size_t size);

#ifdef CONFIG_OF
extern struct gen_pool *of_gen_pool_get(struct device_node *np,
	const char *propname, int index);
#else
static inline struct gen_pool *of_gen_pool_get(struct device_node *np,
	const char *propname, int index)
{
	return NULL;
}
#endif
#endif 
