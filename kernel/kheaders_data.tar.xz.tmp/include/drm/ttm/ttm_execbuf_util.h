


#ifndef _TTM_EXECBUF_UTIL_H_
#define _TTM_EXECBUF_UTIL_H_

#include <ttm/ttm_bo_api.h>
#include <linux/list.h>



struct ttm_validate_buffer {
	struct list_head head;
	struct ttm_buffer_object *bo;
	bool shared;
};



extern void ttm_eu_backoff_reservation(struct ww_acquire_ctx *ticket,
				       struct list_head *list);



extern int ttm_eu_reserve_buffers(struct ww_acquire_ctx *ticket,
				  struct list_head *list, bool intr,
				  struct list_head *dups);



extern void ttm_eu_fence_buffer_objects(struct ww_acquire_ctx *ticket,
					struct list_head *list,
					struct fence *fence);

#endif
