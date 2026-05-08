

#if !defined(IB_FMR_POOL_H)
#define IB_FMR_POOL_H

#include <rdma/ib_verbs.h>

struct ib_fmr_pool;


struct ib_fmr_pool_param {
	int                     max_pages_per_fmr;
	int                     page_shift;
	enum ib_access_flags    access;
	int                     pool_size;
	int                     dirty_watermark;
	void                  (*flush_function)(struct ib_fmr_pool *pool,
						void               *arg);
	void                   *flush_arg;
	unsigned                cache:1;
};

struct ib_pool_fmr {
	struct ib_fmr      *fmr;
	struct ib_fmr_pool *pool;
	struct list_head    list;
	struct hlist_node   cache_node;
	int                 ref_count;
	int                 remap_count;
	u64                 io_virtual_address;
	int                 page_list_len;
	u64                 page_list[0];
};

struct ib_fmr_pool *ib_create_fmr_pool(struct ib_pd             *pd,
				       struct ib_fmr_pool_param *params);

void ib_destroy_fmr_pool(struct ib_fmr_pool *pool);

int ib_flush_fmr_pool(struct ib_fmr_pool *pool);

struct ib_pool_fmr *ib_fmr_pool_map_phys(struct ib_fmr_pool *pool_handle,
					 u64                *page_list,
					 int                 list_len,
					 u64                 io_virtual_address);

int ib_fmr_pool_unmap(struct ib_pool_fmr *fmr);

#endif 
